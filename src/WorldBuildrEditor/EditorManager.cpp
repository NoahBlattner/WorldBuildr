/**
 * @file EditorManager.cpp
 * @brief Définition de la classe EditorManager.
 * @author Noah Blattner
 * @date Janvier 2023
 */

#include <QFileDialog>
#include <QMessageBox>
#include <utility>
#include "EditorManager.h"
#include "EditorHistory.h"
#include "GameCore.h"
#include "GameScene.h"
#include "EditorSprite.h"
#include "SelectionZone.h"
#include "resources.h"
#include "SaveFileManager.h"
#include "TagsManager.h"

EditorManager::EditorManager(GameCore* core) {
    m_pScene = core->getScene();
    m_editorHistory = new EditorHistory(this);

    // Connecte les signaux d'input aux fonctions de traitement
    connect(core, &GameCore::notifyKeyPressed, this, &EditorManager::onKeyPressed);
    connect(core, &GameCore::notifyKeyReleased, this, &EditorManager::onKeyReleased);
    connect(core, &GameCore::notifyMouseMoved, this, &EditorManager::onMouseMoved);
    connect(core, &GameCore::notifyMouseButtonPressed, this, &EditorManager::onMouseButtonPressed);
    connect(core, &GameCore::notifyMouseButtonReleased, this, &EditorManager::onMouseButtonReleased);
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
                createNewEditorSprite();
            }
            break;
        case Qt::Key_V:
            if (m_isCtrlHeld) {
                // On colle les sprites copiés
                duplicateSelectedEditorSprites();
            }
            break;
        case Qt::Key_Z:
            if (m_isCtrlHeld) {
                // On annule la dernière action
                m_editorHistory->undo();
            }
            break;
        case Qt::Key_Y:
            if (m_isCtrlHeld) {
                // On rétablit la dernière action annulée
                m_editorHistory->redo();
            }
            break;
        case Qt::Key_S:
            if (m_isCtrlHeld) {
                if (m_isShiftHeld) {
                    // On sauvegarde le niveau sous
                    save("");
                } else {
                    // On sauvegarde le niveau
                    save(m_saveFilePath);
                }
            }
            break;
        case Qt::Key_O:
            if (m_isCtrlHeld) {
                // On charge un niveau
                load("");
            }
            break;
        case Qt::Key_I:
            if (m_isCtrlHeld) {
                // On importe un niveau
                import("");
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
                if (mouseDownEditorSprite->getEditSelected()) { // Si le sprite est sélectionné
                    // On fait du drag and drop
                    if (!m_isDragging) { // Si c'est le premier mouvement du drag and drop
                        // On met en pause l'historique
                        m_editorHistory->pauseHistory(2);

                        // On commence le drag and drop
                        m_isDragging = true;
                        m_startDragPosition = oldMousePosition;
                    }

                    if (m_isGridEnabled) { // Si la grille est activée
                        // Activer l'historique
                        m_editorHistory->requestResumeHistory(2);

                        int prevGridX = oldMousePosition.x() / m_gridCellSize;
                        int prevGridY = oldMousePosition.y() / m_gridCellSize;

                        int newGridX = newMousePosition.x() / m_gridCellSize;
                        int newGridY = newMousePosition.y() / m_gridCellSize;

                        if (prevGridX != newGridX) { // Si on a changé de case en X
                            // On déplace le sprite
                            setEditorSpriteX(mouseDownEditorSprite, newGridX * m_gridCellSize);
                        }

                        if (prevGridY != newGridY) { // Si on a changé de case en Y
                            // On déplace le sprite
                            setEditorSpriteY(mouseDownEditorSprite, newGridY * m_gridCellSize);
                        }

                        // On met en pause l'historique
                        m_editorHistory->pauseHistory(2);

                    } else if (m_isSpriteSnappingEnabled) { // Sinon si le snap est activé
                        // Prévoir le déplacement du sprite
                        QRectF spriteRectPredict = mouseDownEditorSprite->sceneBoundingRect();
                        spriteRectPredict.translate(newMousePosition - oldMousePosition);

                        // Si le rect entre en collision avec seulement le sprite lui-même
                        if (m_pScene->collidingSprites(spriteRectPredict).size() <= 1) {
                            // On déplace le sprite
                            moveEditorSprite(mouseDownEditorSprite, newMousePosition - oldMousePosition);
                        }

                    } else {
                        // On déplace le sprite
                        moveSelectedEditorSprites(newMousePosition - oldMousePosition);
                    }
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

            if (m_isDragging) { // Si on a fait un drag and drop
                // On reprend l'historique
                m_editorHistory->requestResumeHistory(2);
                // On enregistre l'action
                QPointF delta = mousePosition - m_startDragPosition;
                m_editorHistory->addSpriteAction(EditorHistory::Action::MoveSprite, m_pSelectedEditorSprites, QString::number(delta.x()) + ";" + QString::number(delta.y()));
                m_isDragging = false;
            } else if (mouseUpSprite != nullptr && mouseUpSprite == mouseDownEditorSprite) { // Si le sprite relâché est le même que le sprite cliqué
                // On sélectionne le sprite
                editorSpriteClicked(mouseUpSprite);
            } else if (m_pMultiSelectionZone != nullptr) { // Sinon si une zone de sélection est en cours
                // On récupère tous les sprites qui sont dans la zone de sélection
                auto sprites = m_pMultiSelectionZone->endSelection();

                if (!sprites.empty()) {
                    m_editorHistory->addSpriteAction(EditorHistory::Action::SelectSprite, sprites);
                    m_editorHistory->pauseHistory(2);

                    // On désélectionne tous les sprites
                    unselectAllEditorSprites();

                    // On sélectionne tous les sprites qui sont dans la zone de sélection
                    selectMultipleEditorSprites(sprites);

                    m_editorHistory->requestResumeHistory(2);
                }

                m_pMultiSelectionZone = nullptr;
            }

            mouseDownEditorSprite = nullptr;
            break;
    }
}

/**********************
 * Gestion de l'éditeur
 *********************/

//! Sauvegarde l'éditeur dans un fichier.
//! \param saveFilePath    Chemin du fichier de sauvegarde.
void EditorManager::save(QString saveFilePath) {
    if (saveFilePath.isEmpty()) { // Si le chemin est vide, demander à l'utilisateur de choisir un fichier
        saveFilePath = QFileDialog::getSaveFileName(nullptr, "Save file", SaveFileManager::DEFAULT_SAVE_DIR, "JSON (*.json)");
    }

    if (saveFilePath.isEmpty()) { // Si le chemin est toujours vide, annuler
        return;
    }

    // Ajouter l'extension .json si elle n'est pas présente
    if (!saveFilePath.endsWith(".json")) {
        saveFilePath += ".json";
    }

    // On retient le chemin du fichier de sauvegarde
    m_saveFilePath = saveFilePath;

    // Sauvegarde le fichier
    SaveFileManager::save(this,std::move(saveFilePath));
}

//! Charge un fichier de sauvegarde dans l'éditeur.
//! \param saveFilePath    Chemin du fichier de sauvegarde.
void EditorManager::load(QString saveFilePath) {
    // Demander à l'utilisateur s'il est sûr de vouloir charger un fichier
    if (QMessageBox::question(nullptr, "Load file", "Are you sure you want to load a file? This will erase the current state of the editor.") == QMessageBox::No) {
        return;
    }

    if (saveFilePath.isEmpty()) { // Si le chemin est vide, demander à l'utilisateur de choisir un fichier
        saveFilePath = QFileDialog::getOpenFileName(nullptr, "Load file", SaveFileManager::DEFAULT_SAVE_DIR, "JSON (*.json)");
    }

    if (saveFilePath.isEmpty()) { // Si le chemin est toujours vide, annuler
        return;
    }

    // Ajouter l'extension .json si elle n'est pas présente
    if (!saveFilePath.endsWith(".json")) {
        saveFilePath += ".json";
    }

    SaveFileManager::load(this,std::move(saveFilePath));
}

//! Importe un fichier de sauvegarde dans l'éditeur.
//! \param saveFilePath    Chemin du fichier de sauvegarde.
void EditorManager::import(QString saveFilePath) {
    if (saveFilePath.isEmpty()) { // Si le chemin est vide, demander à l'utilisateur de choisir un fichier
        saveFilePath = QFileDialog::getOpenFileName(nullptr, "Load file", SaveFileManager::DEFAULT_SAVE_DIR, "JSON (*.json)");
    }

    if (saveFilePath.isEmpty()) { // Si le chemin est toujours vide, annuler
        return;
    }

    // Ajouter l'extension .json si elle n'est pas présente
    if (!saveFilePath.endsWith(".json")) {
        saveFilePath += ".json";
    }

    SaveFileManager::import(this,std::move(saveFilePath));
}

//! Réinitialise l'éditeur. Supprime tous les sprites d'éditeur.
void EditorManager::resetEditor() {
    // Supprimer les tags
    TagsManager::clearTags();

    // Supprime l'image de fond
    removeBackGroundImage();

    // Supprime tous les sprites d'éditeur
    selectAllEditorSprites();
    deleteSelectedEditorSprites();

    // Réinitialise l'historique
    m_editorHistory->clearHistory();
}

//! Indique si un rectangle est dans la scène. Le rectangle est considéré comme dans la scène s'il a au moins un point dans la scène.
//! \param rectF    Rectangle à tester.
//! \return         Vrai si le rectangle est dans la scène, faux sinon.
bool EditorManager::isInScene(QRectF rectF) const {
    return rectF.right() >= 0 && rectF.left() <= m_pScene->width()
        && rectF.bottom() >= 0 && rectF.top() <= m_pScene->height();
}

//! Indique si l'éditeur contient le sprite donné.
//! \param pEditSprite    Sprite d'éditeur à chercher.
bool EditorManager::containsEditorSprite(EditorSprite *pEditSprite) const {
    return m_pEditorSprites.contains(pEditSprite);
}

//! Réinitialise l'historique de l'éditeur
void EditorManager::resetHistory() {
    m_editorHistory->clearHistory();
}

//! Annule la dernière action de l'historique
void EditorManager::undo() {
    m_editorHistory->undo();
}

//! Rétablit la dernière action annulée de l'historique
void EditorManager::redo() {
    m_editorHistory->redo();
}

//! Set la taille des cellules de la grille
//! \param size    Taille des cellules de la grille
void EditorManager::setGridCellSize(int size) {
    m_gridCellSize = size;
}

//! Set si la grille est activée.
//! \param enabled    Si la grille est activée
void EditorManager::setGridEnabled(bool enabled) {
    m_isGridEnabled = enabled;

    if (m_isGridEnabled) { // Si la grille est activée
        // On désactive le snap
        m_isSpriteSnappingEnabled = false;
    }
}

//! Set si le snap est activé.
//! \param enabled    Si le snap est activé
void EditorManager::setSnapEnabled(bool enabled) {
    m_isSpriteSnappingEnabled = enabled;

    if (m_isSpriteSnappingEnabled) { // Si le snap est activé
        // On désactive la grille
        m_isGridEnabled = false;
    }
}

//! Copie une image sélectionnée dans le dossier d'images de l'éditeur
//! et retourne le chemin de la nouvelle image
//! \return Le chemin de l'image6
QString EditorManager::loadImageToEditor() {
    // Ouverture d'une boîte de dialogue pour charger une image
    QString imagePath = QFileDialog::getOpenFileName(this, tr("Load Image"), GameFramework::imagesPath(),
                                                     tr("Image Files (*.png *.jpg *.bmp)"));

    // Si l'utilisateur a annulé, on retourne une chaîne vide
    if (imagePath.isEmpty() || QPixmap(imagePath).isNull()) {
        QMessageBox::warning(this, tr("Error"), tr("Une erreur est survenue lors du chargement de l'image"));
        return QString();
    }

    // Chemin du dossier d'images de l'éditeur
    QString editorImagePath = GameFramework::imagesPath() + "editorImages/";

    // Si le dossier d'image n'existe pas
    if (!QDir(editorImagePath).exists()) {
        // On le crée
        QDir().mkdir(editorImagePath);
    }

    // Chemin de la nouvelle image
    QString newImagePath = editorImagePath + imagePath.split("/").last();
    newImagePath = QDir::toNativeSeparators(newImagePath);

    // Copie de l'image dans le dossier d'images de l'éditeur
    QFile::copy(imagePath, newImagePath);

    return newImagePath;
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

//! Crée une zone de sélection à une position donnée.
//! \param startPositon    Position de la souris.
void EditorManager::createSelectionZone(QPointF startPositon) {
    if (m_pMultiSelectionZone != nullptr) {
        m_pMultiSelectionZone->endSelection();
    }

    m_pMultiSelectionZone = new SelectionZone(m_pScene, startPositon);
}

//! Cherche le z-index le plus élevé parmi tous les sprites d'éditeur.
//! \return Le z-index le plus élevé.
int EditorManager::getHighestZIndex() const {
    if (m_pEditorSprites.isEmpty()) { // Si il n'y a pas de sprite
        return 0;
    }

    // Cherche le sprite avec le z-index le plus élevé
    int maxZIndex = 0;
    for (EditorSprite* pEditSprite : m_pEditorSprites) {
        if (pEditSprite->zValue() > maxZIndex) {
            maxZIndex = pEditSprite->zValue();
        }
    }

    // Retourne le z-index suivant
    return maxZIndex;
}

/********************************************
 * Gestion de le création
 *******************************************/

//! Crée un sprite d'éditeur.
//! \param imageFileName    Nom du fichier image à utiliser pour le sprite.
//! \param position         Position du sprite. Défaut : QPointF(0, 0)
void EditorManager::createNewEditorSprite(QString imageFileName, QPointF position) {
    if (imageFileName.isEmpty()) { // Si le nom du fichier image est vide
        // On demande à l'utilisateur de choisir un fichier image
        imageFileName = loadImageToEditor();
    }


    // On crée le sprite d'éditeur
    auto* pEditorSprite = new EditorSprite(imageFileName);

    // On place le sprite au dessus de tous les autres sprites
    pEditorSprite->setZValue(getHighestZIndex() + 1);

    // On ajoute le sprite à l'éditeur
    addEditorSprite(pEditorSprite, position);
}

//! Ajoute un sprite d'éditeur à l'éditeur.
//! \param pEditorSprite    Pointeur vers le sprite d'éditeur à ajouter.
//! \param position         Position du sprite. Défaut : QPointF(0, 0)
void EditorManager::addEditorSprite(EditorSprite *pEditorSprite, const QPointF &position) {
    // On ajoute le sprite à la liste des sprites d'éditeur
    m_pEditorSprites.append(pEditorSprite);

    if (pEditorSprite->getEditSelected()) { // Si le sprite est sélectionné
        // On ajoute le sprite à la liste des sprites sélectionnés
        m_pSelectedEditorSprites.append(pEditorSprite);
    }

    if (position != QPointF(0, 0)) { // Si la position est différente de 0
        // On place le sprite à la position donnée
        pEditorSprite->setPos(position);
    }

    // On ajoute le sprite à la scène
    m_pScene->addSpriteToScene(pEditorSprite);

    // Connecte le signal de click du sprite à la fonction de traitement du click
    connect(pEditorSprite, &EditorSprite::editorSpriteLeftClicked, this, &EditorManager::editorSpriteClicked);

    // Historique
    m_editorHistory->addSpriteAction(EditorHistory::Action::AddSprite, pEditorSprite);
}

//! Duplique un sprite d'éditeur.
EditorSprite* EditorManager::duplicateEditorSprite(EditorSprite* pEditSprite) {
    // Désactive l'historique pour éviter de créer un historique pour chaque sprite dupliqué
    m_editorHistory->pauseHistory();

    // On crée un nouveau sprite d'éditeur
    auto* duplicatedSprite = pEditSprite->clone();

    // On ajoute le sprite à l'éditeur décalé de 10 pixels
    addEditorSprite(duplicatedSprite, pEditSprite->pos() + QPointF(10, 10));

    // On sélectionne le nouveau sprite
    selectEditorSprite(duplicatedSprite);

    // Historique
    m_editorHistory->requestResumeHistory();
    m_editorHistory->addSpriteAction(EditorHistory::Action::DuplicateSprite, pEditSprite);

    return duplicatedSprite;
}

//! Duplique tous les sprites d'éditeur sélectionnés.
void EditorManager::duplicateSelectedEditorSprites() {
    // Désactive l'historique pour éviter de créer un historique pour chaque sprite dupliqué
    m_editorHistory->pauseHistory(2);

    // Dupliquer la liste des sprites sélectionnés
    auto spritesToDuplicate = m_pSelectedEditorSprites;

    // On désélectionne tous les sprites
    unselectAllEditorSprites();

    QList<EditorSprite*> duplicatedSprites;

    // Pour chaque sprite sélectionné
    for (auto* pEditSprite : spritesToDuplicate) {
        // On le duplique
        duplicatedSprites << duplicateEditorSprite(pEditSprite);
    }

    // Historique
    m_editorHistory->requestResumeHistory(2);
    m_editorHistory->addSpriteAction(EditorHistory::Action::DuplicateSprite, duplicatedSprites);
}

/********************************************
 * Gestion de la sélection
 *******************************************/

//! Sélectionne un sprite d'éditeur.
//! \param pEditSprite    Sprite d'éditeur à sélectionner.
void EditorManager::selectEditorSprite(EditorSprite *pEditSprite) {
    // Ajoute le sprite cliqué à la liste des sprites sélectionnés
    if (!m_pSelectedEditorSprites.contains(pEditSprite)) {
        emit editorSpriteSelected(pEditSprite);
        m_pSelectedEditorSprites.append(pEditSprite);

        // Indique au sprite qu'il est sélectionné
        pEditSprite->setEditSelected(true);

        // Historique
        m_editorHistory->addSpriteAction(EditorHistory::Action::SelectSprite, pEditSprite);
    }
}

//! Sélectionne un sprite d'éditeur en désélectionnant tout les autres.
void EditorManager::selectSingleEditorSprite(EditorSprite *pEditSprite) {
    // Désélectionne tous les sprites
    unselectAllEditorSprites();

    // Sélectionne le sprite
    selectEditorSprite(pEditSprite);

    // Historique
    m_editorHistory->addSpriteAction(EditorHistory::Action::SelectSprite, pEditSprite);
}

//! Désélectionne tous les sprites d'éditeur.
void EditorManager::unselectAllEditorSprites() {
    // Désactive l'historique pour éviter de créer un historique pour chaque sprite désélectionné
    m_editorHistory->pauseHistory();

    // Indique à tous les sprites qu'ils ne sont plus sélectionnés
    for (EditorSprite* pSprite : m_pSelectedEditorSprites) {
        pSprite->setEditSelected(false);
    }

    // Vide la liste des sprites sélectionnés
    m_pSelectedEditorSprites.clear();

    // Historique
    m_editorHistory->requestResumeHistory();
    m_editorHistory->addSpriteAction(EditorHistory::Action::DeselectAll, m_pEditorSprites);
}

//! Change la sélection d'un sprite d'éditeur.
void EditorManager::toggleSelectEditorSprite(EditorSprite* pEditSprite) {
    if (m_pSelectedEditorSprites.contains(pEditSprite)) {
        unselectEditorSprite(pEditSprite);
    } else {
        selectEditorSprite(pEditSprite);
    }
}

//! Sélectionne plusieurs sprites d'éditeur.
void EditorManager::selectMultipleEditorSprites(const QList<EditorSprite*> &pEditSprites) {
    // Desactive l'historique pour éviter de créer un historique pour chaque sprite sélectionné
    m_editorHistory->pauseHistory();

    for (EditorSprite* pSprite : pEditSprites) {
        selectEditorSprite(pSprite);
    }

    // Historique
    m_editorHistory->requestResumeHistory();
    m_editorHistory->addSpriteAction(EditorHistory::Action::SelectSprite, pEditSprites);
}

//! Désélectionne un sprite d'éditeur.
void EditorManager::unselectEditorSprite(EditorSprite* pEditSprite) {
    // Enlève le sprite de la liste des sprites sélectionnés
    m_pSelectedEditorSprites.removeOne(pEditSprite);

    // Indique au sprite qu'il n'est plus sélectionné
    pEditSprite->setEditSelected(false);

    // Historique
    m_editorHistory->addSpriteAction(EditorHistory::Action::DeselectSprite, pEditSprite);
}

//! Sélectionne tous les sprites d'éditeur.
void EditorManager::selectAllEditorSprites() {
    // Désactive l'historique pour éviter de créer un historique pour chaque sprite sélectionné
    m_editorHistory->pauseHistory(2);

    selectMultipleEditorSprites(m_pEditorSprites);

    // Historique
    m_editorHistory->requestResumeHistory(2);
    m_editorHistory->addSpriteAction(EditorHistory::Action::SelectAll, m_pEditorSprites);
}

//! Met à jour la multi-sélection.
void EditorManager::updateMultiSelect(QPointF &newMousePosition) {// On met à jour la zone de
    // Désactive l'historique pour éviter de créer un historique pour chaque sprite sélectionné
    m_editorHistory->pauseHistory(2);

    m_pMultiSelectionZone->updateSelection(newMousePosition);
    selectMultipleEditorSprites(m_pMultiSelectionZone->getCollidingEditorSprites());

    // Historique
    m_editorHistory->requestResumeHistory(2);
}

/********************************************
 * Gestion de suppression
 *******************************************/

//! Supprime un sprite d'éditeur.
void EditorManager::deleteEditorSprite(EditorSprite* pEditSprite) {
    emit editorSpriteDeleted(pEditSprite);

    m_pEditorSprites.removeOne(pEditSprite);
    m_pSelectedEditorSprites.removeOne(pEditSprite);
    m_pScene->removeSpriteFromScene(pEditSprite);

    // Historique
    m_editorHistory->addSpriteAction(EditorHistory::Action::RemoveSprite, pEditSprite);
}

//! Supprime tous les sprites sélectionnés.
void EditorManager::deleteSelectedEditorSprites() {
    // Désactive l'historique pour éviter de créer un historique pour chaque sprite supprimé
    m_editorHistory->addSpriteAction(EditorHistory::Action::RemoveSprite, m_pSelectedEditorSprites);
    m_editorHistory->pauseHistory();

    for (auto* pSprite : m_pSelectedEditorSprites) {
        deleteEditorSprite(pSprite);
    }

    // Historique
    m_editorHistory->requestResumeHistory();
}

/********************************************
 * Gestion de modification de sprites
 *******************************************/

//! Déplace un sprite d'éditeur à une position donnée sur l'axe X.
//! \param pEditSprite    Sprite d'éditeur à déplacer.
//! \param x    Position sur l'axe X.
void EditorManager::setEditorSpriteX(EditorSprite* pEditSprite, qreal x) {
    QRectF currentSpriteBounds = pEditSprite->sceneBoundingRect();
    currentSpriteBounds.translate(x - pEditSprite->x(), 0);

    if (!isInScene(currentSpriteBounds)) { // Si le sprite sort de la scène complétement, on ne le déplace pas
        return;
    }

    m_editorHistory->addSpriteAction(EditorHistory::Action::MoveSprite, pEditSprite, QString::number(x - pEditSprite->x()) + ";0");
    pEditSprite->setX(x);
}

//! Déplace un sprite d'éditeur à une position donnée sur l'axe Y.
//! \param pEditSprite    Sprite d'éditeur à déplacer.
//! \param y    Position sur l'axe Y.
void EditorManager::setEditorSpriteY(EditorSprite* pEditSprite, qreal y) {
    QRectF currentSpriteBounds = pEditSprite->sceneBoundingRect();
    currentSpriteBounds.translate(0, y - pEditSprite->y());

    if (!isInScene(currentSpriteBounds)) { // Si le sprite sort de la scène complétement, on ne le déplace pas
        return;
    }

    m_editorHistory->addSpriteAction(EditorHistory::Action::MoveSprite, pEditSprite, "0;" + QString::number(y - pEditSprite->y()));
    pEditSprite->setY(y);
}

//! Déplace un sprite d'éditeur d'un vecteur donné.
//! \param pEditSprite    Sprite d'éditeur à déplacer.
//! \param moveVector    Vecteur de déplacement.
void EditorManager::moveEditorSprite(EditorSprite *pEditSprite, QPointF moveVector) {
    // Calcule la nouvelle position du sprite
    QRectF newSpriteBounds = pEditSprite->sceneBoundingRect();
    newSpriteBounds.translate(moveVector);

    // Si le sprite sort de la scène complétement, on ne le déplace pas
    if (!isInScene(newSpriteBounds)) {
        return;
    }

    // Déplace le sprite
    pEditSprite->moveBy(moveVector.x(), moveVector.y());

    // Historique
    m_editorHistory->addSpriteAction(EditorHistory::Action::MoveSprite, pEditSprite, QString::number(moveVector.x()) + ";" + QString::number(moveVector.y()));
}

//! Déplace tous les sprites sélectionnés d'un vecteur donné.
//! \param moveVector    Vecteur de déplacement.
void EditorManager::moveSelectedEditorSprites(QPointF moveVector) {
    // Désactive l'historique pour éviter de créer un historique pour chaque sprite déplacé
    m_editorHistory->pauseHistory();

    for (auto *pSprite: m_pSelectedEditorSprites) {
        moveEditorSprite(pSprite, moveVector);
    }

    // Historique
    m_editorHistory->requestResumeHistory();
    m_editorHistory->addSpriteAction(EditorHistory::Action::MoveSprite, m_pSelectedEditorSprites, QString::number(moveVector.x()) + ";" + QString::number(moveVector.y()));
}

//! Tourne un sprite d'éditeur d'un angle donné.
//! \param pEditSprite    Sprite d'éditeur à tourner.
//! \param angle    Angle de rotation à appliquer.
void EditorManager::setEditorSpriteRotation(EditorSprite *pEditSprite, qreal angle) {
    pEditSprite->setRotation(angle);

    // Historique
    m_editorHistory->addSpriteAction(EditorHistory::Action::RotateSprite, pEditSprite, QString::number(angle));
}

//! Change l'index de profondeur d'un sprite d'éditeur.
//! \param pEditSprite    Sprite d'éditeur à modifier.
//! \param zIndex    Nouvel index de profondeur.
void EditorManager::setEditorSpriteZIndex(EditorSprite* pEditSprite, int zIndex) {
    // Historique
    m_editorHistory->addSpriteAction(EditorHistory::Action::ChangeZIndex, pEditSprite, QString::number(zIndex - pEditSprite->zValue()));

    pEditSprite->setZValue(zIndex);
}

//! Change la taille d'un sprite d'éditeur.
//! \param pEditSprite    Sprite d'éditeur à redimensionner.
//! \param xScale    Facteur d'agrandissement sur l'axe X.
//! \param yScale    Facteur d'agrandissement sur l'axe Y.
void EditorManager::rescaleEditorSprite(EditorSprite *pEditSprite, double scale) {
    // Historique
    // Les données additionnelles est la différence du scale
    m_editorHistory->addSpriteAction(EditorHistory::Action::RescaleSprite, pEditSprite, QString::number(scale - pEditSprite->scale()));

    pEditSprite->setScale(scale);
}

//! Change la transparence d'un sprite d'éditeur.
//! \param pEditSprite    Sprite d'éditeur à modifier.
void EditorManager::setEditorSpriteOpacity(EditorSprite* pEditSprite, double opacity) {
    // Historique
    m_editorHistory->addSpriteAction(EditorHistory::Action::ChangeOpacity, pEditSprite, QString::number(opacity - pEditSprite->opacity()));

    pEditSprite->setOpacity(opacity);
}

/********************************************
 * Gestion de l'image de fond
 *******************************************/

//! Set l'image de fond de l'éditeur.
//! \param imageFileName    Chemin de l'image de fond. Si vide, on demande à l'utilisateur de choisir une image.
void EditorManager::setBackGroundImage(QString imageFileName) {
    if (!QFile::exists(imageFileName)) { // Si le fichier n'existe pas
        // On demande à l'utilisateur de choisir une image
        imageFileName = loadImageToEditor();

        if (imageFileName.isEmpty()) { // Si l'utilisateur a annulé
            // On quitte la fonction
            return;
        }
    }

    // Création de l'image
    QImage image(imageFileName);

    // Redimensionnement à la taille de la scène
    image = image.scaled(m_pScene->width(), m_pScene->height());

    // On charge l'image de fond dans la scène
    m_pScene->setBackgroundImage(image);

    // On sauvegarde le chemin de l'image
    m_backgroundImageFileName = imageFileName;

    // Historique
    m_editorHistory->addAction(EditorHistory::Action::AddBackground, m_backgroundImageFileName);
}

//! Supprime l'image de fond de l'éditeur.
void EditorManager::removeBackGroundImage() {
    // Historique
    m_editorHistory->addAction(EditorHistory::Action::RemoveBackground, m_backgroundImageFileName);
    m_backgroundImageFileName = QString();

    // On charge une image vide dans la scène
    m_pScene->setBackgroundColor(Qt::black);
}
