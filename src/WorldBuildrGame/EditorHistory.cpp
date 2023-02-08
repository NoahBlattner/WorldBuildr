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

    // On supprime les états suivants (si CTRL+Z -> Action -> CTRL+Y n'est plus possible)
    if (m_currentStateIndex < m_states.size() - 1) {
        for (int i = m_states.size()-1; i > m_currentStateIndex; i--) {
            // On supprime les sprites non référencés
            deleteUnreferencedSpritesInState(i);
            m_states.removeLast();
        }
    }

    // On ajoute l'état
    m_states.append(state);
    m_currentStateIndex = m_states.size()-1;

    // On supprime les états en trop
    if (m_states.size() > MAX_HISTORY_SIZE) {
        for (int i = 0; i < m_states.size() - MAX_HISTORY_SIZE; i++) {
            // On supprime les sprites non référencés
            deleteUnreferencedSpritesInState(i);
            m_states.removeFirst();
        }
    }

    QString actionString;

    for (State state1 : m_states) {
        actionString += QString::number(state1.action) + " ";
    }

    qDebug() << "actions " << actionString;
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

//! Supprime les sprites non référencés dans l'état ou dans l'éditeur
void EditorHistory::deleteUnreferencedSpritesInState(int stateIndex) {
    for (EditorSprite* sprite : m_states[stateIndex].sprites) { // Pour chaque sprite de l'état
        bool deleteSprite = true;
        if (m_pEditorManager->containsEditorSprite(sprite)) { // Si le sprite est référencé dans l'éditeur
            // On ne le supprime pas
            deleteSprite = false;
        } else {
            for (const State& state : m_states) { // On regarde s'il est référencé dans un autre état
                if (state.sprites.contains(sprite)) { // S'il est référencé
                    // On le supprime aps
                    deleteSprite = false;
                    break;
                }
            }
        }

        if (deleteSprite) {
            delete sprite;
        }
    }
}

//! Supprime tout l'historique
void EditorHistory::clearHistory() {
    for (int i = m_states.size()-1; i >= 0; i--) {
        deleteUnreferencedSpritesInState(i);
        m_states.removeLast();
    }
    m_currentStateIndex = -1;
}

//! Annule la dernière action effectuée
void EditorHistory::undo() {
    if (m_currentStateIndex > 0) {
        State state = m_states[m_currentStateIndex];

        // On effectue l'action inverse
        performInverseAction(state);

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

//! Effectue l'action inverse de l'état passé en paramètre
//! \param state L'état dont on veut effectuer l'action inverse
void EditorHistory::performInverseAction(EditorHistory::State &state) {
    // On met en pause l'historique
    pauseHistory(MAX_PAUSE_LEVEL);

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
                // On récupère le vecteur de déplacement depuis les données additionnelles
                QList<QString> data = state.additionalData.split(";");
                QPointF moveVector(data[0].toDouble(), data[1].toDouble());
                m_pEditorManager->moveEditorSprite(sprite, -moveVector);
            }
            break;
        case AddBackground:
            m_pEditorManager->removeBackGroundImage();
            break;
        case RemoveBackground:
            m_pEditorManager->setBackGroundImage(state.additionalData);
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

    // On réactive l'historique
    requestResumeHistory(MAX_PAUSE_LEVEL);
}

//! Effectue l'action de l'état passé en paramètre
//! \param state L'état dont on veut effectuer l'action
void EditorHistory::performAction(EditorHistory::State &state) {
    // On met en pause l'historique
    pauseHistory(MAX_PAUSE_LEVEL);

    switch (state.action) {
        case AddSprite:
        case DuplicateSprite:
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
                QList<QString> data = state.additionalData.split(";");
                QPointF moveVector(data[0].toDouble(), data[1].toDouble());
                m_pEditorManager->moveEditorSprite(sprite, moveVector);
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
