//
// Created by blatnoa on 31.01.2023.
//

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
//! Elle gère la création et la destruction des sprites d'éditeur.
//! Elle gère la sélection des sprites d'éditeur.
//! Elle gère le drag and drop des sprites d'éditeur.
//! Elle gère la création de zones de sélection.
//!
//! createNewEditorSprite() : Crée un sprite d'éditeur à la position donnée avec l'image donnée.
class EditorManager : public QWidget {
    Q_OBJECT

public:
    explicit EditorManager(GameCore* core);

    bool containsEditorSprite(EditorSprite* pEditSprite) const;

    void resetHistory();

    // Gestion de création
    void createSelectionZone(QPointF startPositon);
    void createNewEditorSprite(QString imageFileName = QString(), QPointF position = QPointF(0, 0));
    void addEditorSprite(EditorSprite *pEditorSprite, const QPointF &position = QPointF(0, 0));
    void duplicateEditorSprite(EditorSprite* pEditSprite);
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
    void removeBackGroundImage();

private:
    GameScene* m_pScene = nullptr;

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
