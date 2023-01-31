//
// Created by Noah on 30.01.2023.
//

#ifndef WORLDBUILDR_SELECTIONZONE_H
#define WORLDBUILDR_SELECTIONZONE_H


#include "sprite.h"

class EditorSprite;

//! \author Noah Blattner
//! \brief Zone de sélection.
//!
//! Cette classe représente une zone de sélection. Elle est utilisée pour sélectionner plusieurs sprites d'éditeur.
//! Elle est un sprite pour pouvoir être affichée sur la scène.
//! Elle est une QRectF pour pouvoir être déformé comme un rectangle.
//!
//! Cette classe implémente les fonctions suivantes:
//! - Affichage de la zone de sélection.
//! - Récupération des sprites d'éditeur qui sont en collision avec la zone de sélection.
//! - Mise à jour de la zone de sélection.
//! - Suppression de la zone de sélection.
class SelectionZone : public Sprite, QRectF {
    Q_OBJECT

public:
    explicit SelectionZone(GameScene* scene, QPointF startPosition);

    QList<EditorSprite*> getCollidingEditorSprites() const;

    void updateSelection(QPointF mousePos);
    QList<EditorSprite*> endSelection();

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    GameScene* m_pScene;
    QPointF m_startPoint;

};


#endif //WORLDBUILDR_SELECTIONZONE_H
