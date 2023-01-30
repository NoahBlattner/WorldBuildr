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

const int SCENE_WIDTH = 1280;

//! Initialise le contrôleur de jeu.
//! \param pGameCanvas  GameCanvas pour lequel cet objet travaille.
//! \param pParent      Pointeur sur le parent (afin d'obtenir une destruction automatique de cet objet).
GameCore::GameCore(GameCanvas* pGameCanvas, QObject* pParent) : QObject(pParent) {
    
    // Mémorise l'accès au canvas (qui gère le tick et l'affichage d'une scène)
    m_pGameCanvas = pGameCanvas;
    
    // Créé la scène de base et indique au canvas qu'il faut l'afficher.
    m_pScene = pGameCanvas->createScene(0, 0, SCENE_WIDTH, SCENE_WIDTH / GameFramework::screenRatio());
    pGameCanvas->setCurrentScene(m_pScene);
    
    // Trace un rectangle blanc tout autour des limites de la scène.
    m_pScene->addRect(m_pScene->sceneRect(), QPen(Qt::white));
    
    // Instancier et initialiser les sprites ici :
    createEditorSprite(GameFramework::imagesPath() + "demo/ball.png");
    createEditorSprite(GameFramework::imagesPath() + "demo/ufo1.png", QPointF(100, 100));

    // Démarre le tick pour que les animations qui en dépendent fonctionnent correctement.
    // Attention : il est important que l'enclenchement du tick soit fait vers la fin de cette fonction,
    // sinon le temps passé jusqu'au premier tick (ElapsedTime) peut être élevé et provoquer de gros
    // déplacements, surtout si le déboggueur est démarré.
    m_pGameCanvas->startTick();
}

//! Destructeur de GameCore : efface les scènes
GameCore::~GameCore() {
    delete m_pScene;
    m_pScene = nullptr;
}

//! Traite la pression d'une touche.
//! \param key Numéro de la touche (voir les constantes Qt)
//!
void GameCore::keyPressed(int key) {
    emit notifyKeyPressed(key);

    switch (key) {
        case Qt::Key_Shift:
            m_isShiftPressed = true;
            break;
    }

}

//! Traite le relâchement d'une touche.
//! \param key Numéro de la touche (voir les constantes Qt)
void GameCore::keyReleased(int key) {
    emit notifyKeyReleased(key);

    switch (key) {
        case Qt::Key_Shift:
            m_isShiftPressed = false;
            if (m_pMultiSelectionZone != nullptr) {
                m_pMultiSelectionZone->endSelection();
                m_pMultiSelectionZone = nullptr;
            }
            break;
    }
}

//! Cadence.
//! \param elapsedTimeInMilliseconds  Temps écoulé depuis le dernier appel.
void GameCore::tick(long long elapsedTimeInMilliseconds) {
}

//! La souris a été déplacée.
//! Pour que cet événement soit pris en compte, la propriété MouseTracking de GameView
//! doit être enclenchée avec GameCanvas::startMouseTracking().
void GameCore::mouseMoved(QPointF newMousePosition) {
    emit notifyMouseMoved(newMousePosition);

    if (m_pMultiSelectionZone != nullptr) {
        m_pMultiSelectionZone->updateSelection(newMousePosition);
    }
}

//! Traite l'appui sur un bouton de la souris.
void GameCore::mouseButtonPressed(QPointF mousePosition, Qt::MouseButtons buttons) {
    emit notifyMouseButtonPressed(mousePosition, buttons);

    switch (buttons) {
        case Qt::LeftButton:
            if (m_isShiftPressed) {
                createSelectionZone(mousePosition);
            }
            break;
        case Qt::RightButton:
            // On désélectionne tous les sprites
            unSelectAllEditorSprites();
            break;
    }
}

//! Traite le relâchement d'un bouton de la souris.
void GameCore::mouseButtonReleased(QPointF mousePosition, Qt::MouseButtons buttons) {
    emit notifyMouseButtonReleased(mousePosition, buttons);

    // Switch sur les boutons de la souris
    switch (buttons) {
        case Qt::LeftButton:
            if (m_pMultiSelectionZone != nullptr) {
                // On récupère tous les sprites qui sont dans la zone de sélection
                auto sprites = m_pMultiSelectionZone->getCollidingEditorSprites();

                // On sélectionne tous les sprites qui sont dans la zone de sélection
                selectMultipleEditorSprites(sprites);

                m_pMultiSelectionZone->endSelection();
                m_pMultiSelectionZone = nullptr;
            }
            break;
    }
}

//! Traite le click d'un sprite d'editeur.
//! \param pEditSprite    Sprite d'éditeur cliqué.
void GameCore::onEditorSpriteClicked(EditorSprite *pEditSprite) {
    if (!m_isShiftPressed) { // Si la touche shift n'est pas enfoncée, on déselectionne tous les sprites
        unSelectAllEditorSprites();
    }

    // On sélectionne le sprite cliqué
    selectSingleEditorSprite(pEditSprite);
}

//! Crée un sprite d'éditeur.
//! \param imageFileName    Nom du fichier image à utiliser pour le sprite.
//! \param position         Position du sprite. Défaut : QPointF(0, 0)
void GameCore::createEditorSprite(const QString& imageFileName, QPointF position) {
    auto* pEditorSprite = new EditorSprite(imageFileName);
    pEditorSprite->setPos(position);
    m_pScene->addSpriteToScene(pEditorSprite);

    // Connecte le signal de click du sprite à la fonction de traitement du click
    connect(pEditorSprite, &EditorSprite::editorSpriteLeftClicked, this, &GameCore::onEditorSpriteClicked);
}

//! Sélectionne un sprite d'éditeur.
//! \param pEditSprite    Sprite d'éditeur à sélectionner.
void GameCore::selectSingleEditorSprite(EditorSprite *pEditSprite) {
    // Ajoute le sprite cliqué à la liste des sprites sélectionnés
    m_pSelectedEditorSprites.append(pEditSprite);

    // Indique au sprite qu'il est sélectionné
    pEditSprite->setEditSelected(true);
}

//! Désélectionne tous les sprites d'éditeur.
void GameCore::unSelectAllEditorSprites() {
    // Indique à tous les sprites qu'ils ne sont plus sélectionnés
    foreach (EditorSprite* pSprite, m_pSelectedEditorSprites) {
        pSprite->setEditSelected(false);
    }

    // Vide la liste des sprites sélectionnés
    m_pSelectedEditorSprites.clear();
}

//! Sélectionne plusieurs sprites d'éditeur.
void GameCore::selectMultipleEditorSprites(const QList<EditorSprite *> &pEditSprites) {
    foreach (EditorSprite *pSprite, pEditSprites) {
        selectSingleEditorSprite(pSprite);
    }
}

//! Crée une zone de sélection à une position donnée.
//! \param startPositon    Position de la souris.
void GameCore::createSelectionZone(QPointF startPositon) {
    if (m_pMultiSelectionZone != nullptr) {
        m_pMultiSelectionZone->endSelection();
        m_pMultiSelectionZone = nullptr;
    }

    m_pMultiSelectionZone = new SelectionZone(startPositon);
    m_pScene->addSpriteToScene(m_pMultiSelectionZone);
}
