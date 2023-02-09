/**
 * @file EditorManager.h
 * @brief Définition de la classe EditorManager.
 * @author Noah Blattner
 * @date Janvier 2023
 */

#ifndef WORLDBUILDR_EDITORMANAGER_H
#define WORLDBUILDR_EDITORMANAGER_H

#include <QWidget>
#include <QPointF>

class EditorSprite;
class EditorHistory;
class SelectionZone;
class GameCore;
class GameScene;

//! Classe gérant l'éditeur de niveau.
//! Elle gère la création, la suppression, la sélection et le déplacement des sprites.
//! Elle est également liée à l'historique des actions.
//! Elle gère également la création de la zone de sélection.
//! Elle écoute les évènements de clavier et de souris pour effectuer les actions correspondantes. Elle mets aussi à disposition des raccourcis clavier pour effectuer les actions.
//!
//! Les méthode de gestion des sprites sont :
//! La méthode createNewEditorSprite() permet de créer un nouveau sprite
//! La méthode addEditorSprite() permet d'ajouter un sprite existant
//! La méthode duplicateEditorSprite() permet de dupliquer un sprite
//! La méthode duplicateSelectedEditorSprites() permet de dupliquer les sprites sélectionnés
//! La méthode deleteEditorSprite() permet de supprimer un sprite
//! La méthode deleteSelectedEditorSprites() permet de supprimer les sprites sélectionnés
//! La méthode selectEditorSprite() permet de sélectionner un sprite
//! La méthode selectSingleEditorSprite() permet de sélectionner un seul sprite (dé-selection tous les autres)
//! La méthode selectAllEditorSprites() permet de sélectionner tous les sprites
//! La méthode toggleSelectEditorSprite() permet de basculer la sélection d'un sprite
//! La méthode selectMultipleEditorSprites() permet de sélectionner plusieurs sprites
//! La méthode unselectEditorSprite() permet de dé-sélectionner un sprite
//! La méthode unselectAllEditorSprites() permet de dé-sélectionner tous les sprites
//! La méthode moveEditorSprite() permet de déplacer un sprite
//!
//! Les méthodes de gestion de l'arrière-plan sont :
//! La méthode setBackGroundImage() permet de définir l'image de fond de la scène
//! La méthode removeBackGroundImage() permet de supprimer l'image de fond de la scène
//!
//! Les méthodes de gestion de l'historique sont :
//! La méthode undo() permet d'annuler la dernière action
//! La méthode redo() permet de refaire la dernière action annulée
//! La méthode resetHistory() permet de réinitialiser l'historique
//! Pour l'historique, il faut parfois utiliser les méthodes de pause et de reprise de l'historique.
//! Cette approche à été préférée à un approche où l'on passe un booléen en paramètre à chaque méthode d'action de sprite
//! Pour plus d'information, voir la classe EditorHistory
//!
//! Les méthodes utilitaires sont :
//! La méthode containsEditorSprite() permet de savoir si un sprite est géré par l'éditeur
//! La méthode createSelectionZone() permet de créer la zone de multi-sélection
//! La méthode loadImageToEditor() permet de charger une image dans l'éditeur (copie dans le dossier de l'éditeur).
//! Elle retourne le chemin de l'image dans l'éditeur.
class EditorManager : public QWidget {
    Q_OBJECT

public:
    explicit EditorManager(GameCore* core);

    void resetEditor();

    // Gestion des sprites
    QList<EditorSprite*> getEditorSprites() const { return m_pEditorSprites; }
    bool containsEditorSprite(EditorSprite* pEditSprite) const;

    // Gestion de l'historique
    void undo();
    void redo();
    void resetHistory();

    // Gestion de création
    void createSelectionZone(QPointF startPositon);
    void createNewEditorSprite(QString imageFileName = QString(), QPointF position = QPointF(0, 0));
    void addEditorSprite(EditorSprite *pEditorSprite, const QPointF &position = QPointF(0, 0));
    EditorSprite* duplicateEditorSprite(EditorSprite* pEditSprite);
    void duplicateSelectedEditorSprites();

    // Gestion de la sélection
    void selectEditorSprite(EditorSprite* pEditSprite);
    void selectSingleEditorSprite(EditorSprite* pEditSprite);
    void selectAllEditorSprites();
    void toggleSelectEditorSprite(EditorSprite* pEditSprite);
    void selectMultipleEditorSprites(const QList<EditorSprite*>& pEditSprites);
    void unselectEditorSprite(EditorSprite* pEditSprite);
    void unselectAllEditorSprites();

    // Gestion de suppression
    void deleteEditorSprite(EditorSprite* pEditSprite);
    void deleteSelectedEditorSprites();

    // Gestion de modification de sprites
    void moveEditorSprite(EditorSprite* pEditSprite, QPointF moveVector);
    void moveSelectedEditorSprites(QPointF moveVector);

    // Gestion de l'image de fond
    void setBackGroundImage(QString imageFileName = QString());
    QString getBackgroundImagePath() const { return m_backgroundImageFileName; }
    void removeBackGroundImage();

private:
    GameScene* m_pScene = nullptr;

    QString m_name = QString();

    EditorHistory* m_editorHistory = nullptr;

    QString m_backgroundImageFileName = QString();

    // Etat de touche
    bool m_isShiftHeld = false;
    bool m_isCtrlHeld = false;

    // Etat de la souris
    Qt::MouseButtons m_heldMouseButtons;

    // Multi selection
    SelectionZone* m_pMultiSelectionZone = nullptr;

    // Drag and drop
    QPointF m_startDragPosition;
    bool m_isDragging = false;
    EditorSprite* mouseDownEditorSprite = nullptr;

    // Liste des sprites
    QList<EditorSprite*> m_pEditorSprites;
    QList<EditorSprite*> m_pSelectedEditorSprites;

    QString loadImageToEditor();

private slots:
    void editorSpriteClicked(EditorSprite* pEditSprite);

    void onKeyPressed(int key);
    void onKeyReleased(int key);

    void onMouseMoved(QPointF newMousePosition, QPointF oldMousePosition);
    void onMouseButtonPressed(QPointF mousePosition, Qt::MouseButtons buttons);
    void onMouseButtonReleased(QPointF mousePosition, Qt::MouseButtons buttons);

    void updateMultiSelect(QPointF &newMousePosition);
};


#endif //WORLDBUILDR_EDITORMANAGER_H
