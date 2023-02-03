//
// Created by blatnoa on 01.02.2023.
//

#ifndef WORLDBUILDR_EDITORHUD_H
#define WORLDBUILDR_EDITORHUD_H

#include <QGraphicsScene>

class EditorManager;
class EditorActionPanel;
class GameCore;

class EditorHud : public QGraphicsScene {
public:
    EditorHud(QWidget* pParent = nullptr);

    void initHud(EditorManager* editorManager);

private:
    EditorManager* m_pEditorManager = nullptr;

    EditorActionPanel* actionPanel = nullptr;

};


#endif //WORLDBUILDR_EDITORHUD_H
