/**
 * @file EditorSprite.h
 * @brief Définition de la classe EditorSprite.
 * @author Noah Blattner
 * @date Janvier 2023
 */

#ifndef WORLDBUILDR_EDITORSPRITE_H
#define WORLDBUILDR_EDITORSPRITE_H


#include "sprite.h"

//! Classe représentant une sprite dans l'éditeur.
//! Une fois ajoutée à une scène, elle écoute le clic gauche de la souris pour envoyer un signal.
//! Elle gère également l'affichage de la sélection.
//! Elle gère le contour donnant l'information de sélection.
//!
//! Les méthodes de gestion de la sélection sont :
//! La méthode setEditSelected() permet de définir si la sprite est sélectionnée ou non. Elle met à jour l'affichage.
//! La méthode getEditSelected() permet de savoir si la sprite est sélectionnée ou non.
//!
//! Les méthodes de gestion de l'image sont :
//! La méthode getImgPath() permet de récupérer le chemin de l'image utilisée par le sprite.
class EditorSprite : public Sprite {
    Q_OBJECT

public:
    explicit EditorSprite(const QString& imageFileName, bool selected = false, QGraphicsItem* pParent = nullptr);

    virtual void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = nullptr) override;

    void setEditSelected(bool selected);
    bool getEditSelected() const;

    void setTag(const QString& tag);
    QString getTag();
    void removeTag();

    QString getImgPath() const { return m_imagePath; }

    EditorSprite* clone() const;

    void setX(qreal x);
    void setY(qreal y);
    void moveBy(qreal dx, qreal dy);
    void setRotation(qreal angle);

private:
    const int TAG_KEY = 0;

    QString m_imagePath = "";

    bool m_isEditSelected = false;

signals:
    void editorSpriteLeftClicked(EditorSprite* pSprite);

    void editorSpriteUnselected();

    void editorSpriteModified();
};


#endif //WORLDBUILDR_EDITORSPRITE_H
