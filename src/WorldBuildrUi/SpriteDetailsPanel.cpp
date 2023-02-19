//
// Created by Noah on 18.02.2023.
//

#include <QPushButton>
#include <QHBoxLayout>
#include <QSpinBox>
#include "SpriteDetailsPanel.h"

SpriteDetailsPanel::SpriteDetailsPanel(QWidget *pParent) : QWidget(pParent) {
    // Création du layout
    mainLayout = new QHBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    // Initialisation des boutons
    initInputs();

    // Initialisation du layout
    initLayout();
}

//! Lie un sprite au panneau de détails.
void SpriteDetailsPanel::bindSprite(EditorSprite *sprite) {
    if (sprite == nullptr) { // Si le sprite est nul, on délie le sprite actuel
        unbindSprite();
        return;
    }
    m_pSprite = sprite;

    // Mettre à jour le panneau
    updatePanel();
}

//! Délie le sprite du panneau de détails.
void SpriteDetailsPanel::unbindSprite() {
    m_pSprite = nullptr;
    updatePanel();
}

//! Met à jour les données du sprite dans le panneau.
void SpriteDetailsPanel::updatePanel() {
    if (m_pSprite == nullptr) { // Si le sprite est nul, on ne fait rien
        return;
    }
}

//! Initialise le layout du panneau.
void SpriteDetailsPanel::initLayout() {
    // Création des layouts
    positionLayout = new QVBoxLayout();
    sizeLayout = new QVBoxLayout();

    // Ajout des champs de texte
    positionLayout->addWidget(xPositionEdit);
    positionLayout->addWidget(yPositionEdit);

    sizeLayout->addWidget(widthEdit);
    sizeLayout->addWidget(heightEdit);

    // Ajout des layouts dans les groupes
    positionGroup->setLayout(positionLayout);
    sizeGroup->setLayout(sizeLayout);

    // Ajout des groupes dans le layout principal
    mainLayout->addWidget(positionGroup);
    mainLayout->addWidget(sizeGroup);
    mainLayout->addWidget(rotationEdit);
}

//! Initialise les champs du panneau.
void SpriteDetailsPanel::initInputs() {
    // Création des champs de texte

    // Creation et setup des champs de position
    xPositionEdit = new QSpinBox();
    xPositionEdit->setRange(0, 10000);
    xPositionEdit->setSingleStep(10);
    xPositionEdit->setSuffix(" px");
    yPositionEdit = new QSpinBox();
    yPositionEdit->setRange(0, 10000);
    yPositionEdit->setSingleStep(10);
    yPositionEdit->setSuffix(" px");

    // Creation et setup des champs de taille
    widthEdit = new QSpinBox();
    widthEdit->setRange(0, 10000);
    widthEdit->setSingleStep(10);
    widthEdit->setSuffix(" px");
    heightEdit = new QSpinBox();
    heightEdit->setRange(0, 10000);
    heightEdit->setSingleStep(10);
    heightEdit->setSuffix(" px");

    // Creation et setup du champ de rotation
    rotationEdit = new QSpinBox();
    rotationEdit->setRange(0, 360);
    rotationEdit->setSingleStep(10);
    rotationEdit->setSuffix("°");
}
