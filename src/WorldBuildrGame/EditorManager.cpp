//
// Created by blatnoa on 31.01.2023.
//

#include <QFileDialog>
#include "EditorManager.h"

#include "GameCore.h"
#include "GameScene.h"
#include "EditorSprite.h"
#include "SelectionZone.h"
#include "resources.h"

EditorManager::EditorManager(GameCore* core) {
    m_pScene = core->getScene();

    // Connecte les signaux d'input aux fonctions de traitement
    connect(core, &GameCore::notifyKeyPressed, this, &EditorManager::onKeyPressed);
    connect(core, &GameCore::notifyKeyReleased, this, &EditorManager::onKeyReleased);
    connect(core, &GameCore::notifyMouseMoved, this, &EditorManager::onMouseMoved);
    connect(core, &GameCore::notifyMouseButtonPressed, this, &EditorManager::onMouseButtonPressed);
    connect(core, &GameCore::notifyMouseButtonReleased, this, &EditorManager::onMouseButtonReleased);
}

//! Copie une image sélectionnée dans le dossier d'images de l'éditeur
//! et retourne le chemin de la nouvelle image
//! \return Le chemin de l'image6
QString EditorManager::loadImageToEditor() {
    // Ouverture d'une boîte de dialogue pour charger une image
    QString imagePath = QFileDialog::getOpenFileName(this, tr("Load Image"), GameFramework::imagesPath(), tr("Image Files (*.png *.jpg *.bmp)"));
    QString newImagePath = GameFramework::imagesPath() + "editorImages/" + imagePath.split("/").last();
    QFile::copy(imagePath, newImagePath);

    return newImagePath;
}

/********************************************
 * Gestion des touche et boutons de la souris
 *******************************************/

//! Traite la pression d'une touche du clavier.
void EditorManager::onKeyPressed(int key) {
    switch (key) {
        case Qt::Key_Shift:
            m_isShiftHeld = true;
            break;
        case Qt::Key_Control:
            m_isCtrlHeld = true;
            break;
        case Qt::Key_Escape:
            // On désélectionne tous les sprites
            unselectAllEditorSprites();
            break;
        case Qt::Key_A:
            if (m_isCtrlHeld) {
                // On sélectionne tous les sprites
                selectMultipleEditorSprites(m_pEditorSprites);
            }
            break;
        case Qt::Key_N:
            if (m_isCtrlHeld) {
                // On crée un nouveau sprite
                addNewEditorSprite();
            }
            break;
    }
}

//! Traite le relâchement d'une touche du clavier.
void EditorManager::onKeyReleased(int key) {
    switch (key) {
        case Qt::Key_Shift:
            m_isShiftHeld = false;
            break;
        case Qt::Key_Control:
            m_isCtrlHeld = false;
            break;
        case Qt::Key_Delete:
            deleteSelectedEditorSprites();
            break;
    }
}

//! Traite la pression d'un bouton de la souris.
void EditorManager::onMouseButtonPressed(QPointF mousePosition, Qt::MouseButtons buttons) {
    // Ajout le bouton à l'état de la souris
    m_heldMouseButtons |= buttons;

    switch (buttons) {
        case Qt::LeftButton:
            // Get le sprite cliqué
            mouseDownEditorSprite = dynamic_cast<EditorSprite *>(m_pScene->spriteAt(mousePosition));
            if (mouseDownEditorSprite == nullptr) {  // Si on a pas cliqué sur un sprite
                createSelectionZone(mousePosition);
            }
            break;
        case Qt::RightButton:
            // On désélectionne tous les sprites
            unselectAllEditorSprites();
            break;
    }
}

//! Traite le mouvement de la souris.
void EditorManager::onMouseMoved(QPointF newMousePosition, QPointF oldMousePosition) {
    // Switch sur les boutons de la souris appuyés durant le mouvement
    switch (m_heldMouseButtons) {
        case Qt::LeftButton:
            if (m_pMultiSelectionZone != nullptr) { // Si une zone de sélection est en cours
                // On met à jour la sélection
                updateMultiSelect(newMousePosition);
            } else if (m_isShiftHeld) {
                createSelectionZone(newMousePosition);
            } else if (mouseDownEditorSprite != nullptr) { // Sinon si on a précédemment cliqué sur un sprite
                if (mouseDownEditorSprite->getEditSelected()) {
                    // On déplace le sprite
                    moveSelectedEditorSprites(newMousePosition-oldMousePosition);
                } else {
                    selectSingleEditorSprite(mouseDownEditorSprite);
                }
            }
            break;
    }
}

//! Traite le relâchement d'un bouton de la souris.
void EditorManager::onMouseButtonReleased(QPointF mousePosition, Qt::MouseButtons buttons) {
    // Enlève le bouton de l'état de la souris
    m_heldMouseButtons &= ~buttons;

    // Switch sur les boutons de la souris
    switch (buttons) {
        case Qt::LeftButton:
            EditorSprite* mouseUpSprite;
            mouseUpSprite = dynamic_cast<EditorSprite *>(m_pScene->spriteAt(mousePosition));
            if (mouseUpSprite != nullptr && mouseUpSprite == mouseDownEditorSprite) { // Si le sprite relâché est le même que le sprite cliqué
                // On sélectionne le sprite
                editorSpriteClicked(mouseUpSprite);
            } else if (m_pMultiSelectionZone != nullptr) { // Sinon si une zone de sélection est en cours
                // On récupère tous les sprites qui sont dans la zone de sélection
                auto sprites = m_pMultiSelectionZone->endSelection();

                // On désélectionne tous les sprites
                unselectAllEditorSprites();

                // On sélectionne tous les sprites qui sont dans la zone de sélection
                selectMultipleEditorSprites(sprites);

                m_pMultiSelectionZone = nullptr;
            }
            mouseDownEditorSprite = nullptr;
            break;
    }
}

/********************************************
 * Gestion de la sélection
 *******************************************/

//! Sélectionne un sprite d'éditeur.
//! \param pEditSprite    Sprite d'éditeur à sélectionner.
void EditorManager::selectEditorSprite(EditorSprite *pEditSprite) {
    // Ajoute le sprite cliqué à la liste des sprites sélectionnés
    if (!m_pSelectedEditorSprites.contains(pEditSprite)) {
        m_pSelectedEditorSprites.append(pEditSprite);

        // Indique au sprite qu'il est sélectionné
        pEditSprite->setEditSelected(true);
    }
}

//! Sélectionne un sprite d'éditeur en désélectionnant tout les autres.
void EditorManager::selectSingleEditorSprite(EditorSprite *pEditSprite) {
    // Désélectionne tous les sprites
    unselectAllEditorSprites();

    // Sélectionne le sprite
    selectEditorSprite(pEditSprite);
}

//! Change la sélection d'un sprite d'éditeur.
void EditorManager::toggleSelectEditorSprite(EditorSprite* pEditSprite) {
    if (m_pSelectedEditorSprites.contains(pEditSprite)) {
        unselectEditorSprite(pEditSprite);
    } else {
        selectEditorSprite(pEditSprite);
    }
}

//! Désélectionne un sprite d'éditeur.
void EditorManager::unselectEditorSprite(EditorSprite* pEditSprite) {
    // Enlève le sprite de la liste des sprites sélectionnés
    m_pSelectedEditorSprites.removeOne(pEditSprite);

    // Indique au sprite qu'il n'est plus sélectionné
    pEditSprite->setEditSelected(false);
}

//! Désélectionne tous les sprites d'éditeur.
void EditorManager::unselectAllEditorSprites() {
    // Indique à tous les sprites qu'ils ne sont plus sélectionnés
    foreach (EditorSprite* pSprite, m_pSelectedEditorSprites) {
        pSprite->setEditSelected(false);
    }

    // Vide la liste des sprites sélectionnés
    m_pSelectedEditorSprites.clear();
}

/********************************************
* Gestion de la multi-sélection
 *******************************************/

//! Crée une zone de sélection à une position donnée.
//! \param startPositon    Position de la souris.
void EditorManager::createSelectionZone(QPointF startPositon) {
    if (m_pMultiSelectionZone != nullptr) {
        m_pMultiSelectionZone->endSelection();
    }

    m_pMultiSelectionZone = new SelectionZone(m_pScene, startPositon);
}

//! Met à jour la multi-sélection.
void EditorManager::updateMultiSelect(QPointF &newMousePosition) {// On met à jour la zone de sélection
    m_pMultiSelectionZone->updateSelection(newMousePosition);
    selectMultipleEditorSprites(m_pMultiSelectionZone->getCollidingEditorSprites());
}

//! Sélectionne plusieurs sprites d'éditeur.
void EditorManager::selectMultipleEditorSprites(const QList<EditorSprite*> &pEditSprites) {
    foreach (EditorSprite* pSprite, pEditSprites) {
        selectEditorSprite(pSprite);
    }
}

//! Sélectionne tous les sprites d'éditeur.
void EditorManager::selectAllEditorSprites() {
    selectMultipleEditorSprites(m_pEditorSprites);
}

/********************************************
 * Gestion des sprites d'éditeur
 *******************************************/

//! Crée un sprite d'éditeur.
//! \param imageFileName    Nom du fichier image à utiliser pour le sprite.
//! \param position         Position du sprite. Défaut : QPointF(0, 0)
void EditorManager::addNewEditorSprite(QString imageFileName, QPointF position) {
    if (imageFileName.isEmpty()) { // Si le nom du fichier image est vide
        // On demande à l'utilisateur de choisir un fichier image
        imageFileName = loadImageToEditor();
    }

    // On crée le sprite d'éditeur
    auto* pEditorSprite = new EditorSprite(imageFileName);

    // On ajoute le sprite à la liste des sprites d'éditeur
    m_pEditorSprites.append(pEditorSprite);

    // On centre le point d'origine du sprite
    pEditorSprite->setTransformOriginPoint(pEditorSprite->boundingRect().center());

    // On place le sprite à la position donnée
    pEditorSprite->setPos(position);

    // On ajoute le sprite à la scène
    m_pScene->addSpriteToScene(pEditorSprite);

    // Connecte le signal de click du sprite à la fonction de traitement du click
    connect(pEditorSprite, &EditorSprite::editorSpriteLeftClicked, this, &EditorManager::editorSpriteClicked);
}

//! Traite le click d'un sprite d'editeur.
//! \param pEditSprite    Sprite d'éditeur cliqué.
void EditorManager::editorSpriteClicked(EditorSprite* pEditSprite) {
    if (m_isShiftHeld) { // Si la touche shift est enfoncée
        // On change la sélection du sprite
        toggleSelectEditorSprite(pEditSprite);
    } else {
        // On sélectionne ce sprite uniquement
        selectSingleEditorSprite(pEditSprite);
    }
}

//! Déplace tous les sprites sélectionnés d'un vecteur donné.
void EditorManager::moveSelectedEditorSprites(QPointF moveVector) {
    foreach (EditorSprite* pSprite, m_pSelectedEditorSprites) {
        // Calcule la nouvelle position du sprite
        QRectF currentSpriteBounds = pSprite->sceneBoundingRect();
        QRectF newSpriteBounds = currentSpriteBounds;
        newSpriteBounds.translate(moveVector);

        // Si le sprite sort de la scène sur l'axe X,
        // on le déplace pour qu'il soit à la limite de la scène
        if (newSpriteBounds.left() < 0) {
            moveVector.setX(-currentSpriteBounds.left());
        } else if (newSpriteBounds.right() > m_pScene->sceneRect().right()) {
            moveVector.setX(m_pScene->sceneRect().right() - currentSpriteBounds.right());
        }

        // Si le sprite sort de la scène sur l'axe Y,
        // on le déplace pour qu'il soit à la limite de la scène
        if (newSpriteBounds.top() < 0) {
            moveVector.setY(-currentSpriteBounds.top());
        } else if (newSpriteBounds.bottom() > m_pScene->sceneRect().bottom()) {
            moveVector.setY(m_pScene->sceneRect().bottom() - currentSpriteBounds.bottom());
        }

        // Déplace le sprite
        pSprite->moveBy(moveVector.x(), moveVector.y());

    }
}

//! Supprime un sprite d'éditeur.
void EditorManager::deleteEditorSprite(EditorSprite* pEditSprite) {
    m_pEditorSprites.removeOne(pEditSprite);
    m_pSelectedEditorSprites.removeOne(pEditSprite);
    m_pScene->removeSpriteFromScene(pEditSprite);
    delete pEditSprite;
}

//! Supprime tous les sprites sélectionnés.
void EditorManager::deleteSelectedEditorSprites() {
    foreach (EditorSprite* pSprite, m_pSelectedEditorSprites) {
            deleteEditorSprite(pSprite);
    }
}