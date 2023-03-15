/**
  Fichier qui contient toute la logique du jeu.
  
  @author   JCO
  @date     Février 2014
 */
#include "gamecore.h"

#include <cmath>

#include <QDebug>
#include <QSettings>
#include <iostream>

#include "gamescene.h"
#include "gamecanvas.h"
#include "resources.h"
#include "utilities.h"
#include "sprite.h"
#include "EditorSprite.h"
#include "SelectionZone.h"
#include "EditorManager.h"
#include "EditorActionPanel.h"
#include "GameView.h"

const int SCENE_WIDTH = 1280;

//! Initialise le contrôleur de jeu.
//! \param pGameCanvas  GameCanvas pour lequel cet objet travaille.
//! \param pParent      Pointeur sur le parent (afin d'obtenir une destruction automatique de cet objet).
GameCore::GameCore(GameCanvas* pGameCanvas, QObject* pParent) : QObject(pParent) {
    
    // Mémorise l'accès au canvas (qui gère le tick et l'affichage d'une scène)
    m_pGameCanvas = pGameCanvas;
    m_pActionPanel = pGameCanvas->getActionPanel();
    m_pSpriteDetailsPanel = pGameCanvas->getDetailsPanel();
    
    // Créé la scène de base et indique au canvas qu'il faut l'afficher.
    m_pScene = pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_WIDTH / GameFramework::screenRatio());
    pGameCanvas->setCurrentScene(m_pScene);

    // Créé le gestionnaire d'éditeur
    m_pEditorManager = new EditorManager(this);

    // Instancier et initialiser les sprites ici :
    m_pEditorManager->createNewEditorSprite(GameFramework::imagesPath() + "demo/ufo1.png", QPointF(100, 100));
    m_pEditorManager->createNewEditorSprite(GameFramework::imagesPath() + "demo/ball.png", QPointF(200, 200));
    m_pEditorManager->createNewEditorSprite(GameFramework::imagesPath() + "demo/tennisball.png", QPointF(300, 300));
    m_pEditorManager->createNewEditorSprite(GameFramework::imagesPath() + "demo/plane_cartoon.png", QPointF(400, 400));
    m_pEditorManager->setBackGroundImage(GameFramework::imagesPath() + "demo/landscape_background.jpg");

    // Réinitialise l'historique pour supprimer les actions de création de sprites
    m_pEditorManager->resetHistory();

    // Lie le gestionnaire d'éditeur au panneaux
    m_pActionPanel->bindEditorManager(m_pEditorManager);
    m_pSpriteDetailsPanel->bindEditorManager(m_pEditorManager);

    // Démarre le tick pour que les animations qui en dépendent fonctionnent correctement.
    // Attention : il est important que l'enclenchement du tick soit fait vers la fin de cette fonction,
    // sinon le temps passé jusqu'au premier tick (ElapsedTime) peut être élevé et provoquer de gros
    // déplacements, surtout si le débogueur est démarré.
    m_pGameCanvas->startTick();
}

//! Destructeur de GameCore : efface les scènes
GameCore::~GameCore() {
    delete m_pEditorManager;
    delete m_pScene;
    m_pScene = nullptr;
    m_pEditorManager = nullptr;
}

//! Traite la pression d'une touche.
//! \param key Numéro de la touche (voir les constantes Qt)
//!
void GameCore::keyPressed(int key) {
    emit notifyKeyPressed(key);
}

//! Traite le relâchement d'une touche.
//! \param key Numéro de la touche (voir les constantes Qt)
void GameCore::keyReleased(int key) {
    emit notifyKeyReleased(key);
}

//! Cadence.
//! \param elapsedTimeInMilliseconds  Temps écoulé depuis le dernier appel.
void GameCore::tick(long long elapsedTimeInMilliseconds) {
}

//! La souris a été déplacée.
//! Pour que cet événement soit pris en compte, la propriété MouseTracking de GameView
//! doit être enclenchée avec GameCanvas::startMouseTracking().
void GameCore::mouseMoved(QPointF newMousePosition, QPointF oldMousePosition) {
    emit notifyMouseMoved(newMousePosition, oldMousePosition);
}

//! Traite l'appui sur un bouton de la souris.
void GameCore::mouseButtonPressed(QPointF mousePosition, Qt::MouseButtons buttons) {
    emit notifyMouseButtonPressed(mousePosition, buttons);
}

//! Traite le relâchement d'un bouton de la souris.
void GameCore::mouseButtonReleased(QPointF mousePosition, Qt::MouseButtons buttons) {
    emit notifyMouseButtonReleased(mousePosition, buttons);
}

//! Retourne la scène de jeu.
GameScene *GameCore::getScene() const {
    return m_pScene;
}
