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

#include "resources.h"
#include "EditorSprite.h"
#include "EditorManager.h"
#include "SaveFileManager.h"

const QString SaveFileManager::DEFAULT_SAVE_DIR = GameFramework::resourcesPath() + "/saves";

//! Sauvegarde l'état actuel de l'éditeur dans un fichier JSON
//! \param editorManager L'éditeur à sauvegarder
//! \param path Le chemin du fichier de sauvegarde
void SaveFileManager::save(EditorManager *editorManager, QString path) {
    // Création du dossier de sauvegarde s'il n'existe pas
    QDir dir(DEFAULT_SAVE_DIR);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    if (path.isEmpty()) { // Si le chemin est vide
        // On demande à l'utilisateur de choisir un chemin de sauvegarde (avec un nom de fichier par défaut)
        path = QFileDialog::getSaveFileName(nullptr, "Sauvegarder sous", DEFAULT_SAVE_DIR, "JSON file (*.json)");
        if (path.isEmpty()) { // Si le chemin est toujours vide, on annule
            return;
        }
    }

    if (!path.endsWith(".json")) { // Si le chemin ne finit pas par .json on l'ajoute
        path += ".json";
    }

    // On sauvegarde le fichier (on remplace le fichier s'il existe déjà)
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible d'ouvrir le fichier " + path);
        return;
    }

    // On écrit le fichier
    QJsonObject json = convertEditorToJsonObject(editorManager);
    file.write(QJsonDocument(json).toJson());
    file.close();
}

void SaveFileManager::load(EditorManager *editorManager, QString path) {
    if (path.isEmpty() || !QFile::exists(path)) { // Si le chemin est vide ou que le fichier n'existe pas
        // On demande à l'utilisateur de choisir un chemin de sauvegarde (avec un nom de fichier par défaut)
        path = QFileDialog::getOpenFileName(nullptr, "Ouvrir", DEFAULT_SAVE_DIR, "JSON file (*.json)");
        if (path.isEmpty()) { // Si le chemin est toujours vide, on annule
            return;
        }
    }

    // On ouvre le fichier
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible d'ouvrir le fichier " + path);
        return;
    }

    editorManager->resetEditor(); // On vide l'éditeur

    // On lit le JSON depuis le fichier
    QJsonDocument json = QJsonDocument::fromJson(file.readAll());

    // On ferme le fichier
    file.close();

    // On charge le JSON dans l'éditeur
    loadJsonIntoEditor(editorManager, json.object());

}

//! Convertit l'éditeur en objet JSON
//! \param editorManager L'éditeur à convertir
QJsonObject SaveFileManager::convertEditorToJsonObject(EditorManager* editorManager) {
    QJsonObject json;
    json["background"] = editorManager->getBackgroundImagePath();
    json["sprites"] = convertSpritesToJsonObject(editorManager->getEditorSprites());
    return json;
}

//! Convertit une liste de sprites en tableau JSON
//! \param sprites La liste de sprites à convertir
QJsonArray SaveFileManager::convertSpritesToJsonObject(const QList<EditorSprite *>& sprites) {
    QJsonArray json;
    for (EditorSprite *sprite : sprites) { // Pour chaque sprite
        QJsonObject spriteJson;
        spriteJson["x"] = sprite->x();
        spriteJson["y"] = sprite->y();
        spriteJson["width"] = sprite->width();
        spriteJson["height"] = sprite->height();
        spriteJson["texturePath"] = sprite->getImgPath();
        spriteJson["rotation"] = sprite->rotation();
        json.append(spriteJson);
    }
    return json;
}

//! Charge un fichier JSON et charge son contenu dans un éditeur
//! \param editorManager L'éditeur dans lequel charger le fichier
//! \param path Le chemin du fichier à charger
void SaveFileManager::loadJsonIntoEditor(EditorManager *editorManager, QJsonObject jsonObject) {
    // On charge le fond
    editorManager->setBackGroundImage(jsonObject["background"].toString());

    // On charge les sprites
    QList<EditorSprite *> sprites = generateSpritesFromJson(jsonObject["sprites"].toArray());
    for (EditorSprite *sprite : sprites) {
        editorManager->addEditorSprite(sprite);
    }
}

//! Convertit un tableau JSON en liste de sprites d'éditeur
//! \param jsonArray Le tableau JSON à convertir
QList<EditorSprite *> SaveFileManager::generateSpritesFromJson(const QJsonArray& jsonArray) {
    QList<EditorSprite *> sprites;
    for (QJsonValue jsonValue : jsonArray) { // Pour chaque sprite
        QJsonObject spriteJson = jsonValue.toObject();
        auto* sprite = new EditorSprite(spriteJson["texturePath"].toString());
        sprite->setPos(spriteJson["x"].toInt(), spriteJson["y"].toInt());
        sprite->setRotation(spriteJson["rotation"].toInt());
        sprite->setTransform(QTransform::fromScale(spriteJson["width"].toInt() / sprite->width(), spriteJson["height"].toInt() / sprite->height()));
        sprites.append(sprite);
    }
    return sprites;
}
