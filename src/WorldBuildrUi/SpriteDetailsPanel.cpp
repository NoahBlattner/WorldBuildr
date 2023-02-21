//
// Created by Noah on 18.02.2023.
//

#include <QPushButton>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QComboBox>
#include <QLabel>
#include "EditorSprite.h"
#include "EditorManager.h"
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

//! Lie le gestionnaire d'éditeur au panneau de détails.
void SpriteDetailsPanel::bindEditorManager(EditorManager *pEditorManager) {
    m_pEditorManager = pEditorManager;
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
    optionsLayout = new QHBoxLayout();
    optionsLayout->setAlignment(Qt::AlignCenter);
    positionLayout = new QVBoxLayout();
    positionLayout->setAlignment(Qt::AlignCenter);
    sizeLayout = new QVBoxLayout();
    sizeLayout->setAlignment(Qt::AlignCenter);

    // Ajout des champs de texte et leur label
    // Options
    auto* option1VBox = new QVBoxLayout();
    option1VBox->setAlignment(Qt::AlignCenter);
    option1VBox->addWidget(new QLabel("Position"));
    option1VBox->addWidget(posStepEdit);
    optionsLayout->addLayout(option1VBox);
    auto* option2VBox = new QVBoxLayout();
    option2VBox->addWidget(new QLabel("Taille"));
    option2VBox->addWidget(sizeStepEdit);
    optionsLayout->addLayout(option2VBox);
    auto* option3VBox = new QVBoxLayout();
    option3VBox->addWidget(new QLabel("Rotation"));
    option3VBox->addWidget(rotationStepEdit);
    optionsLayout->addLayout(option3VBox);

    // Position
    auto* xPosHBox = new QHBoxLayout();
    xPosHBox->setAlignment(Qt::AlignCenter);
    xPosHBox->addWidget(new QLabel("X"));
    xPosHBox->addWidget(xPositionEdit);
    positionLayout->addLayout(xPosHBox);
    auto* yPosHBox = new QHBoxLayout();
    yPosHBox->setAlignment(Qt::AlignCenter);
    yPosHBox->addWidget(new QLabel("Y"));
    yPosHBox->addWidget(yPositionEdit);
    positionLayout->addLayout(yPosHBox);

    // Taille
    auto* widthHBox = new QHBoxLayout();
    widthHBox->setAlignment(Qt::AlignCenter);
    widthHBox->addWidget(new QLabel("Largeur"));
    widthHBox->addWidget(widthEdit);
    sizeLayout->addLayout(widthHBox);
    auto* heightHBox = new QHBoxLayout();
    heightHBox->setAlignment(Qt::AlignCenter);
    heightHBox->addWidget(new QLabel("Hauteur"));
    heightHBox->addWidget(heightEdit);
    sizeLayout->addLayout(heightHBox);

    // Ajout des layouts dans les groupes
    auto* optionsGroup = new QGroupBox("Options d'intervalles");
    optionsGroup->setLayout(optionsLayout);
    auto* positionGroup = new QGroupBox("Position");
    positionGroup->setLayout(positionLayout);
    auto* sizeGroup = new QGroupBox("Taille");
    sizeGroup->setLayout(sizeLayout);

    // Ajout des groupes dans le layout principal
    mainLayout->addWidget(optionsGroup);
    mainLayout->addWidget(positionGroup);
    mainLayout->addWidget(sizeGroup);
    mainLayout->addWidget(rotationEdit);
}

//! Initialise les champs du panneau.
void SpriteDetailsPanel::initInputs() {
    // Création des champs de texte

    // Création et setup des champs d'options
    posStepEdit = new QComboBox();
    posStepEdit->addItems(PIXEL_STEPS);
    sizeStepEdit = new QComboBox();
    sizeStepEdit->addItems(PIXEL_STEPS);
    rotationStepEdit = new QComboBox();
    rotationStepEdit->addItems(ROTATION_STEPS);

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
    rotationEdit->setRange(-1000, 1000);
    rotationEdit->setSingleStep(10);
    rotationEdit->setSuffix("°");
}

//! Connecte les signaux aux slots.
void SpriteDetailsPanel::connectSignals() {
    // Connecter les signaux de modification des champs d'options
    connect(posStepEdit, &QComboBox::currentIndexChanged, this, &SpriteDetailsPanel::onPosStepChanged);
    connect(sizeStepEdit,&QComboBox::currentIndexChanged, this, &SpriteDetailsPanel::onSizeStepChanged);
    connect(rotationStepEdit, &QComboBox::currentIndexChanged   , this, &SpriteDetailsPanel::onRotationStepChanged);

    // Connecter les signaux de modification des champs de position
    connect(xPositionEdit, &QSpinBox::valueChanged, this, &SpriteDetailsPanel::onXPosFieldEdited);
    connect(yPositionEdit, &QSpinBox::valueChanged, this, &SpriteDetailsPanel::onYPosFieldEdited);

    // Connecter les signaux de modification des champs de taille
    connect(widthEdit, &QSpinBox::valueChanged, this, &SpriteDetailsPanel::onWidthFieldEdited);
    connect(heightEdit, &QSpinBox::valueChanged, this, &SpriteDetailsPanel::onHeightFieldEdited);

    // Connecter le signal de modification du champ de rotation
    connect(rotationEdit, &QSpinBox::valueChanged, this, &SpriteDetailsPanel::onRotationFieldEdited);
}

/*******************************
 * Slots
 *******************************/

//! Appelé lorsque la valeur d'intervalle de position est modifiée.
void SpriteDetailsPanel::onPosStepChanged(int newStepIndex) {
    // Mettre à jour l'intervalle du champs de position
    xPositionEdit->setSingleStep(PIXEL_STEPS[newStepIndex].toInt());
    yPositionEdit->setSingleStep(PIXEL_STEPS[newStepIndex].toInt());
}

//! Appelé lorsque la valeur d'intervalle de taille est modifiée.
void SpriteDetailsPanel::onSizeStepChanged(int newStepIndex) {
    // Mettre à jour l'intervalle du champs de taille
    widthEdit->setSingleStep(PIXEL_STEPS[newStepIndex].toInt());
    heightEdit->setSingleStep(PIXEL_STEPS[newStepIndex].toInt());
}

//! Appelé lorsque la valeur d'intervalle de rotation est modifiée.
void SpriteDetailsPanel::onRotationStepChanged(int newStepIndex) {
    // Mettre à jour l'intervalle du champs de rotation
    rotationEdit->setSingleStep(ROTATION_STEPS[newStepIndex].toInt());
}

//! Appelé lorsque la largeur du sprite est modifiée.
//! \param value La nouvelle valeur de la largeur.
void SpriteDetailsPanel::onXPosFieldEdited(int value) {
    if (m_pSprite == nullptr) // Si le sprite est nul, on ne fait rien
        return;

    m_pEditorManager->moveEditorSprite(m_pSprite,QPointF(value, 0));
}

//! Appelé lorsque la hauteur du sprite est modifiée.
//! \param value La nouvelle valeur de la hauteur.
void SpriteDetailsPanel::onYPosFieldEdited(int value) {
    if (m_pSprite == nullptr) // Si le sprite est nul, on ne fait rien
        return;

    m_pEditorManager->moveEditorSprite(m_pSprite,QPointF(0, value));
}

//! Appelé lorsque la rotation du sprite est modifiée.
//! \param value La nouvelle valeur de largeur.
void SpriteDetailsPanel::onWidthFieldEdited(int value) {
    if (m_pSprite == nullptr) // Si le sprite est nul, on ne fait rien
        return;

    // TODO
    // m_pSprite->setWidth(value);
}

//! Appelé lorsque la rotation du sprite est modifiée.
//! \param value La nouvelle valeur de rotation.
void SpriteDetailsPanel::onHeightFieldEdited(int value) {
    if (m_pSprite == nullptr) // Si le sprite est nul, on ne fait rien
        return;

    // TODO
    // m_pSprite->setHeight(value);
}

//! Appelé lorsque la rotation du sprite est modifiée.
//! \param value La nouvelle valeur de rotation.
void SpriteDetailsPanel::onRotationFieldEdited(int value) {
    if (m_pSprite == nullptr) // Si le sprite est nul, on ne fait rien
        return;

    // On transforme la valeur pour qu'elle soit comprise entre 0 et 360
    if (value < 0) {
        value = 360 + value;
        rotationEdit->setValue(value);
    } else if (value >= 360) {
        value = value - 360;
        rotationEdit->setValue(value);
    }

    // TODO
    // m_pEditorManager->rotateEditorSprite(m_pSprite, value);
}
