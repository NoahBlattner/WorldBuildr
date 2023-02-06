//
// Created by blatnoa on 06.02.2023.
//

#include "EditorHistory.h"

#include <utility>
#include "EditorManager.h"
#include "EditorSprite.h"

EditorHistory::EditorHistory(EditorManager *editorManager) {
    m_pEditorManager = editorManager;
}

//! Ajoute un état à l'historique
//! \param action L'action effectuée
//! \param sprites Les sprites concernées
void EditorHistory::addState(EditorHistory::Action action, QList<EditorSprite*> sprites) {
    addState(action, std::move(sprites), "");
}

//! Ajoute un état à l'historique qui contient une seule sprite
//! \param action L'action effectuée
//! \param sprite La sprite concernée
void EditorHistory::addState(EditorHistory::Action action, EditorSprite* sprite) {
    QList<EditorSprite*> sprites;
    sprites.append(sprite);
    addState(action, sprites);
}

//! Ajoute un état à l'historique concernant un seul sprite avec des données supplémentaires
//! \param action L'action effectuée
//! \param sprite La sprite concernée
//! \param additionalData Les données supplémentaires
void EditorHistory::addState(EditorHistory::Action action, EditorSprite *sprite, QString additionalData) {
    QList<EditorSprite*> sprites;
    sprites.append(sprite);
    addState(action, sprites, std::move(additionalData));
}

//! Ajoute un état à l'historique avec des données supplémentaires
//! \param action L'action effectuée
//! \param sprites Les sprites concernées
//! \param additionalData Les données supplémentaires
void EditorHistory::addState(EditorHistory::Action action, QList<EditorSprite *> sprites, QString additionalData) {
    State state;
    state.action = action;
    state.sprites = std::move(sprites);
    state.additionalData = std::move(additionalData);

    // On avance dans l'historique
    m_currentStateIndex++;

    // On supprime les états suivants
    if (m_currentStateIndex < m_states.size()) {
        m_states.erase(m_states.begin() + m_currentStateIndex, m_states.end());
    }

    // On ajoute l'état
    m_states.append(state);

    // On supprime les états en trop
    if (m_states.size() > MAX_HISTORY_SIZE) {
        m_states.erase(m_states.begin(), m_states.begin() + m_states.size() - MAX_HISTORY_SIZE);
    }
}

//! Rétablit la dernière action annulée
void EditorHistory::undo() {
    if (m_currentStateIndex > 0) {
        m_currentStateIndex--;
        State state = m_states[m_currentStateIndex];

        switch (state.action) {
            case AddSprite:
            case DuplicateSprite:
                for (EditorSprite* sprite : state.sprites) {
                    m_pEditorManager->deleteEditorSprite(sprite);
                }
                break;
            case RemoveSprite:
                for (EditorSprite* sprite : state.sprites) {
                    m_pEditorManager->addEditorSprite(sprite);
                }
                break;
            case MoveSprite:
                for (EditorSprite* sprite : state.sprites) {
                    QList<QString> data = state.additionalData.split(";");
                    QPointF moveVector(data[0].toDouble(), data[1].toDouble());
                    m_pEditorManager->moveEditorSprite(sprite, moveVector);
                }
                break;
            case AddBackground:
                for (EditorSprite* sprite : state.sprites) {
                    m_pEditorManager->removeBackGroundImage();
                }
                break;
            case SelectAll:
                m_pEditorManager->unselectAllEditorSprites();
                break;
            case DeselectAll:
                m_pEditorManager->selectAllEditorSprites();
                break;
            case SelectSprite:
                for (EditorSprite* sprite : state.sprites) {
                    m_pEditorManager->unselectEditorSprite(sprite);
                }
                break;
            case DeselectSprite:
                for (EditorSprite* sprite : state.sprites) {
                    m_pEditorManager->selectEditorSprite(sprite);
                }
                break;
        }
    }
}

//! Supprime un état de l'historique
void EditorHistory::removeState(int index) {
    // On supprime l'état
    m_states.removeAt(index);

    if (m_currentStateIndex > index) { // Si l'état supprimé est avant l'état courant
        m_currentStateIndex--;
    }
}

//! Supprime le dernier état de l'historique
void EditorHistory::removeLastState() {
    removeState(m_states.size() - 1);
}

void EditorHistory::redo() {
    // TODO
}
