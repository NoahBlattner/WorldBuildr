/*
 * @file EditorHistory.cpp
 * @brief Définition de la classe EditorHistory.
 * @author Noah Blattner
 * @date Février 2023
 */

#include "EditorHistory.h"

#include <utility>
#include "EditorManager.h"
#include "EditorSprite.h"

EditorHistory::EditorHistory(EditorManager *editorManager) {
    m_pEditorManager = editorManager;
}

EditorHistory::~EditorHistory() {
    clearHistory();
}

//! Ajoute une action simple à l'historique
//! \param action L'action effectuée
//! \param additionalData Les données supplémentaires
void EditorHistory::addAction(EditorHistory::Action action, QString additionalData) {
    QList<EditorSprite*> sprites;
    addSpriteAction(action, sprites, std::move(additionalData));
}

//! Ajoute un état à l'historique qui contient une seule sprite
//! \param action L'action effectuée
//! \param sprite La sprite concernée
void EditorHistory::addSpriteAction(EditorHistory::Action action, EditorSprite* sprite) {
    QList<EditorSprite*> sprites;
    sprites.append(sprite);
    addSpriteAction(action, sprites);
}

//! Ajoute un état à l'historique
//! \param action L'action effectuée
//! \param sprites Les sprites concernées
void EditorHistory::addSpriteAction(EditorHistory::Action action, QList<EditorSprite*> sprites) {
    addSpriteAction(action, std::move(sprites), "");
}

//! Ajoute un état à l'historique concernant un seul sprite avec des données supplémentaires
//! \param action L'action effectuée
//! \param sprite La sprite concernée
//! \param additionalData Les données supplémentaires
void EditorHistory::addSpriteAction(EditorHistory::Action action, EditorSprite *sprite, QString additionalData) {
    QList<EditorSprite*> sprites;
    sprites.append(sprite);
    addSpriteAction(action, sprites, std::move(additionalData));
}

//! Ajoute un état à l'historique avec des données supplémentaires
//! \param action L'action effectuée
//! \param sprites Les sprites concernées
//! \param additionalData Les données supplémentaires
void EditorHistory::addSpriteAction(EditorHistory::Action action, QList<EditorSprite *> sprites, QString additionalData) {
    if (m_paused) { // Si l'historique est en pause, on ne fait rien
        return;
    }

    State state;
    state.action = action;
    state.sprites = std::move(sprites);
    state.additionalData = std::move(additionalData);

    if (m_currentStateIndex < m_states.size() - 1) { // Si CTRL+Z -> Action
        // On supprime les états après l'état ajouté
        deleteActionsFrom(m_currentStateIndex + 1);
    }

    // On ajoute l'état
    m_states.append(state);

    // On supprime les états en trop
    if (m_states.size() > MAX_HISTORY_SIZE) {
        // On supprime les états en trop
        deleteActionsTo(m_states.size() - MAX_HISTORY_SIZE);
    }

    // On met à jour l'index de l'état courant
    m_currentStateIndex = m_states.size()-1;
}

//! Mettre en pause l'historique pour qu'il n'accepte plus d'état
//! Un niveau de pause est associé à chaque pause
//! \param level Le niveau de pause
void EditorHistory::pauseHistory(int level) {
    if (level > MAX_PAUSE_LEVEL) {
        level = MAX_PAUSE_LEVEL;
    }
    m_paused = true;

    if (level > m_pauseLevel) {
        m_pauseLevel = level;
    }
}

//! Réactiver l'historique pour qu'il accepte à nouveau des états
//! Ne réactive l'historique que si le niveau de pause est inférieur ou égal au niveau passé en paramètre
//! \param level Le niveau de pause à réactiver
void EditorHistory::requestResumeHistory(int level) {
    if (level >= m_pauseLevel) {
        m_pauseLevel = 1;
        m_paused = false;
    }
}

//! Supprime les sprites non référencés dans l'historique autre que dans l'état passé en paramètre
//! \param stateIndex L'index de l'état dont on veut supprimer les sprites non référencés
void EditorHistory::deleteActionAndUnreferencedSprites(int stateIndex) {
    if (stateIndex < 0 || stateIndex >= m_states.size()) { // Si l'état n'existe pas
        return;
    }

    // On récupère les spritesToCheckForDelete de l'état
    auto spritesToCheckForDelete = m_states[stateIndex].sprites;

    // On supprime l'action de l'historique
    m_states.removeAt(stateIndex);

    for (EditorSprite* currentSprite : spritesToCheckForDelete) { // Pour chaque sprite de l'état supprimé
        bool deleteSprite = true;
        if (m_pEditorManager->containsEditorSprite(currentSprite)) { // Si le sprite est référencé dans l'éditeur
            // On ne le supprime pas
            deleteSprite = false;
        } else {
            // On regarde s'il est référencé dans une autre action de l'historique
            for (const State& state : m_states) { // Pour chaque action de l'historique
                if (state.sprites.contains(currentSprite)) {
                    // On le supprime pas
                    deleteSprite = false;
                    break;
                }
            }
        }

        if (deleteSprite) { // Si le sprite n'est pas référencé dans l'éditeur et dans l'historique
            delete currentSprite;
        }
    }
}

//! Supprime les états suivants l'état à l'index donné (Incluant cet état)
//! \param index L'index de l'état à partir duquel on supprime
void EditorHistory::deleteActionsFrom(int index) {
    for (int i = index; i < m_states.size(); i++) {
        deleteActionAndUnreferencedSprites(i);
    }
}

//! Supprime les états précédents l'état à l'index donné (Incluant cet état)
//! \param index L'index de l'état à jusqu'auquel on supprime
void EditorHistory::deleteActionsTo(int index) {
    for (int i = index; i >= 0; i--) {
        deleteActionAndUnreferencedSprites(i);
    }
}

//! Supprime tout l'historique
void EditorHistory::clearHistory() {
    // Supprime tout l'historique
    deleteActionsTo(m_states.size()-1);
    m_currentStateIndex = -1;
}

//! Annule la dernière action effectuée
void EditorHistory::undo() {
    if (m_currentStateIndex > 0) {
        State state = m_states[m_currentStateIndex];

        // On effectue l'action inverse
        performAction(state, true);

        // On décrémente l'index de l'état courant
        m_currentStateIndex--;
    }
}

//! Rétablit la dernière action annulée
void EditorHistory::redo() {
    if (m_currentStateIndex < m_states.size()-1) {
        m_currentStateIndex++;
        State state = m_states[m_currentStateIndex];

        // On effectue l'action inverse
        performAction(state);
    }
}

//! Effectue l'action de l'état passé en paramètre
//! \param state L'état dont on veut effectuer l'action
void EditorHistory::performAction(EditorHistory::State &state, bool inverse) {
    // On met en pause l'historique
    pauseHistory(MAX_PAUSE_LEVEL);

    if (inverse) { // Si on veut effectuer l'action inverse
        // On inverse l'action
        state.action = inverseAction(state.action);
    }

    switch (state.action) {
        case AddSprite:
        case DuplicateSprite: // On fait la même chose pour les deux actions
            for (EditorSprite* sprite : state.sprites) {
                m_pEditorManager->addEditorSprite(sprite);
            }
            break;
        case RemoveSprite:
            for (EditorSprite* sprite : state.sprites) {
                m_pEditorManager->deleteEditorSprite(sprite);
            }
            break;
        case MoveSprite:
            for (EditorSprite* sprite : state.sprites) {
                // On récupère le vecteur de déplacement depuis les données additionnelles
                QList<QString> data = state.additionalData.split(";");
                QPointF moveVector(data[0].toDouble(), data[1].toDouble());
                // Si on veut effectuer l'action inverse, on inverse le vecteur de déplacement
                m_pEditorManager->moveEditorSprite(sprite, (inverse) ? -moveVector : moveVector);
            }
            break;
        case ChangeZIndex:
            for (EditorSprite* sprite : state.sprites) {
                // On récupère le nouveau z-index depuis les données additionnelles
                int newZIndex = state.additionalData.toInt() * (inverse ? -1 : 1) + sprite->zValue();
                // Si on veut effectuer l'action inverse, on inverse le z-index
                m_pEditorManager->setEditorSpriteZIndex(sprite, newZIndex);
            }
            break;
        case RotateSprite:
            for (EditorSprite* sprite : state.sprites) {
                // On récupère l'angle de rotation depuis les données additionnelles
                qreal newAngle = state.additionalData.toDouble() * (inverse ? -1 : 1) + sprite->rotation();
                // Si on veut effectuer l'action inverse, on inverse l'angle de rotation
                m_pEditorManager->setEditorSpriteRotation(sprite, newAngle);
            }
            break;
        case RescaleSprite:
            for (EditorSprite* sprite : state.sprites) {
                // On récupère le facteur d'échelle depuis les données additionnelles
                qreal scale = state.additionalData.toDouble();
                // Si on veut effectuer l'action inverse, on inverse le facteur d'échelle
                double newScale = state.additionalData.toDouble() * ((inverse) ? -1 : 1) + sprite->scale();
                m_pEditorManager->rescaleEditorSprite(sprite, newScale);
            }
            break;
        case ChangeOpacity:
            for (EditorSprite* sprite : state.sprites) {
                // On récupère le nouveau niveau d'opacité depuis les données additionnelles
                qreal newOpacity = state.additionalData.toDouble() * (inverse ? -1 : 1) + sprite->opacity();
                // Si on veut effectuer l'action inverse, on inverse le niveau d'opacité
                 m_pEditorManager->setEditorSpriteOpacity(sprite, newOpacity);
            }
            break;
        case AddBackground:
            m_pEditorManager->setBackGroundImage(state.additionalData);
            break;
        case RemoveBackground:
            m_pEditorManager->removeBackGroundImage();
            break;
        case SelectAll:
            m_pEditorManager->selectAllEditorSprites();
            break;
        case DeselectAll:
            m_pEditorManager->unselectAllEditorSprites();
            break;
        case SelectSprite:
            for (EditorSprite* sprite : state.sprites) {
                m_pEditorManager->selectEditorSprite(sprite);
            }
            break;
        case DeselectSprite:
            for (EditorSprite* sprite : state.sprites) {
                m_pEditorManager->unselectEditorSprite(sprite);
            }
            break;
    }

    // On réactive l'historique
    requestResumeHistory(MAX_PAUSE_LEVEL);
}

//! Retourne l'action inverse de l'action passée en paramètre
//! Dans certains cas, l'action inverse est la même que l'action (move, rotate)
//! \param action L'action dont on veut l'action inverse
EditorHistory::Action EditorHistory::inverseAction(EditorHistory::Action action) {
    switch (action) {
        case AddSprite:
            return RemoveSprite;
        case RemoveSprite:
            return AddSprite;
        case MoveSprite:
            return MoveSprite;
        case ChangeZIndex:
            return ChangeZIndex;
        case RotateSprite:
            return RotateSprite;
        case RescaleSprite:
            return RescaleSprite;
        case ChangeOpacity:
            return ChangeOpacity;
        case AddBackground:
            return RemoveBackground;
        case RemoveBackground:
            return AddBackground;
        case SelectAll:
            return DeselectAll;
        case DeselectAll:
            return SelectAll;
        case SelectSprite:
            return DeselectSprite;
        case DeselectSprite:
            return SelectSprite;
        case DuplicateSprite:
            return RemoveSprite;
    }

    // On ne devrait jamais arriver ici
    return AddSprite;
}
