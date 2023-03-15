/**
 * @file SpriteDetailsPanel.cpp
 * @brief Définition de la classe SpriteDetailsPanel.
 * @author Noah Blattner
 * @date Février 2023
 */

#include <QPushButton>
#include <QHBoxLayout>
#include <QSpinBox>
#include <QComboBox>
#include <QListWidget>
#include <QLabel>
#include "EditorSprite.h"
#include "EditorManager.h"
#include "SpriteDetailsPanel.h"
#include "resources.h"
#include "TagEditDialog.h"

SpriteDetailsPanel::SpriteDetailsPanel(QWidget *pParent) : QWidget(pParent) {
    // Création du layout
    mainLayout = new QHBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    // Initialisation des boutons
    initInputs();

    // Initialisation du layout
    initLayout();

    // Initialisation des signaux
    connectInputSignals();

    // Met à jour le panneau de détails
    updatePanel();
}

//! Lie le gestionnaire d'éditeur au panneau de détails.
void SpriteDetailsPanel::bindEditorManager(EditorManager *pEditorManager) {
    m_pEditorManager = pEditorManager;
    // Connecte les signaux du gestionnaire d'éditeur aux slots du panneau de détails
    connect(m_pEditorManager, &EditorManager::editorSpriteSelected, this,
            &SpriteDetailsPanel::onBindSprite);
    connect(m_pEditorManager, &EditorManager::editorSpriteDeleted, this,
            &SpriteDetailsPanel::onUnbindSprite);
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
}

//! Déconnecte les signaux du sprite.
void SpriteDetailsPanel::disconnectSpriteSignals() {// Déconnecter les signaux du sprite
    if (m_pSprite == nullptr) // Si le sprite est nul, on ne fait rien
        return;
    disconnect(m_pSprite, &EditorSprite::editorSpriteModified, this, &SpriteDetailsPanel::onSpriteModified);
    disconnect(m_pSprite, &EditorSprite::editorSpriteUnselected, this, &SpriteDetailsPanel::onUnbindSprite);
}

//! Délie le sprite du panneau de détails.
void SpriteDetailsPanel::onUnbindSprite() {
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
    m_pXPositionEdit->setValue(m_pSprite->x());
    m_pYPositionEdit->setValue(m_pSprite->y());
    m_pZPositionEdit->setValue(m_pSprite->zValue());

    // Mettre à jour les champs de taille
    m_pScaleEdit->setValue(m_pSprite->scale());

    // Mettre à jour le champ de rotation
    m_pRotationEdit->setValue(m_pSprite->rotation());

    // Mettre à jour le champ d'opacité
    m_pOpacityEdit->setValue(m_pSprite->opacity() * 100);

    m_ignoreFieldEdited = false; // Ne plus ignorer les signaux de modification des champs
}

//! Initialise le layout du panneau.
void SpriteDetailsPanel::initLayout() {
    // Création des layouts
    auto* optionsLayout = new QHBoxLayout();
    optionsLayout->setAlignment(Qt::AlignCenter);
    auto* positionLayout = new QVBoxLayout();
    positionLayout->setAlignment(Qt::AlignCenter);
    auto* scaleLayout = new QVBoxLayout();
    scaleLayout->setAlignment(Qt::AlignCenter);
    auto* rotationLayout = new QVBoxLayout();
    rotationLayout->setAlignment(Qt::AlignCenter);
    auto* opacityLayout = new QVBoxLayout();
    opacityLayout->setAlignment(Qt::AlignCenter);
    auto* tagLayout = new QVBoxLayout();
    tagLayout->setAlignment(Qt::AlignCenter);

    // Ajout des champs de texte et leur label
    // Options
    auto* option1VBox = new QVBoxLayout();
    option1VBox->setAlignment(Qt::AlignCenter);
    option1VBox->addWidget(new QLabel("Position"));
    option1VBox->addWidget(m_pPosStepEdit);
    optionsLayout->addLayout(option1VBox);
    auto* option2VBox = new QVBoxLayout();
    option2VBox->addWidget(new QLabel("Rotation"));
    option2VBox->addWidget(m_pRotationStepEdit);
    optionsLayout->addLayout(option2VBox);

    // Position
    auto* xPosHBox = new QHBoxLayout();
    xPosHBox->setAlignment(Qt::AlignCenter);
    xPosHBox->addWidget(new QLabel("X"));
    xPosHBox->addWidget(m_pXPositionEdit);
    positionLayout->addLayout(xPosHBox);
    auto* yPosHBox = new QHBoxLayout();
    yPosHBox->setAlignment(Qt::AlignCenter);
    yPosHBox->addWidget(new QLabel("Y"));
    yPosHBox->addWidget(m_pYPositionEdit);
    positionLayout->addLayout(yPosHBox);
    auto* zPosHBox = new QHBoxLayout();
    zPosHBox->setAlignment(Qt::AlignCenter);
    zPosHBox->addWidget(new QLabel("Z"));
    zPosHBox->addWidget(m_pZPositionEdit);
    positionLayout->addLayout(zPosHBox);

    // Taille
    scaleLayout->addWidget(m_pScaleEdit);

    // Rotation
    rotationLayout->addWidget(m_pRotationEdit);

    // Opacité
    opacityLayout->addWidget(m_pOpacityEdit);

    // Tag
    tagLayout->addWidget(m_pSetTagButton);

    // Ajout des layouts dans les groupes
    auto* optionsGroup = new QGroupBox("Options d'intervalles");
    optionsGroup->setLayout(optionsLayout);
    auto* positionGroup = new QGroupBox("Position");
    positionGroup->setLayout(positionLayout);
    auto* scaleGroup = new QGroupBox("Taille");
    scaleGroup->setLayout(scaleLayout);
    auto* rotationGroup = new QGroupBox("Rotation");
    rotationGroup->setLayout(rotationLayout);
    auto* opacityGroup = new QGroupBox("Opacité");
    opacityGroup->setLayout(opacityLayout);
    auto* tagGroup = new QGroupBox("Tags");
    tagGroup->setLayout(tagLayout);

    // Ajout des groupes dans le layout principal
    mainLayout->addWidget(optionsGroup);
    mainLayout->addWidget(positionGroup);
    mainLayout->addWidget(scaleGroup);
    mainLayout->addWidget(rotationGroup);
    mainLayout->addWidget(opacityGroup);
    mainLayout->addWidget(tagGroup);
}

//! Initialise les champs du panneau.
void SpriteDetailsPanel::initInputs() {
    // Création des champs de texte

    // Création et setup des champs d'options
    m_pPosStepEdit = new QComboBox();
    m_pPosStepEdit->addItems(PIXEL_STEPS);
    m_pPosStepEdit->setStyleSheet(GameFramework::loadStyleSheetString("comboboxStyle.qss"));
    m_pRotationStepEdit = new QComboBox();
    m_pRotationStepEdit->addItems(ROTATION_STEPS);
    m_pRotationStepEdit->setStyleSheet(GameFramework::loadStyleSheetString("comboboxStyle.qss"));

    // Creation et setup des champs de position
    m_pXPositionEdit = new QSpinBox();
    m_pXPositionEdit->setRange(0, 10000);
    m_pXPositionEdit->setSingleStep(10);
    m_pXPositionEdit->setSuffix(" px");
    m_pXPositionEdit->setStyleSheet(GameFramework::loadStyleSheetString("spinboxStyle.qss"));
    m_pYPositionEdit = new QSpinBox();
    m_pYPositionEdit->setRange(0, 10000);
    m_pYPositionEdit->setSingleStep(10);
    m_pYPositionEdit->setSuffix(" px");
    m_pYPositionEdit->setStyleSheet(GameFramework::loadStyleSheetString("spinboxStyle.qss"));
    m_pZPositionEdit = new QSpinBox();
    m_pZPositionEdit->setRange(0, 10000);
    m_pZPositionEdit->setSingleStep(1);
    m_pZPositionEdit->setStyleSheet(GameFramework::loadStyleSheetString("spinboxStyle.qss"));

    // Creation et setup des champs de taille
    m_pScaleEdit = new QDoubleSpinBox();
    m_pScaleEdit->setRange(.05, 5);
    m_pScaleEdit->setStyleSheet(GameFramework::loadStyleSheetString("spinboxStyle.qss"));

    // Creation et setup du champ de rotation
    m_pRotationEdit = new QSpinBox();
    m_pRotationEdit->setRange(-1000, 1000);
    m_pRotationEdit->setSingleStep(10);
    m_pRotationEdit->setSuffix("°");
    m_pRotationEdit->setStyleSheet(GameFramework::loadStyleSheetString("spinboxStyle.qss"));

    // Création et setup du champ d'opacité
    m_pOpacityEdit = new QSpinBox();
    m_pOpacityEdit->setRange(0, 100);
    m_pOpacityEdit->setSingleStep(10);
    m_pOpacityEdit->setSuffix("%");
    m_pOpacityEdit->setStyleSheet(GameFramework::loadStyleSheetString("spinboxStyle.qss"));
    
    // Création et setup button d'édition des tags
    m_pSetTagButton = new QPushButton("Éditer les tags");
    m_pSetTagButton->setStyleSheet(GameFramework::loadStyleSheetString("buttonStyle.qss"));
}

//! Connecte les signaux aux slots.
void SpriteDetailsPanel::connectInputSignals() {
    // Connecter les signaux de modification des champs d'options
    connect(m_pPosStepEdit, &QComboBox::currentIndexChanged, this, &SpriteDetailsPanel::onPosStepChanged);
    connect(m_pRotationStepEdit, &QComboBox::currentIndexChanged   , this, &SpriteDetailsPanel::onRotationStepChanged);

    // Connecter les signaux de modification des champs de position
    connect(m_pXPositionEdit, &QSpinBox::valueChanged, this, &SpriteDetailsPanel::onXPosFieldEdited);
    connect(m_pYPositionEdit, &QSpinBox::valueChanged, this, &SpriteDetailsPanel::onYPosFieldEdited);
    connect(m_pZPositionEdit, &QSpinBox::valueChanged, this, &SpriteDetailsPanel::onZPosFieldEdited);

    // Connecter les signaux de modification des champs de taille
    connect(m_pScaleEdit, &QDoubleSpinBox::valueChanged, this, &SpriteDetailsPanel::onScaleFieldEdited);

    // Connecter le signal de modification du champ de rotation
    connect(m_pRotationEdit, &QSpinBox::valueChanged, this, &SpriteDetailsPanel::onRotationFieldEdited);

    // Connecter le signal de modification du champ d'opacité
    connect(m_pOpacityEdit, &QSpinBox::valueChanged, this, &SpriteDetailsPanel::onOpacityFieldEdited);

    // Connecter le signal de clic de bouton d'édition des tags
    connect(m_pSetTagButton, &QPushButton::clicked, this, &SpriteDetailsPanel::onEditTagsButtonClicked);
}

/*******************************
 * Slots
 *******************************/

//! Appelé lorsque la valeur d'intervalle de position est modifiée.
void SpriteDetailsPanel::onPosStepChanged(int newStepIndex) {
    // Mettre à jour l'intervalle du champs de position
    int newStep = PIXEL_STEPS[newStepIndex].split(" ")[0].toInt();
    m_pXPositionEdit->setSingleStep(newStep);
    m_pYPositionEdit->setSingleStep(newStep);
}

//! Appelé lorsque la valeur d'intervalle de rotation est modifiée.
void SpriteDetailsPanel::onRotationStepChanged(int newStepIndex) {
    // Mettre à jour l'intervalle du champs de rotation
    m_pRotationEdit->setSingleStep(ROTATION_STEPS[newStepIndex].split("°")[0].toInt());
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
        m_pRotationEdit->setValue(value);
    } else if (value >= 360) {
        value = value - 360;
        m_pRotationEdit->setValue(value);
    }

    m_pEditorManager->setEditorSpriteRotation(m_pSprite, value);
}

//! Appelé lorsque l'opacité du sprite est modifiée.
//! \param value La nouvelle valeur d'opacité.
void SpriteDetailsPanel::onOpacityFieldEdited(int value) {
    if (m_pSprite == nullptr || m_ignoreFieldEdited) // Si le sprite est nul ou qu'on ignore les changements
        // On ne fait rien
        return;

    m_pEditorManager->setEditorSpriteOpacity(m_pSprite, value / 100.0);
}

//! Appelé lorsque le bouton d'édition des tags est cliqué.
void SpriteDetailsPanel::onEditTagsButtonClicked() {
    // Ouvrir un dialogue de modification des tags
    TagEditDialog dialog(m_pSprite, this);
    dialog.exec();
}
