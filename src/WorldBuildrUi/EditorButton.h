//
// Created by blatnoa on 02.02.2023.
//

#ifndef WORLDBUILDR_EDITORBUTTON_H
#define WORLDBUILDR_EDITORBUTTON_H

#include <Sprite.h>

class EditorButton : public QGraphicsPixmapItem {
    public :
        EditorButton(QString imagePath, void (*onClick)(), QGraphicsItem* parent = nullptr);
        ~EditorButton();

        void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    void (*m_onClick)();
};


#endif //WORLDBUILDR_EDITORBUTTON_H
