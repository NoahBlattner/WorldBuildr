//
// Created by blatnoa on 31.01.2023.
//

#ifndef WORLDBUILDR_EDITORMANAGER_H
#define WORLDBUILDR_EDITORMANAGER_H

#include <QObject>
#include <QPointF>

class EditorSprite;
class SelectionZone;
class GameCore;
class GameScene;

class EditorManager : public QObject {
    Q_OBJECT

public:
    explicit EditorManager(GameCore* core);
    void createEditorSprite(const QString& imageFileName, QPointF position = QPointF(0, 0));

private:
    GameScene* m_pScene = nullptr;

    // Etat de touche
    bool m_isShiftHeld = false;
    bool m_isCtrlHeld = false;

    // Etat de la souris
    Qt::MouseButtons m_heldMouseButtons;

    // Multi selection
    SelectionZone* m_pMultiSelectionZone = nullptr;

    // Drag and drop
    EditorSprite* mouseDownEditorSprite;

    // Liste des sprites
    QList<EditorSprite*> m_pEditorSprites;
    QList<EditorSprite*> m_pSelectedEditorSprites;

    void selectEditorSprite(EditorSprite* pEditSprite);
    void selectSingleEditorSprite(EditorSprite* pEditSprite);
    void toggleSelectEditorSprite(EditorSprite* pEditSprite);
    void selectMultipleEditorSprites(const QList<EditorSprite*>& pEditSprites);
    void unSelectEditorSprite(EditorSprite* pEditSprite);
    void unSelectAllEditorSprites();

    void createSelectionZone(QPointF startPositon);

    void removeEditorSprite(EditorSprite* pEditSprite);
    void removeSelectedEditorSprites();

    void moveSelectedEditorSprites(QPointF moveVector);

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
