//
// Created by blatnoa on 31.01.2023.
//

#include "EditorManager.h"

#include "GameCore.h"
#include "GameScene.h"
#include "EditorSprite.h"
#include "SelectionZone.h"


EditorManager::EditorManager(GameCore* core) {
    m_pScene = core->getScene();

    // Connecte les signaux d'input aux fonctions de traitement
    connect(core, &GameCore::notifyKeyPressed, this, &EditorManager::onKeyPressed);
    connect(core, &GameCore::notifyKeyReleased, this, &EditorManager::onKeyReleased);
    connect(core, &GameCore::notifyMouseMoved, this, &EditorManager::onMouseMoved);
    connect(core, &GameCore::notifyMouseButtonPressed, this, &EditorManager::onMouseButtonPressed);
    connect(core, &GameCore::notifyMouseButtonReleased, this, &EditorManager::onMouseButtonReleased);
}

//! Traite la pression d'une touche du clavier.
void EditorManager::onKeyPressed(int key) {
    switch (key) {
        case Qt::Key_Shift:
            m_isShiftHeld = true;
            break;
    }
}

//! Traite le relâchement d'une touche du clavier.
void EditorManager::onKeyReleased(int key) {
    switch (key) {
        case Qt::Key_Shift:
            m_isShiftHeld = false;
            if (m_pMultiSelectionZone != nullptr) { // Si une multi-sélection est en cours
                // On termine la sélection
                selectMultipleEditorSprites(m_pMultiSelectionZone->endSelection());
                m_pMultiSelectionZone = nullptr;
            }
            break;
    }
}

//! Traite la pression d'un bouton de la souris.
void EditorManager::onMouseButtonPressed(QPointF mousePosition, Qt::MouseButtons buttons) {
    switch (buttons) {
        case Qt::LeftButton:
            if (m_isShiftHeld) {  // Si la touche shift est enfoncée
                // Créer une zone de sélection
                createSelectionZone(mousePosition);
            }
            break;
        case Qt::RightButton:
            // On désélectionne tous les sprites
            unSelectAllEditorSprites();
            break;
    }
}

//! Traite le mouvement de la souris.
void EditorManager::onMouseMoved(QPointF newMousePosition) {
    if (m_pMultiSelectionZone != nullptr) { // Si une zone de sélection est en cours
        // On met à jour la zone de sélection
        m_pMultiSelectionZone->updateSelection(newMousePosition);
    }
}

//! Traite le relâchement d'un bouton de la souris.
void EditorManager::onMouseButtonReleased(QPointF mousePosition, Qt::MouseButtons buttons) {
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
void EditorManager::onEditorSpriteClicked(EditorSprite *pEditSprite) {
    if (!m_isShiftHeld) { // Si la touche shift n'est pas enfoncée, on déselectionne tous les sprites
        unSelectAllEditorSprites();
    }

    // On sélectionne le sprite cliqué
    selectSingleEditorSprite(pEditSprite);
}

//! Crée un sprite d'éditeur.
//! \param imageFileName    Nom du fichier image à utiliser pour le sprite.
//! \param position         Position du sprite. Défaut : QPointF(0, 0)
void EditorManager::createEditorSprite(const QString& imageFileName, QPointF position) {
    auto* pEditorSprite = new EditorSprite(imageFileName);
    pEditorSprite->setPos(position);
    m_pScene->addSpriteToScene(pEditorSprite);

    // Connecte le signal de click du sprite à la fonction de traitement du click
    connect(pEditorSprite, &EditorSprite::editorSpriteLeftClicked, this, &EditorManager::onEditorSpriteClicked);
}

//! Sélectionne un sprite d'éditeur.
//! \param pEditSprite    Sprite d'éditeur à sélectionner.
void EditorManager::selectSingleEditorSprite(EditorSprite *pEditSprite) {
    // Ajoute le sprite cliqué à la liste des sprites sélectionnés
    m_pSelectedEditorSprites.append(pEditSprite);

    // Indique au sprite qu'il est sélectionné
    pEditSprite->setEditSelected(true);
}

//! Désélectionne tous les sprites d'éditeur.
void EditorManager::unSelectAllEditorSprites() {
    // Indique à tous les sprites qu'ils ne sont plus sélectionnés
    foreach (EditorSprite* pSprite, m_pSelectedEditorSprites) {
        pSprite->setEditSelected(false);
    }

    // Vide la liste des sprites sélectionnés
    m_pSelectedEditorSprites.clear();
}

//! Sélectionne plusieurs sprites d'éditeur.
void EditorManager::selectMultipleEditorSprites(const QList<EditorSprite *> &pEditSprites) {
    foreach (EditorSprite *pSprite, pEditSprites) {
            selectSingleEditorSprite(pSprite);
    }
}

//! Crée une zone de sélection à une position donnée.
//! \param startPositon    Position de la souris.
void EditorManager::createSelectionZone(QPointF startPositon) {
    if (m_pMultiSelectionZone != nullptr) {
        m_pMultiSelectionZone->endSelection();
        m_pMultiSelectionZone = nullptr;
    }

    m_pMultiSelectionZone = new SelectionZone(m_pScene, startPositon);
}
