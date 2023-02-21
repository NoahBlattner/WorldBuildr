//
// Created by Noah on 18.02.2023.
//

#include <QPushButton>
#include <QHBoxLayout>
#include <QSpinBox>
#include "EditorSprite.h"
#include "SpriteDetailsPanel.h"

SpriteDetailsPanel::SpriteDetailsPanel(QWidget *pParent) : QWidget(pParent) {
    // Création du layout
    mainLayout = new QHBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    // Initialisation des boutons
    initInputs();

    // Initialisation du layout
    initLayout();

    // Initialisation des signaux
    connectSignals();
}

//! Lie un sprite au panneau de détails.
void SpriteDetailsPanel::onBindSprite(EditorSprite *sprite) {
    onUnbindSprite(); // On délie le sprite actuel

    if (sprite == nullptr) // Si le sprite est nul, on ne fait rien
        return;

    m_pSprite = sprite;

    // Connecter les signaux de modification du sprite
    connect(m_pSprite, &EditorSprite::editorSpriteModified, this, &SpriteDetailsPanel::onSpriteModified);

    // Connecter le signal de suppression du sprite
    connect(m_pSprite, &EditorSprite::spriteDestroyed, this, &SpriteDetailsPanel::onUnbindSprite);

    // Mettre à jour le panneau
    updatePanel();
}

//! Délie le sprite du panneau de détails.
void SpriteDetailsPanel::onUnbindSprite() {
    disconnect(this); // Déconnecter tous les signaux
    m_pSprite = nullptr;
    updatePanel();
}

//! Appelé lorsque le sprite est modifié.
void SpriteDetailsPanel::onSpriteModified() {
    updatePanel();
}

//! Met à jour les données du sprite dans le panneau.
void SpriteDetailsPanel::updatePanel() {
    if (m_pSprite == nullptr) { // Si le sprite est nul, on ne fait rien
        return;
    }

    // Mettre à jour les champs de position
    xPositionEdit->setValue(m_pSprite->x());
    yPositionEdit->setValue(m_pSprite->y());

    // Mettre à jour les champs de taille
    widthEdit->setValue(m_pSprite->width());
    heightEdit->setValue(m_pSprite->height());

    // Mettre à jour le champ de rotation
    rotationEdit->setValue(m_pSprite->rotation());
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

//! Connecte les signaux aux slots.
void SpriteDetailsPanel::connectSignals() {
    // Connecter les signaux de modification des champs de position
    connect(xPositionEdit, &QSpinBox::valueChanged, this, &SpriteDetailsPanel::xPositionFieldChanged);
    connect(yPositionEdit, &QSpinBox::valueChanged, this, &SpriteDetailsPanel::yPositionFieldChanged);

    // Connecter les signaux de modification des champs de taille
    connect(widthEdit, &QSpinBox::valueChanged, this, &SpriteDetailsPanel::widthFieldChanged);
    connect(heightEdit, &QSpinBox::valueChanged, this, &SpriteDetailsPanel::heightFieldChanged);

    // Connecter le signal de modification du champ de rotation
    connect(rotationEdit, &QSpinBox::valueChanged, this, &SpriteDetailsPanel::rotationFieldChanged);
}

/*******************************
 * Slots
 *******************************/

//! Appelé lorsque la largeur du sprite est modifiée.
//! \param value La nouvelle valeur de la largeur.
void SpriteDetailsPanel::xPositionFieldChanged(int value) {
    if (m_pSprite == nullptr) // Si le sprite est nul, on ne fait rien
        return;

    m_pSprite->setX(value);
}

//! Appelé lorsque la hauteur du sprite est modifiée.
//! \param value La nouvelle valeur de la hauteur.
void SpriteDetailsPanel::yPositionFieldChanged(int value) {
    if (m_pSprite == nullptr) // Si le sprite est nul, on ne fait rien
        return;

    m_pSprite->setY(value);
}

//! Appelé lorsque la rotation du sprite est modifiée.
//! \param value La nouvelle valeur de largeur.
void SpriteDetailsPanel::widthFieldChanged(int value) {
    if (m_pSprite == nullptr) // Si le sprite est nul, on ne fait rien
        return;

    // TODO
    // m_pSprite->setWidth(value);
}

//! Appelé lorsque la rotation du sprite est modifiée.
//! \param value La nouvelle valeur de rotation.
void SpriteDetailsPanel::heightFieldChanged(int value) {
    if (m_pSprite == nullptr) // Si le sprite est nul, on ne fait rien
        return;

    // TODO
    // m_pSprite->setHeight(value);
}

//! Appelé lorsque la rotation du sprite est modifiée.
//! \param value La nouvelle valeur de rotation.
void SpriteDetailsPanel::rotationFieldChanged(int value) {
    if (m_pSprite == nullptr) // Si le sprite est nul, on ne fait rien
        return;

    m_pSprite->setRotation(value);
}
