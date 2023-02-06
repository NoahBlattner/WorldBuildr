//
// Created by Noah on 29.01.2023.
//

#ifndef WORLDBUILDR_EDITORSPRITE_H
#define WORLDBUILDR_EDITORSPRITE_H


#include "sprite.h"

//! Classe représentant un sprite dans l'éditeur.
//! Elle peut être sélectionnée avec un clique de souris.
//! Si elle est sélectionnée, elle est entourée d'une bordure.
class EditorSprite : public Sprite {
    Q_OBJECT

public:
    explicit EditorSprite(const QString& imageFileName, QGraphicsItem* pParent = nullptr);

    virtual void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = nullptr) override;

    EditorSprite* clone() const;

    void setEditSelected(bool selected);
    bool getEditSelected() const;

    QString getImgPath() const { return m_imagePath; }

private:
    QString m_imagePath = "";

    bool m_isEditSelected = false;

signals:
    void editorSpriteLeftClicked(EditorSprite* pSprite);
};


#endif //WORLDBUILDR_EDITORSPRITE_H
