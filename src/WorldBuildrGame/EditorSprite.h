//
// Created by Noah on 29.01.2023.
//

#ifndef WORLDBUILDR_EDITORSPRITE_H
#define WORLDBUILDR_EDITORSPRITE_H


#include <QPaintEvent>
#include "sprite.h"

// TODO : Add documentation
class EditorSprite : public Sprite {
    Q_OBJECT
public:
    explicit EditorSprite(const QString& imageFileName, QGraphicsItem* pParent = nullptr);

    virtual void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = nullptr) override;

    void setEditSelected(bool selected);
    bool getEditSelected() const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    bool m_isEditSelected = false;

signals:
    void editorSpriteClicked(EditorSprite* pSprite);
};


#endif //WORLDBUILDR_EDITORSPRITE_H
