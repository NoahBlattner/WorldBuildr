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

//! Sauvegarde l'état actuel de l'éditeur dans un fichier JSON
//! \param editorManager L'éditeur à sauvegarder
//! \param path Le chemin du fichier de sauvegarde
void SaveFileManager::save(EditorManager *editorManager, QString path) {
    QString savesDirectory = GameFramework::resourcesPath() + "/saves";

    // Création du dossier de sauvegarde s'il n'existe pas
    QDir dir(savesDirectory);
    if (!dir.exists()) {
        dir.mkpath(".");
    }

    if (path.isEmpty()) { // Si le chemin est vide
        // On demande à l'utilisateur de choisir un chemin de sauvegarde (avec un nom de fichier par défaut)
        path = QFileDialog::getSaveFileName(nullptr, "Sauvegarder sous", savesDirectory, "JSON file (*.json)");
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
    // TODO
}

//! Convertit l'éditeur en objet JSON
//! \param editorManager L'éditeur à convertir
QJsonObject SaveFileManager::convertEditorToJsonObject(EditorManager *editorManager) {
    QJsonObject json;
    json["version"] = 1;
    json["name"] = editorManager->getName();
    json["background"] = editorManager->getBackgroundImage();
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
        spriteJson["texture"] = sprite->getImgPath();
        spriteJson["rotation"] = sprite->rotation();
        json.append(spriteJson);
    }
    return json;
}
