//
// Created by blatnoa on 08.02.2023.
//

#include <QString>
#include <QMessageBox>
#include <QDir>
#include <QFileDialog>
#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include "GameFramework/resources.h"
#include "EditorSprite.h"
#include "EditorManager.h"
#include "SaveFileManager.h"
#include "TagsManager.h"

const QString SaveFileManager::DEFAULT_SAVE_DIR = GameFramework::resourcesPath() + "saves";

//! Sauvegarde l'état actuel de l'éditeur dans un fichier JSON
//! \param editorManager L'éditeur à sauvegarder
//! \param savePath Le chemin du fichier de sauvegarde
void SaveFileManager::save(EditorManager *editorManager, QString savePath) {
    // Création du dossier de sauvegarde s'il n'existe pas
    QDir dir(DEFAULT_SAVE_DIR);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    if (savePath.isEmpty()) { // Si le chemin est vide
        // On demande à l'utilisateur de choisir un chemin de sauvegarde (avec un nom de fichier par défaut)
        savePath = QFileDialog::getSaveFileName(nullptr, "Sauvegarder sous", DEFAULT_SAVE_DIR, "JSON file (*.json)");
        if (savePath.isEmpty()) { // Si le chemin est toujours vide, on annule
            return;
        }
    }

    if (!savePath.endsWith(".json")) { // Si le chemin ne finit pas par .json on l'ajoute
        savePath += ".json";
    }

    // On sauvegarde le fichier (on remplace le fichier s'il existe déjà)
    QFile file(savePath);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible d'ouvrir le fichier " + savePath);
        return;
    }

    // On écrit le fichier
    QJsonObject json = convertEditorToJsonObject(editorManager);
    file.write(QJsonDocument(json).toJson());
    file.close();
}

//! Charge un fichier JSON dans l'éditeur. Ceci remplace l'état actuel de l'éditeur.
//! \param editorManager L'éditeur dans lequel charger le fichier
//! \param saveFilePath Le chemin du fichier à charger
void SaveFileManager::load(EditorManager *editorManager, QString saveFilePath) {

    // On charge le JSON depuis le fichier
    QJsonDocument json = loadJsonDocument(saveFilePath);

    if (json.isEmpty()) { // Si le JSON est vide, on annule
        return;
    }

    editorManager->resetEditor(); // On vide l'éditeur

    // On charge le JSON dans l'éditeur
    loadJsonIntoEditor(editorManager, json.object());

    // On charge le fond
    QString backgroundPath = QDir::toNativeSeparators(json.object()["background"].toString());
    if (!backgroundPath.isEmpty()) {
        editorManager->setBackGroundImage( GameFramework::resourcesPath() + backgroundPath);
    }
}

//! Importe un fichier JSON dans l'éditeur. Ceci ajoute les sprites du fichier à l'état actuel de l'éditeur.
//! \param editorManager L'éditeur dans lequel importer le fichier
//! \param importFilePath Le chemin du fichier à importer
void SaveFileManager::import(EditorManager *editorManager, QString importFilePath) {

    // On charge le JSON depuis le fichier
    QJsonDocument json = loadJsonDocument(importFilePath);

    if (json.isEmpty()) { // Si le JSON est vide, on annule
        return;
    }

    // On importe le JSON dans l'éditeur
    loadJsonIntoEditor(editorManager, json.object());
}

//! Charge le données depuis un fichier JSON vers un objet QJsonDocument
//! \param saveFilePath Le chemin du fichier à charger
//! \return L'objet QJsonDocument contenant les données du fichier
QJsonDocument SaveFileManager::loadJsonDocument(QString saveFilePath) {
    if (saveFilePath.isEmpty() || !QFile::exists(saveFilePath)) { // Si le chemin est vide ou que le fichier n'existe pas
        // On demande à l'utilisateur de choisir un chemin de sauvegarde (avec un nom de fichier par défaut)
        saveFilePath = QFileDialog::getOpenFileName(nullptr, "Ouvrir", DEFAULT_SAVE_DIR, "JSON file (*.json)");
        if (saveFilePath.isEmpty()) { // Si le chemin est toujours vide, on annule
            return {};
        }
    }

    // On ouvre le fichier
    QFile file(saveFilePath);
    if (!file.open(QIODevice::ReadOnly)) { // Si le fichier ne peut pas être ouvert
        // On affiche une erreur
        QMessageBox::critical(nullptr, "Erreur", "Impossible d'ouvrir le fichier " + saveFilePath);
        return {};
    }

    // On lit le JSON depuis le fichier
    QJsonDocument json = QJsonDocument::fromJson(file.readAll());

    // On ferme le fichier
    file.close();

    return json;
}

/*****************
 * Conversions JSON -> Editeur et Editeur -> JSON
 *****************/

//! Convertit l'éditeur en objet JSON
//! \param editorManager L'éditeur à convertir
QJsonObject SaveFileManager::convertEditorToJsonObject(EditorManager* editorManager) {
    QJsonObject json;
    json["tags"] = convertTagsToJsonArray(TagsManager::getTags());
    json["sceneWidth"] = editorManager->getSceneSize().width();
    json["sceneHeight"] = editorManager->getSceneSize().height();
    json["background"] = QDir::toNativeSeparators(editorManager->getBackgroundImagePath()).remove(QDir::toNativeSeparators(GameFramework::resourcesPath()));
    json["sprites"] = convertSpritesToJsonArray(editorManager -> getEditorSprites());
    return json;
}

//! Convertit les tags en tableau JSON
QJsonArray SaveFileManager::convertTagsToJsonArray(const QList<QString> &tags) {
    QJsonArray json;
    for (const QString& tag : tags) {
        json.append(tag);
    }
    return json;
}

//! Convertit une liste de sprites en tableau JSON
//! \param sprites La liste de sprites à convertir
QJsonArray SaveFileManager::convertSpritesToJsonArray(const QList<EditorSprite *>& sprites) {
    QJsonArray json;
    qDebug() << QDir::toNativeSeparators(DEFAULT_SAVE_DIR);
    for (EditorSprite *sprite : sprites) { // Pour chaque sprite
        QJsonObject spriteJson;
        spriteJson["x"] = sprite->x();
        spriteJson["y"] = sprite->y();
        spriteJson["scale"] = sprite->scale();
        spriteJson["texturePath"] = QDir::toNativeSeparators(sprite->getImgPath()).remove(QDir::toNativeSeparators(GameFramework::resourcesPath()));
        spriteJson["rotation"] = sprite->rotation();
        spriteJson["tag"] = sprite->getTag();
        json.append(spriteJson);
    }
    return json;
}

//! Charge un fichier JSON et charge son contenu dans un éditeur. Ceci remplace l'état actuel de l'éditeur.
//! \param editorManager L'éditeur dans lequel charger le fichier
//! \param path Le chemin du fichier à charger
void SaveFileManager::loadJsonIntoEditor(EditorManager *editorManager, QJsonObject jsonObject) {
    // On charge les tags
    loadTagsFromJson(jsonObject["tags"].toArray());

    // On charge la taille de la scène
    editorManager->setSceneSize(QSize(jsonObject["sceneWidth"].toInt(), jsonObject["sceneHeight"].toInt()));

    // On charge les sprites
    QList<EditorSprite*> sprites = loadSpritesFromJson(jsonObject["sprites"] . toArray());
    for (EditorSprite* sprite : sprites) {
        editorManager->addEditorSprite(sprite);
    }

    // On vide l'historique
    editorManager->resetHistory();
}

//! Charge les tags depuis un tableau JSON
//! \param jsonArray Le tableau JSON à convertir
void SaveFileManager::loadTagsFromJson(const QJsonArray &jsonArray) {
    // On charge les tags
    for (QJsonValue jsonValue : jsonArray) { // Pour chaque tag
        TagsManager::addTag(jsonValue.toString());
    }
}

//! Convertit un tableau JSON en liste de sprites d'éditeur
//! \param jsonArray Le tableau JSON à convertir
QList<EditorSprite*> SaveFileManager::loadSpritesFromJson(const QJsonArray& jsonArray) {
    QList<EditorSprite*> sprites;
    for (QJsonValue jsonValue: jsonArray) { // Pour chaque sprite
        QJsonObject spriteJson = jsonValue . toObject();
        auto* sprite = new EditorSprite(
                QDir::toNativeSeparators(GameFramework::resourcesPath() + jsonValue["texturePath"] . toString()));
        sprite -> setX(spriteJson["x"] . toDouble());
        sprite -> setY(spriteJson["y"] . toDouble());
        sprite -> setRotation(spriteJson["rotation"] . toInt());
        sprite -> setScale(spriteJson["scale"] . toDouble());
        sprite -> setTag(spriteJson["tag"] . toString());
        sprites . append(sprite);
    }
    return sprites;
}