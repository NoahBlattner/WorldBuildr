/**
 * @file SelectionZone.h
 * @brief Déclaration de la classe SelectionZone.
 * @author Noah Blattner
 * @date Janvier 2023
 */

#ifndef WORLDBUILDR_SELECTIONZONE_H
#define WORLDBUILDR_SELECTIONZONE_H


#include "GameFramework/sprite.h"

class EditorSprite;

//! Cette classe représente une zone de sélection.
//! Elle est utilisée pour sélectionner plusieurs sprites d'éditeur.
//! Elle est un sprite pour pouvoir être affichée sur la scène.
//! Elle est une QRectF pour pouvoir être déformé comme un rectangle.
//!
//! Les méthodes de gestion de la sélection sont :
//! La méthode getCollidingEditorSprites() permet de récupérer la liste des sprites d'éditeur qui sont en collision avec la zone de sélection.
//! La méthode updateSelection() permet de mettre à jour la zone de sélection en fonction de la position de la souris.
//! La méthode endSelection() permet de terminer la sélection et de récupérer la liste des sprites d'éditeur sélectionnés.
//!
//! Les méthodes de gestion de l'affichage sont :
//! La méthode paint() permet d'afficher la zone de sélection.
class SelectionZone : public Sprite, QRectF {
    Q_OBJECT

public:
    explicit SelectionZone(GameScene* scene, QPointF startPosition);

    QList<EditorSprite*> getCollidingEditorSprites() const;

    void updateSelection(QPointF mousePos);
    QList<EditorSprite*> endSelection();

private:
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;

    QPointF m_startPoint;

};


#endif //WORLDBUILDR_SELECTIONZONE_H
