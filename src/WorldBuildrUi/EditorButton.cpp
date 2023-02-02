//
// Created by blatnoa on 02.02.2023.
//

#include "EditorButton.h"

EditorButton::EditorButton(QString imagePath, void (*onClick)(), QGraphicsItem *parent) : QGraphicsPixmapItem(imagePath, parent) {
    m_onClick = onClick;
}

void EditorButton::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    m_onClick();
}

EditorButton::~EditorButton() = default;


