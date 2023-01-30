//
// Created by Noah on 30.01.2023.
//

#ifndef WORLDBUILDR_SELECTIONZONE_H
#define WORLDBUILDR_SELECTIONZONE_H


#include "sprite.h"

class EditorSprite;

class SelectionZone : public Sprite {
    Q_OBJECT

public:
    explicit SelectionZone(QPointF startPosition, QGraphicsItem* pParent = nullptr);

    virtual void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = nullptr) override;

    QList<EditorSprite*> getCollidingEditorSprites() const;

    void updateSelection(QPointF mousePos);
    void endSelection();

private:
    QPointF m_startPos;
    QPointF m_endPos;


};


#endif //WORLDBUILDR_SELECTIONZONE_H
