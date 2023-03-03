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

    if (!selected) { // Si le sprite est désélectionné
        // On envoie un signal
        emit editorSpriteUnselected();
    }

    if (scene() != nullptr) // Si la sprite est dans une scène
        // On met à jour la scène
        update();
}

//! \brief Retourne si le sprite est sélectionné.
bool EditorSprite::getEditSelected() const {
    return m_isEditSelected;
}

//! \brief Ajoute un tag à la sprite. Si un tag existe déjà, il est remplacé.
//! \param tag   Tag à ajouter.
void EditorSprite::setTag(const QString &tag) {
    setData(TAG_KEY, tag);
}

//! \brief Retourne le tag de la sprite.
//! \return Tag de la sprite.
QString EditorSprite::getTag() {
    return data(TAG_KEY).toString();
}

//! \brief Supprime le tag de la sprite.
void EditorSprite::removeTag() {
    setData(TAG_KEY, QVariant());
}

//! Crée un clone du sprite d'éditeur.
EditorSprite *EditorSprite::clone() const {
    auto* clone = new EditorSprite(m_imagePath, m_isEditSelected);
    clone->setPos(pos());
    clone->setRotation(rotation());
    clone->setScale(scale());
    return clone;
}

/****************************
 * Override QGraphicsItem to add signals
 * Call the parent method and emit a signal for the corresponding value change
 ***************************/

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

//! \brief Change l'angle de rotation du sprite et émet un signal de modification.
//! \param angle    Angle de rotation.
void EditorSprite::setRotation(qreal angle) {
    QGraphicsItem::setRotation(angle);

    emit editorSpriteModified();
}
