//
// Created by blatnoa on 01.02.2023.
//

#ifndef WORLDBUILDR_EDITORHUDSCENE_H
#define WORLDBUILDR_EDITORHUDSCENE_H

#include <QGraphicsScene>

class EditorManager;

class EditorHudScene : public QGraphicsScene {
public:
    EditorHudScene(EditorManager* editorManager);

private:
    EditorManager* m_pEditorManager;


};


#endif //WORLDBUILDR_EDITORHUDSCENE_H
