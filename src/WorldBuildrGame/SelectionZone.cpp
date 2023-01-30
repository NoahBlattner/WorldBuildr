//
// Created by Noah on 30.01.2023.
//

#include "SelectionZone.h"
#include "QPainter"
#include "QGraphicsSceneMouseEvent"
#include "GameScene.h"
#include "EditorSprite.h"

//! \brief Crée un nouveau sprite de sélection à Z-index 1000.
SelectionZone::SelectionZone(QPointF startPosition, QGraphicsItem *pParent) : Sprite(pParent) {
    // Set empty image
    setPixmap(QPixmap(1, 1));

    setZValue(1000);
    setPos(startPosition);
    m_startPos = startPosition;
}

//! \brief Dessine le sprite de sélection.
void SelectionZone::paint(QPainter *pPainter, const QStyleOptionGraphicsItem *pOption, QWidget *pWidget) {
    Sprite::paint(pPainter, pOption, pWidget);

    pPainter->setPen(Qt::blue);
    pPainter->drawRect(boundingRect());
}

//! \brief Retourne la liste des sprites en collision qui sont des EditorSprite.
QList<EditorSprite *> SelectionZone::getCollidingEditorSprites() const {
    QList<EditorSprite*> collidingEditorSprites;

    // Garde seulement les sprites qui sont des EditorSprite
    for (auto & i : collidingSprites()) {
        auto* editorSprite = dynamic_cast<EditorSprite*>(i);
        if (editorSprite != nullptr)
            collidingEditorSprites.append(editorSprite);
    }

    return collidingEditorSprites;
}

//! \brief Met à jour la sélection.
void SelectionZone::updateSelection(QPointF mousePos) {
    // Mettre à jour la taille de la zone de sélection
    m_endPos = mousePos;

    setPixmap(QPixmap(m_endPos.x() - m_startPos.x(), m_endPos.y() - m_startPos.y()));
}

//! \brief Termine la sélection et supprime le sprite.
void SelectionZone::endSelection() {
    // On supprime la zone de sélection
    dynamic_cast<GameScene*>(scene())->removeSpriteFromScene(this);
    delete this;
}




