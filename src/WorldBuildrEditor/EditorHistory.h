//
// Created by blatnoa on 06.02.2023.
//

#ifndef WORLDBUILDR_EDITORHISTORY_H
#define WORLDBUILDR_EDITORHISTORY_H

#include <QList>

class EditorManager;
class EditorSprite;

//! Classe permettant de gérer l'historique des actions de l'éditeur
//! Cette classe peut être utilisée pour annuler et rétablir des actions d'un éditeur
//!
//! Pour utiliser cette classe, il faut pass
//! La méthode addAction() doit être appelée à chaque fois qu'une action est effectuée
//! La méthode undo() permet d'annuler la dernière action
//! La méthode redo() permet de rétablir la dernière action annulée
//! La méthode clearHistory() permet de vider l'historique
//!
//! Il est parfois nécessaire de mettre en pause l'historique, notamment lorsqu'on effectue une action s'appliquant à plusieurs sprites
//! Dans ce cas, on ne veut pas sauvegarder une action par sprite, mais une seule action pour tous les sprites concernés
//! Pour cela, on appelle la méthode pauseHistory() avant d'effectuer l'action, et la méthode requestResumeHistory() après l'action
//! Si on fait face à plus de 1 niveau de pause, on peut utiliser la méthode pauseHistory(int level) et requestResumeHistory(int level)
//! Le niveau de pause permet de mettre un pause l'historique à un niveau donné. Pour le réactiver, un niveau de pause égal ou supérieur est nécessaire
//! La méthode pauseHistory() permet de mettre en pause l'historique
//! La méthode requestResumeHistory() permet de demander la reprise de l'historique
class EditorHistory {
public:
    explicit EditorHistory(EditorManager* editorManager);
    ~EditorHistory();

    enum Action {
        AddSprite,
        RemoveSprite,
        MoveSprite,
        DuplicateSprite,
        SelectAll,
        DeselectAll,
        SelectSprite,
        DeselectSprite,
        AddBackground,
        RemoveBackground
    };

    void addAction(Action action, QString additionalData);
    void addSpriteAction(Action action, QList<EditorSprite*> sprites);
    void addSpriteAction(Action action, EditorSprite* sprite);
    void addSpriteAction(Action action, QList<EditorSprite*> sprites, QString additionalData);
    void addSpriteAction(Action action, EditorSprite* sprite, QString additionalData);

    void undo();
    void redo();

    void clearHistory();

    void pauseHistory(int level = 1);
    void requestResumeHistory(int level = 1);

private:
    int const MAX_HISTORY_SIZE = 100;
    int const MAX_PAUSE_LEVEL = 100;

    bool m_paused = false;

    int m_pauseLevel = 1;

    EditorManager* m_pEditorManager = nullptr;

    struct State {
        Action action;
        QList<EditorSprite*> sprites;
        QString additionalData;
    };

    QList<State> m_states;
    int m_currentStateIndex = -1;

    void deleteUnreferencedSpritesInState(int i);

    void performAction(State &state);
    void performInverseAction(State &state);
};


#endif //WORLDBUILDR_EDITORHISTORY_H
