/*
 * @file LevelLoader.cpp
 * @brief Définition de la classe LevelLoader.
 * @author Noah Blattner
 * @date Février 2023
 */

#include <QDir>
#include <QMessageBox>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include "LevelLoader.h"
#include "gamescene.h"
#include "sprite.h"
#include "resources.h"

//! Constructeur
//! \param scene La scène dans laquelle charger les niveaux
//! \param levelsPath Le chemin des niveaux
LevelLoader::LevelLoader(GameScene* scene, QString levelsPath) {
    m_pScene = scene;
    m_levelsPath = std::move(levelsPath);
}

//! Charge un niveau dans la scène
//! \param scene La scène dans laquelle charger le niveau
//! \param levelName Le nom du niveau
//! \return La liste des sprites chargés
QList<Sprite *> LevelLoader::loadLevel(const QString& levelName) {
    // Concaténation du chemin du niveau avec le nom du niveau
    QString levelPath = m_levelsPath + "/" + levelName;

    if (!levelName.endsWith(".json")) { // Si le nom du niveau ne finit pas par .json
        levelPath += ".json";
    }

    levelPath = QDir::toNativeSeparators(levelPath);

    if (!QFile::exists(levelPath)) { // Si le fichier n'existe pas
        // On affiche une erreur
        QMessageBox::critical(nullptr, "Erreur", "Le fichier de niveau " + levelName + " n'existe pas.");
        return {};
    }

    QFile file(levelPath);
    if (!file.open(QIODevice::ReadOnly)) { // Si on ne peut pas ouvrir le fichier
        // On affiche une erreur
        QMessageBox::critical(nullptr, "Erreur", "Impossible d'ouvrir le fichier de niveau " + levelName + ".");
        return {};
    }

    // On prend les données du fichier et on le ferme
    QJsonDocument jsonDocument = QJsonDocument::fromJson(file.readAll());
    file.close();

    // On récupère l'objet JSON
    QJsonObject levelObject = jsonDocument.object();

    // On charge l'arrière-plan
    m_pScene->setBackgroundImage(QImage(GameFramework::resourcesPath() + levelObject["background"].toString()));

    // On charge les sprites
    return loadSprites(levelObject["sprites"].toArray());
}

//! Charge les sprites dans la scène
//! \param scene La scène dans laquelle charger les sprites
//! \param spritesArray Le tableau JSON des sprites
QList<Sprite*> LevelLoader::loadSprites(const QJsonArray& spritesArray) {
    QList<Sprite*> sprites;

    // Pour chaque sprite
    for (QJsonValue spriteValue : spritesArray) {
        // On récupère l'objet JSON
        QJsonObject spriteJson = spriteValue.toObject();

        // On crée la sprite avec son image
        auto* sprite = new Sprite(QDir::toNativeSeparators(GameFramework::resourcesPath() + spriteJson["texturePath"].toString()));
        sprites.append(sprite); // On ajoute la sprite à la liste

        // On applique les transformations
        sprite->setPos(spriteJson["x"].toDouble(), spriteJson["y"].toDouble());
        sprite->setRotation(spriteJson["rotation"].toInt());
        sprite->setScale(spriteJson["scale"].toDouble());

        m_pScene->addSpriteToScene(sprite); // On ajoute la sprite à la scène
    }

    return sprites;
}

//! Décharge un niveau de la scène
void LevelLoader::unloadLevel() {
    for (Sprite* sprite : m_pScene->sprites()) {
        m_pScene->removeSpriteFromScene(sprite);
        delete sprite;
    }
}


