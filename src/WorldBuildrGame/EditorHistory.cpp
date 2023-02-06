//
// Created by blatnoa on 06.02.2023.
//

#include "EditorHistory.h"
#include "EditorSprite.h"

EditorHistory::EditorHistory(EditorManager *editorManager) {
    m_pEditorManager = editorManager;
}

void EditorHistory::addAction(EditorHistory::Action action, QList<EditorSprite*> sprites) {
    State state;
    state.action = action;
    state.sprites = sprites;

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

void EditorHistory::undo() {
    if (m_currentStateIndex > 0) {
        m_currentStateIndex--;
        State state = m_states[m_currentStateIndex];

        switch (state.action) {
            case AddSprite:
                for (EditorSprite* sprite : state.sprites) {
                    m_pEditorManager->removeSprite(sprite);
                }
                break;
            case RemoveSprite:
                for (EditorSprite* sprite : state.sprites) {
                    m_pEditorManager->addSprite(sprite);
                }
                break;
            case MoveSprite:
                for (EditorSprite* sprite : state.sprites) {
                    m_pEditorManager->moveSprite(sprite, sprite.getPreviousPosition());
                }
                break;
            case DuplicateSprite:
                for (EditorSprite* sprite : state.sprites) {
                    m_pEditorManager->removeSprite(sprite);
                }
                break;
            case AddBackground:
                for (EditorSprite* sprite : state.sprites) {
                    m_pEditorManager->removeBackground(sprite);
                }
                break;
            case DuplicateBackground:
                for (EditorSprite* sprite : state.sprites) {
                    m_pEditorManager->removeBackground(sprite);
                }
                break;
            case SelectAll:
                for (EditorSprite* sprite : state.sprites) {
                    m_pEditorManager->deselectSprite(sprite);
                }
                break;
            case DeselectAll:
                for (EditorSprite* sprite : state.sprites) {
                    m_pEditorManager->selectSprite(sprite);
                }
                break;
            case SelectSprite:
                for (EditorSprite* sprite : state.sprites) {
                    m_pEditorManager->deselectSprite(sprite);
                }
                break;
            case DeselectSprite:
                for (EditorSprite* sprite : state.sprites) {
                    m_pEditorManager->selectSprite(sprite);
                }
                break;
        }
    }
}


