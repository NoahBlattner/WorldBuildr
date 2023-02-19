//
// Created by Noah on 18.02.2023.
//

#ifndef WORLDBUILDR_SPRITEDETAILSPANEL_H
#define WORLDBUILDR_SPRITEDETAILSPANEL_H


#include <QWidget>
#include <QGroupBox>

class QVBoxLayout;
class QHBoxLayout;
class QSpinBox;
class EditorSprite;

//! Classe de panneau de détails d'un sprite.
class SpriteDetailsPanel : public QWidget {

    Q_OBJECT

public slots:
    void onBindSprite(EditorSprite* sprite);
    void onUnbindSprite();

    void onSpriteModified();

public:
    explicit SpriteDetailsPanel(QWidget* pParent = nullptr);

private:
    EditorSprite* m_pSprite = nullptr;

    QHBoxLayout* mainLayout = nullptr;

    QGroupBox* positionGroup = new QGroupBox("Position");
    QGroupBox* sizeGroup = new QGroupBox("Taille");

    QVBoxLayout* positionLayout = nullptr;
    QVBoxLayout* sizeLayout = nullptr;

    QSpinBox* xPositionEdit;
    QSpinBox* yPositionEdit;

    QSpinBox* widthEdit;
    QSpinBox* heightEdit;

    QSpinBox* rotationEdit;

    void initLayout();
    void initInputs();

    void updatePanel();
};


#endif //WORLDBUILDR_SPRITEDETAILSPANEL_H
