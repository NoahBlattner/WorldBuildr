/**
 * @file SelectionZone.cpp
 * @brief Définition de la classe SelectionZone.
 * @author Noah Blattner
 * @date Janvier 2023
 */

#include "SelectionZone.h"
#include "GameScene.h"
#include "EditorSprite.h"
#include "QPainter"

//! \brief Crée un nouveau sprite de sélection à Z-index 1000.
SelectionZone::SelectionZone(GameScene* scene, QPointF startPosition) : QRectF(startPosition, QSizeF(0, 0)) {
    m_startPoint = startPosition;

    // On ajoute le sprite à la scène
    scene->addSpriteToScene(this);
    setZValue(1000);
}

//! \brief Retourne la liste des sprites en collision qui sont des EditorSprite.
QList<EditorSprite *> SelectionZone::getCollidingEditorSprites() const {
    QList<EditorSprite*> collidingEditorSprites;

    // Garde seulement les sprites qui sont des EditorSprite
    for (auto & i : m_pParentScene->collidingSprites(*this)) {
        auto* editorSprite = dynamic_cast<EditorSprite*>(i);
        if (editorSprite != nullptr)
            collidingEditorSprites.append(editorSprite);
    }

    return collidingEditorSprites;
}

//! \brief Met à jour la sélection.
void SelectionZone::updateSelection(QPointF mousePos) {
    // On met à jour la taille de la zone de sélection
    QPointF diff = mousePos - m_startPoint;
    setSize(QSizeF(diff.x(), diff.y()));
    update();
}

//! \brief Termine la sélection et supprime le sprite.
QList<EditorSprite*> SelectionZone::endSelection() {
    auto collidingEditorSprites = getCollidingEditorSprites();

    // On supprime la zone de sélection
    m_pParentScene->removeSpriteFromScene(this);
    delete this;

    return collidingEditorSprites;
}

void SelectionZone::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    // On dessine la zone de sélection
    painter->setPen(Qt::lightGray);
    painter->drawRect(*this);
}




