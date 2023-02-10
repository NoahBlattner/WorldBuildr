/*
 * @file LevelLoader.cpp
 * @brief Définition de la classe LevelLoader.
 * @author Noah Blattner
 * @date Février 2023
 */

#ifndef WORLDBUILDR_LEVELLOADER_H
#define WORLDBUILDR_LEVELLOADER_H

#include <QString>
#include <QList>

class QJsonArray;
class Sprite;
class GameScene;

class LevelLoader {
public:
    explicit LevelLoader(GameScene* scene, QString levelsPath);

    QList<Sprite*> loadLevel(const QString& levelName);
    void unloadLevel();

private:
    GameScene* m_pScene;
    QString m_levelsPath;

    QList<Sprite*> loadSprites(const QJsonArray& spritesArray);
};


#endif //WORLDBUILDR_LEVELLOADER_H
