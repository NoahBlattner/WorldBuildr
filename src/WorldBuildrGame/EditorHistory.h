//
// Created by blatnoa on 06.02.2023.
//

#ifndef WORLDBUILDR_EDITORHISTORY_H
#define WORLDBUILDR_EDITORHISTORY_H

#include <QList>

class EditorManager;
class EditorSprite;

//! Classe permettant de gérer l'historique des actions de l'éditeur
//! Cette classe peut être utilisée pour annuler et rétablir des actions
//!
//! La méthode addState() doit être appelée à chaque fois qu'une action est effectuée
//! La méthode undo() permet d'annuler la dernière action
//! La méthode redo() permet de rétablir la dernière action annulée
class EditorHistory {
public:
    explicit EditorHistory(EditorManager* editorManager);
    ~EditorHistory();

    enum Action {
        AddSprite,
        RemoveSprite,
        MoveSprite,
        DuplicateSprite,
        AddBackground,
        SelectAll,
        DeselectAll,
        SelectSprite,
        DeselectSprite
    };

    void addState(Action action, QList<EditorSprite*> sprites);
    void addState(Action action, EditorSprite* sprite);
    void addState(Action action, QList<EditorSprite*> sprites, QString additionalData);
    void addState(Action action, EditorSprite* sprite, QString additionalData);

    void undo();
    void redo();

    void removeState(int index);
    void removeLastState();
    void clearHistory();

private:
    int const MAX_HISTORY_SIZE = 100;

    EditorManager* m_pEditorManager = nullptr;

    struct State {
        Action action;
        QList<EditorSprite*> sprites;
        QString additionalData;
    };

    QList<State> m_states;
    int m_currentStateIndex = 0;

};


#endif //WORLDBUILDR_EDITORHISTORY_H
