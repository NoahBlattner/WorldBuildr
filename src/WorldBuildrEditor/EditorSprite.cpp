/**
 * @file EditorSprite.cpp
 * @brief Définition de la classe EditorSprite.
 * @author Noah Blattner
 * @date Janvier 2023
 */

#include "EditorSprite.h"
#include <QGraphicsSceneMouseEvent>
#include "GameScene.h"
#include <iostream>
#include <QPainter>

EditorSprite::EditorSprite(const QString &imageFileName, bool selected, QGraphicsItem *pParent) : Sprite(imageFileName, pParent) {
    m_imagePath = imageFileName;
    m_isEditSelected = selected;

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

    if (scene() != nullptr)
        update();
}

//! \brief Retourne si le sprite est sélectionné.
bool EditorSprite::getEditSelected() const {
    return m_isEditSelected;
}

//! Crée un clone du sprite d'éditeur.
EditorSprite *EditorSprite::clone() const {
    auto* clone = new EditorSprite(m_imagePath, m_isEditSelected);
    clone->setPos(pos());
    clone->setRotation(rotation());
    clone->setScale(scale());
    return clone;
}

//! \brief Change la position en x du sprite et émet un signal de modification.
void EditorSprite::setX(qreal x) {
    QGraphicsItem::setX(x);

    emit editorSpriteModified();
}

//! \brief Change la position en y du sprite et émet un signal de modification.
void EditorSprite::setY(qreal y) {
    QGraphicsItem::setY(y);

    emit editorSpriteModified();
}

//! \brief Déplace le sprite et émet un signal de modification.
//! \param dx   Déplacement en x.
//! \param dy   Déplacement en y.
void EditorSprite::moveBy(qreal dx, qreal dy) {
    Sprite::moveBy(dx, dy);

    emit editorSpriteModified();
}