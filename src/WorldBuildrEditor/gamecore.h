/**
  \file
  \brief    Déclaration de la classe GameCore.
  \author   JCO
  \date     septembre 2018
*/
#ifndef GAMECORE_H
#define GAMECORE_H

#include <QObject>
#include <QPointF>
#include "sprite.h"


class GameCanvas;
class GameScene;
class EditorSprite;
class SelectionZone;
class EditorManager;
class EditorActionPanel;
class SpriteDetailsPanel;

//! \brief Classe qui gère la logique du jeu.
//!
//! Dans son état actuel, cette classe crée une scène vide, délimite
//! sa surface avec une ligne blanche puis démarre la cadence du jeu.
class GameCore : public QObject
{
    Q_OBJECT
public:
    explicit GameCore(GameCanvas* pGameCanvas, QObject *parent = nullptr);
    ~GameCore() override;

    GameScene* getScene() const;

    void keyPressed(int key);
    void keyReleased(int key);

    void mouseMoved(QPointF newMousePosition, QPointF oldMousePosition);
    void mouseButtonPressed(QPointF mousePosition, Qt::MouseButtons buttons);
    void mouseButtonReleased(QPointF mousePosition, Qt::MouseButtons buttons);

    void tick(long long elapsedTimeInMilliseconds);

signals:
    void notifyMouseMoved(QPointF newMousePosition, QPointF oldMousePosition);
    void notifyMouseButtonPressed(QPointF mousePosition, Qt::MouseButtons buttons);
    void notifyMouseButtonReleased(QPointF mousePosition, Qt::MouseButtons buttons);
    void notifyKeyPressed(int key);
    void notifyKeyReleased(int key);

private:
    GameCanvas* m_pGameCanvas = nullptr;
    GameScene* m_pScene = nullptr;

    EditorManager* m_pEditorManager = nullptr;
    EditorActionPanel* m_pActionPanel = nullptr;
    SpriteDetailsPanel* m_pSpriteDetailsPanel = nullptr;

};


#endif // GAMECORE_H
