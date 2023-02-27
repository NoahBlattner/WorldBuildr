//
// Created by Noah on 18.02.2023.
//

#include <QPushButton>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QDoubleSpinBox>
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

    // Met à jour le panneau de détails
    updatePanel();

    // TODO Add Z-Index and opacity fields
}

//! Lie le gestionnaire d'éditeur au panneau de détails.
void SpriteDetailsPanel::bindEditorManager(EditorManager *pEditorManager) {
    m_pEditorManager = pEditorManager;
    // Connecte les signaux du gestionnaire d'éditeur aux slots du panneau de détails
    connect(m_pEditorManager, &EditorManager::editorSpriteSelected, this,
            &SpriteDetailsPanel::onBindSprite);
}

//! Lie un sprite au panneau de détails.
void SpriteDetailsPanel::onBindSprite(EditorSprite *sprite) {
    if (sprite == nullptr) // Si le sprite est nul, on ne fait rien
        return;

    // Déconnecter les signaux du sprite précédent
    disconnectSpriteSignals();

    // Mettre à jour le sprite
    m_pSprite = sprite;

    // Connecter les signaux du nouveau sprite
    connectSpriteSignals();

    // Mettre à jour le panneau
    updatePanel();
}

//! Connecte les signaux du sprite au panneau de détails.
void SpriteDetailsPanel::connectSpriteSignals() const {// Connecter les signaux de modification du sprite
    connect(m_pSprite, &EditorSprite::editorSpriteModified, this, &SpriteDetailsPanel::onSpriteModified);

    // Connecter le signal de désélection du sprite
    connect(m_pSprite, &EditorSprite::editorSpriteUnselected, this, &SpriteDetailsPanel::onUnbindSprite);

    // Connecter le signal de suppression du sprite
    connect(m_pSprite, &EditorSprite::spriteDestroyed, this, &SpriteDetailsPanel::onUnbindSprite);
}

//! Déconnecte les signaux du sprite.
void SpriteDetailsPanel::disconnectSpriteSignals() const {// Déconnecter les signaux du sprite
    if (m_pSprite == nullptr) // Si le sprite est nul, on ne fait rien
        return;
    disconnect(m_pSprite, &EditorSprite::editorSpriteModified, this, &SpriteDetailsPanel::onSpriteModified);
    disconnect(m_pSprite, &EditorSprite::editorSpriteUnselected, this, &SpriteDetailsPanel::onUnbindSprite);
    disconnect(m_pSprite, &EditorSprite::spriteDestroyed, this, &SpriteDetailsPanel::onUnbindSprite);
}

//! Délie le sprite du panneau de détails.
void SpriteDetailsPanel::onUnbindSprite() {
    if (m_pSprite == nullptr) // Si le sprite est nul, on ne fait rien
        return;

    disconnectSpriteSignals();

    m_pSprite = nullptr;

    // Mettre à jour le panneau
    updatePanel();
}

//! Appelé lorsque le sprite est modifié.
void SpriteDetailsPanel::onSpriteModified() {
    updatePanel();
}

//! Met à jour les données du sprite dans le panneau.
void SpriteDetailsPanel::updatePanel() {
    if (m_pSprite == nullptr) { // Si le sprite est nul
        // Désactiver le panneau
        setEnabled(false);
        return;
    }

    setEnabled(true); // Activer le panneau

    m_ignoreFieldEdited = true; // Ignorer les signaux de modification des champs

    // Mettre à jour les champs de position
    xPositionEdit->setValue(m_pSprite->x());
    yPositionEdit->setValue(m_pSprite->y());
    zPositionEdit->setValue(m_pSprite->zValue());

    // Mettre à jour les champs de taille
    scaleEdit->setValue(m_pSprite->scale());

    // Mettre à jour le champ de rotation
    rotationEdit->setValue(m_pSprite->rotation());

    m_ignoreFieldEdited = false; // Ne plus ignorer les signaux de modification des champs
}

//! Initialise le layout du panneau.
void SpriteDetailsPanel::initLayout() {
    // Création des layouts
    optionsLayout = new QHBoxLayout();
    optionsLayout->setAlignment(Qt::AlignCenter);
    positionLayout = new QVBoxLayout();
    positionLayout->setAlignment(Qt::AlignCenter);
    scaleLayout = new QVBoxLayout();
    scaleLayout->setAlignment(Qt::AlignCenter);
    rotationLayout = new QVBoxLayout();
    rotationLayout->setAlignment(Qt::AlignCenter);

    // Ajout des champs de texte et leur label
    // Options
    auto* option1VBox = new QVBoxLayout();
    option1VBox->setAlignment(Qt::AlignCenter);
    option1VBox->addWidget(new QLabel("Position"));
    option1VBox->addWidget(posStepEdit);
    optionsLayout->addLayout(option1VBox);
    auto* option2VBox = new QVBoxLayout();
    option2VBox->addWidget(new QLabel("Rotation"));
    option2VBox->addWidget(rotationStepEdit);
    optionsLayout->addLayout(option2VBox);

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
    auto* zPosHBox = new QHBoxLayout();
    zPosHBox->setAlignment(Qt::AlignCenter);
    zPosHBox->addWidget(new QLabel("Z"));
    zPosHBox->addWidget(zPositionEdit);
    positionLayout->addLayout(zPosHBox);

    // Taille
    scaleLayout->addWidget(scaleEdit);

    // Rotation
    rotationLayout->addWidget(rotationEdit);

    // Ajout des layouts dans les groupes
    auto* optionsGroup = new QGroupBox("Options d'intervalles");
    optionsGroup->setLayout(optionsLayout);
    auto* positionGroup = new QGroupBox("Position");
    positionGroup->setLayout(positionLayout);
    auto* scaleGroup = new QGroupBox("Taille");
    scaleGroup->setLayout(scaleLayout);
    auto* rotationGroup = new QGroupBox("Rotation");
    rotationGroup->setLayout(rotationLayout);

    // Ajout des groupes dans le layout principal
    mainLayout->addWidget(optionsGroup);
    mainLayout->addWidget(positionGroup);
    mainLayout->addWidget(scaleGroup);
    mainLayout->addWidget(rotationGroup);
}

//! Initialise les champs du panneau.
void SpriteDetailsPanel::initInputs() {
    // Création des champs de texte

    // Création et setup des champs d'options
    posStepEdit = new QComboBox();
    posStepEdit->addItems(PIXEL_STEPS);
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
    zPositionEdit = new QSpinBox();
    zPositionEdit->setRange(0, 10000);
    zPositionEdit->setSingleStep(1);

    // Creation et setup des champs de taille
    scaleEdit = new QDoubleSpinBox();
    scaleEdit->setRange(.05, 5);

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
    connect(rotationStepEdit, &QComboBox::currentIndexChanged   , this, &SpriteDetailsPanel::onRotationStepChanged);

    // Connecter les signaux de modification des champs de position
    connect(xPositionEdit, &QSpinBox::valueChanged, this, &SpriteDetailsPanel::onXPosFieldEdited);
    connect(yPositionEdit, &QSpinBox::valueChanged, this, &SpriteDetailsPanel::onYPosFieldEdited);
    connect(zPositionEdit, &QSpinBox::valueChanged, this, &SpriteDetailsPanel::onZPosFieldEdited);

    // Connecter les signaux de modification des champs de taille
    connect(scaleEdit, &QDoubleSpinBox::valueChanged, this, &SpriteDetailsPanel::onScaleFieldEdited);

    // Connecter le signal de modification du champ de rotation
    connect(rotationEdit, &QSpinBox::valueChanged, this, &SpriteDetailsPanel::onRotationFieldEdited);
}

/*******************************
 * Slots
 *******************************/

//! Appelé lorsque la valeur d'intervalle de position est modifiée.
void SpriteDetailsPanel::onPosStepChanged(int newStepIndex) {
    // Mettre à jour l'intervalle du champs de position
    int newStep = PIXEL_STEPS[newStepIndex].split(" ")[0].toInt();
    xPositionEdit->setSingleStep(newStep);
    yPositionEdit->setSingleStep(newStep);
}

//! Appelé lorsque la valeur d'intervalle de rotation est modifiée.
void SpriteDetailsPanel::onRotationStepChanged(int newStepIndex) {
    // Mettre à jour l'intervalle du champs de rotation
    rotationEdit->setSingleStep(ROTATION_STEPS[newStepIndex].split("°")[0].toInt());
}

//! Appelé lorsque la largeur du sprite est modifiée.
//! \param value La nouvelle valeur de la largeur.
void SpriteDetailsPanel::onXPosFieldEdited(int value) {
    if (m_pSprite == nullptr || m_ignoreFieldEdited) // Si le sprite est nul ou qu'on ignore les changements
        // On ne fait rien
        return;

    m_pEditorManager->moveEditorSprite(m_pSprite,QPointF(value-m_pSprite->x(), 0));
}

//! Appelé lorsque la hauteur du sprite est modifiée.
//! \param value La nouvelle valeur de la hauteur.
void SpriteDetailsPanel::onYPosFieldEdited(int value) {
    if (m_pSprite == nullptr || m_ignoreFieldEdited) // Si le sprite est nul ou qu'on ignore les changements
        // On ne fait rien
        return;

    m_pEditorManager->moveEditorSprite(m_pSprite,QPointF(0, value-m_pSprite->y()));
}

//! Appelé lorsque la profondeur du sprite est modifiée.
//! \param value La nouvelle valeur de la profondeur.
void SpriteDetailsPanel::onZPosFieldEdited(int value) {
    if (m_pSprite == nullptr || m_ignoreFieldEdited) // Si le sprite est nul ou qu'on ignore les changements
        // On ne fait rien
        return;

    m_pEditorManager->setEditorSpriteZIndex(m_pSprite, value);
}

//! Appelé lorsque la taille du sprite est modifiée.
//! \param newScale La nouvelle valeur de la taille.
void SpriteDetailsPanel::onScaleFieldEdited(double newScale) {
    if (m_pSprite == nullptr || m_ignoreFieldEdited) // Si le sprite est nul ou qu'on ignore les changements
        // On ne fait rien
        return;

    m_pEditorManager->rescaleEditorSprite(m_pSprite, newScale);
}

//! Appelé lorsque la rotation du sprite est modifiée.
//! \param value La nouvelle valeur de rotation.
void SpriteDetailsPanel::onRotationFieldEdited(int value) {
    if (m_pSprite == nullptr || m_ignoreFieldEdited) // Si le sprite est nul ou qu'on ignore les changements
        // On ne fait rien
        return;

    // On transforme la valeur pour qu'elle soit comprise entre 0 et 360
    if (value < 0) {
        value = 360 + value;
        rotationEdit->setValue(value);
    } else if (value >= 360) {
        value = value - 360;
        rotationEdit->setValue(value);
    }

    m_pEditorManager->setEditorSpriteRotation(m_pSprite, value);
}