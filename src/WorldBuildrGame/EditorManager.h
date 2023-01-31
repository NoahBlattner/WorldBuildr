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
    bool m_isShiftHeld = false;
    GameScene* m_pScene = nullptr;

    SelectionZone* m_pMultiSelectionZone = nullptr;

    QList<EditorSprite*> m_pSelectedEditorSprites;

    void selectSingleEditorSprite(EditorSprite* pEditSprite);
    void selectMultipleEditorSprites(const QList<EditorSprite *> &pEditSprites);
    void unSelectAllEditorSprites();

    void createSelectionZone(QPointF startPositon);

private slots:
    void onEditorSpriteClicked(EditorSprite* pEditSprite);

    void onKeyPressed(int key);
    void onKeyReleased(int key);

    void onMouseMoved(QPointF newMousePosition);
    void onMouseButtonPressed(QPointF mousePosition, Qt::MouseButtons buttons);
    void onMouseButtonReleased(QPointF mousePosition, Qt::MouseButtons buttons);
};


#endif //WORLDBUILDR_EDITORMANAGER_H
