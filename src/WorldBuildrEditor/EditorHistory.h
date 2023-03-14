/**
 * @file EditorHistory.h
 * @brief Déclaration de la classe EditorHistory.
 * @author Noah Blattner
 * @date Février 2023
 */

#ifndef WORLDBUILDR_EDITORHISTORY_H
#define WORLDBUILDR_EDITORHISTORY_H

#include <QList>

class EditorManager;
class EditorSprite;

//! Classe permettant de gérer l'historique des actions de l'éditeur
//! Cette classe peut être utilisée pour annuler et rétablir des actions d'un éditeur
//!
//! Pour utiliser cette classe, il faut passer les actions de l'éditeur à la méthode addSpriteAction()
//! Cette méthode permet de sauvegarder l'action effectuée, ainsi que les données nécessaires à son annulation et rétablissement
//! Ces données sont : l'action effectué, les sprites concernés par l'action, ainsi que des données supplémentaires en format QString
//! Il est possible d'omettre les 2 paramètres derniers, mais il ne faut faire cela que si l'action ne nécessite pas de données supplémentaires
//! Par exemple, l'action AddBackground ne nécessite pas de sprite, et l'action SelectAll ne nécessite pas de données supplémentaires
//! Lorsque aucun sprite n'est passé en paramètre, la méthode addAction() peut être utilisée pour simplifier l'appel
//!
//! Pour annuler une action, il faut appeler la méthode undo()
//! Pour rétablir une action précédemment annulé, il faut appeler la méthode redo()
//! Pour vider l'historique, il faut appeler la méthode clearHistory()
//! Ceci est utile lorsque on initialise un nouveau projet, ou lorsque l'on ouvre un projet existant
//!
//! Il est parfois nécessaire de mettre en pause l'historique, notamment lorsqu'on effectue une action s'appliquant à plusieurs sprites
//! Dans ce cas, on ne veut pas sauvegarder une action par sprite, mais une seule action pour tous les sprites concernés
//! Pour cela, on appelle la méthode pauseHistory() avant d'effectuer l'action, et la méthode requestResumeHistory() après l'action
//! Si on fait face à plus de 1 niveau de pause, on peut utiliser la méthode pauseHistory(int level) et requestResumeHistory(int level)
//! Le niveau de pause permet de mettre un pause l'historique à un niveau donné. Pour le réactiver, un niveau de pause égal ou supérieur est nécessaire
//! La méthode pauseHistory(int level = 1) permet de mettre en pause l'historique
//! La méthode requestResumeHistory(int level = 1) permet de demander la reprise de l'historique
//!
//! L'historique sauvegarde les actions dans une liste. Cette liste est limitée à MAX_HISTORY_SIZE actions
//! Lorsqu'on annule une action, et qu'on effectue une nouvelle action, toutes les actions suivantes sont supprimées
//! Par exemple, si on effectue la séquence suivante : "Action -> Action 2 -> Undo -> Action 3", l'historique sera : Action -> Action 3
//!
//! L'historique gardera une référence sur les sprites concernés par les actions pour pouvoir les annuler et les rétablir
//! L'historique se chargera de supprimer les sprites qui ne sont plus référencés par aucune action et ne se trouvent plus dans l'éditeur
class EditorHistory {
public:
    explicit EditorHistory(EditorManager* editorManager);
    ~EditorHistory();

    enum Action {
        AddSprite,
        RemoveSprite,
        MoveSprite,
        ChangeZIndex,
        RotateSprite,
        ChangeOpacity,
        DuplicateSprite,
        RescaleSprite,
        SelectAll,
        DeselectAll,
        SelectSprite,
        DeselectSprite,
        AddBackground,
        RemoveBackground
    };

    static Action inverseAction(Action action);

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

    void deleteAction(int i);
    void deleteActionsFrom(int index);
    void deleteActionsTo(int index);

    void deleteActionAndUnreferencedSprites(int i);

    void performAction(State &state, bool inverse = false);
};


#endif //WORLDBUILDR_EDITORHISTORY_H
