//
// Created by Noah on 29.01.2023.
//

#include "EditorSprite.h"
#include <QGraphicsSceneMouseEvent>
#include "GameScene.h"
#include <iostream>
#include <QPainter>

EditorSprite::EditorSprite(const QString &imageFileName, QGraphicsItem *pParent) : Sprite(imageFileName, pParent) {
    // On centre le point d'origine du sprite
    setTransformOriginPoint(globalBoundingRect().center());
}

//! \brief Dessine le sprite.
void EditorSprite::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget) {
    Sprite::paint(pPainter, pOption, pWidget);

    if (m_isEditSelected) {
        pPainter->setPen(Qt::lightGray);
        pPainter->drawPath(shape());
    }
}

//! \brief Set si le sprite est sélectionné et met à jour l'affichage.
//! \param selected     True si le sprite est sélectionné.
void EditorSprite::setEditSelected(bool selected) {
    m_isEditSelected = selected;

    update();
}

//! \brief Retourne si le sprite est sélectionné.
bool EditorSprite::getEditSelected() const {
    return m_isEditSelected;
}
