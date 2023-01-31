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
    // Etat de touche
    bool m_isShiftHeld = false;

    // Etat de la souris
    QPointF previousMousePosition;
    Qt::MouseButtons m_heldMouseButtons;

    GameScene* m_pScene = nullptr;

    SelectionZone* m_pMultiSelectionZone = nullptr;

    QList<EditorSprite*> m_pSelectedEditorSprites;
    EditorSprite* clickedSprite;
    bool m_holdingEditorSprite = false;

    void selectSingleEditorSprite(EditorSprite* pEditSprite);
    void selectMultipleEditorSprites(const QList<EditorSprite *> &pEditSprites);
    void unSelectAllEditorSprites();

    void createSelectionZone(QPointF startPositon);

    void moveSelectedEditorSprites(QPointF moveVector);

private slots:
    void editorSpriteClicked(EditorSprite* pEditSprite);

    void onKeyPressed(int key);
    void onKeyReleased(int key);

    void onMouseMoved(QPointF newMousePosition, QPointF oldMousePosition);
    void onMouseButtonPressed(QPointF mousePosition, Qt::MouseButtons buttons);
    void onMouseButtonReleased(QPointF mousePosition, Qt::MouseButtons buttons);
};


#endif //WORLDBUILDR_EDITORMANAGER_H
