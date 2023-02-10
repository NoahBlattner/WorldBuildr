/*
 * @file LevelLoader.cpp
 * @brief Définition de la classe LevelLoader.
 * @author Noah Blattner
 * @date Février 2023
 */

#ifndef WORLDBUILDR_LEVELLOADER_H
#define WORLDBUILDR_LEVELLOADER_H

#include <QString>

class QList;
class Sprite;
class GameScene;

class LevelLoader {
public:
    explicit LevelLoader(QString levelsPath);

    QList<Sprite*> loadLevel(GameScene* scene, const QString& levelName);
    void unloadLevel(GameScene* scene);

private:
    QString m_levelsPath;

    static void loadSprites(GameScene* scene, const QJsonArray& spritesArray);
};


#endif //WORLDBUILDR_LEVELLOADER_H
