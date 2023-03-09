/**
 * @file EditorActionPanel.cpp
 * @brief Définition de la classe EditorActionPanel.
 * @author Noah Blattner
 * @date Janvier 2023
 */

#include "EditorActionPanel.h"
#include "EditorManager.h"
#include "resources.h"
#include "GameScene.h"
#include "GameCore.h"
#include <QPushButton>
#include <QCheckBox>
#include <QLabel>
#include <QSpinBox>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QGroupBox>

EditorActionPanel::EditorActionPanel(QWidget* pParent) : QWidget(pParent){
    // Initialisation des boutons
    initInputs();

    // Initialisation du layout
    initLayout();

    // Connexion des signaux
    connectSignals();
}

//! Initialise le UI avec le manager d'éditeur
//! \param editorManager Le manager d'éditeur à lier à l'UI
void EditorActionPanel::bindEditorManager(EditorManager *editorManager) {
    m_pEditorManager = editorManager;
}

/*********************
 * Initialisation
 ********************/

//! Initialise le layout
void EditorActionPanel::initLayout() {
    auto* layoutActionsCreation = new QVBoxLayout();
    layoutActionsCreation->addWidget(addButton);
    layoutActionsCreation->addWidget(duplicateButton);
    layoutActionsCreation->addWidget(removeButton);

    auto* layoutActionsHistorique = new QVBoxLayout();
    layoutActionsHistorique->addWidget(undoButton);
    layoutActionsHistorique->addWidget(redoButton);

    auto* layoutActionsSelection = new QVBoxLayout();
    layoutActionsSelection->addWidget(selectAllButton);
    layoutActionsSelection->addWidget(deselectAllButton);

    auto* layoutActionsScene = new QVBoxLayout();
    layoutActionsScene->addWidget(addBackgroundButton);
    layoutActionsScene->addWidget(removeBackgroundButton);
    layoutActionsScene->addWidget(editSceneButton);

    // Layout des options
    auto* optionsLayout = new QVBoxLayout();
    optionsLayout->addWidget(snapToSpritesCheckBox);
    optionsLayout->addWidget(alignToGridCheckBox);
    auto* gridCellSizeLayout = new QHBoxLayout();
    gridCellSizeLayout->addWidget(gridCellSizeSpinBox);
    gridCellSizeLayout->addWidget(new QLabel("Taille de la grille"));
    optionsLayout->addLayout(gridCellSizeLayout);

    auto* layoutActionsFichier = new QVBoxLayout();
    layoutActionsFichier->addWidget(saveButton);
    layoutActionsFichier->addWidget(loadButton);
    layoutActionsFichier->addWidget(importButton);

    // Ajout des layouts dans les groupes de boutons
    auto* groupeActionsScene = new QGroupBox("Scène");
    groupeActionsScene->setLayout(layoutActionsScene);
    auto* groupeActionsCreation = new QGroupBox("Création et suppression");
    groupeActionsCreation->setLayout(layoutActionsCreation);
    groupeActionsCreation -> setStyleSheet(GameFramework::loadStyleSheetString("groupboxStyle.qss"));
    auto* groupeActionsHistorique = new QGroupBox("Historique");
    groupeActionsHistorique->setLayout(layoutActionsHistorique);
    groupeActionsHistorique -> setStyleSheet(GameFramework::loadStyleSheetString("groupboxStyle.qss"));
    auto* groupeActionsSelection = new QGroupBox("Sélection");
    groupeActionsSelection->setLayout(layoutActionsSelection);
    groupeActionsSelection -> setStyleSheet(GameFramework::loadStyleSheetString("groupboxStyle.qss"));
    auto* groupeOptions = new QGroupBox("Options");
    groupeOptions->setLayout(optionsLayout);
    groupeOptions -> setStyleSheet(GameFramework::loadStyleSheetString("groupboxStyle.qss"));
    auto* groupeActionsFichier = new QGroupBox("Fichier");
    groupeActionsFichier->setLayout(layoutActionsFichier);
    groupeActionsFichier -> setStyleSheet(GameFramework::loadStyleSheetString("groupboxStyle.qss"));

    // Création du layout
    mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);
    // Ajout des groupes dans le layout principal
    mainLayout->addWidget(groupeActionsFichier);
    mainLayout->addStretch();
    mainLayout->addWidget(groupeActionsCreation);
    mainLayout->addWidget(groupeActionsHistorique);
    mainLayout->addWidget(groupeActionsSelection);
    mainLayout->addWidget(groupeActionsScene);
    mainLayout->addWidget(groupeOptions);
}

//! Initialise les boutons
void EditorActionPanel::initInputs() {
    // Création du bouton d'ajout de sprite
    addButton = new QPushButton(QIcon(GameFramework::imagesPath() + "icons/addIcon.png"), "Ajouter un sprite");
    addButton->setToolTip("Ajouter un sprite");
    addButton->setStyleSheet(GameFramework::loadStyleSheetString("buttonStyle.qss"));

    // Création d'un bouton de duplication de sprite
    duplicateButton = new QPushButton(QIcon(GameFramework::imagesPath() + "icons/duplicateIcon.png"), "Dupliquer les sprites sélectionnés");
    duplicateButton->setToolTip("Dupliquer les sprites sélectionnés");
    duplicateButton->setStyleSheet(GameFramework::loadStyleSheetString("buttonStyle.qss"));

    // Création d'un bouton de suppression de sprite
    removeButton = new QPushButton(QIcon(GameFramework::imagesPath() + "icons/deleteIcon.png"), "Supprimer les sprites sélectionnés");
    removeButton->setToolTip("Supprimer les sprites sélectionnés");
    removeButton->setStyleSheet(GameFramework::loadStyleSheetString("buttonStyle.qss"));

    // Création d'un bouton d'annulation de la dernière action
    undoButton = new QPushButton(QIcon(GameFramework::imagesPath() + "icons/undoIcon.png"), "Annuler la dernière action");
    undoButton->setToolTip("Annuler la dernière action");
    undoButton->setStyleSheet(GameFramework::loadStyleSheetString("buttonStyle.qss"));

    // Création d'un bouton de rétablissement de la dernière action annulée
    redoButton = new QPushButton(QIcon(GameFramework::imagesPath() + "icons/redoIcon.png"), "Rétablir la dernière action annulée");
    redoButton->setToolTip("Rétablir la dernière action annulée");
    redoButton->setStyleSheet(GameFramework::loadStyleSheetString("buttonStyle.qss"));

    // Création d'un bouton de sélection de tous les sprites
    selectAllButton = new QPushButton(QIcon(GameFramework::imagesPath() + "icons/selectAllIcon.png"), "Sélectionner tous les sprites");
    selectAllButton->setToolTip("Sélectionner tous les sprites");
    selectAllButton->setStyleSheet(GameFramework::loadStyleSheetString("buttonStyle.qss"));

    // Création d'un bouton de déselection de tous les sprites
    deselectAllButton = new QPushButton(QIcon(GameFramework::imagesPath() + "icons/deselectIcon.png"), "Désélectionner tous les sprites");
    deselectAllButton->setToolTip("Désélectionner tous les sprites");
    deselectAllButton->setStyleSheet(GameFramework::loadStyleSheetString("buttonStyle.qss"));

    // Création d'un bouton d'ajout d'arrière-plan
    addBackgroundButton = new QPushButton(QIcon(GameFramework::imagesPath() + "icons/backgroundIcon.png"), "Ajouter un arrière-plan");
    addBackgroundButton->setToolTip("Ajouter un arrière-plan à la scène");
    addBackgroundButton->setStyleSheet(GameFramework::loadStyleSheetString("buttonStyle.qss"));

    // Création d'un bouton de suppression d'arrière-plan
    removeBackgroundButton = new QPushButton(QIcon(GameFramework::imagesPath() + "icons/removeBackgroundIcon.png"), "Supprimer l'arrière-plan");
    removeBackgroundButton->setToolTip("Supprimer l'arrière-plan de la scène");
    removeBackgroundButton->setStyleSheet(GameFramework::loadStyleSheetString("buttonStyle.qss"));

    // Création du bouton d'édition de la scène
    editSceneButton = new QPushButton(QIcon(GameFramework::imagesPath() + "icons/editIcon.png"), "Changer la taille");
    editSceneButton->setToolTip("Changer la taille de la scène");
    editSceneButton->setStyleSheet(GameFramework::loadStyleSheetString("buttonStyle.qss"));

    // Création de la checkbox d'alignement aux sprites
    snapToSpritesCheckBox = new QCheckBox("Aligner aux sprites");
    snapToSpritesCheckBox->setToolTip("Aligner aux sprites");
    snapToSpritesCheckBox->setStyleSheet(GameFramework::loadStyleSheetString("checkboxStyle.qss"));

    // Création de la checkbox d'alignement à la grille
    alignToGridCheckBox = new QCheckBox("Utiliser la grille");
    alignToGridCheckBox->setToolTip("Aligner les sprites à la grille");
    alignToGridCheckBox->setStyleSheet(GameFramework::loadStyleSheetString("checkboxStyle.qss"));

    // Création du champs de texte pour la taille de la grille
    gridCellSizeSpinBox = new QSpinBox();
    gridCellSizeSpinBox->setToolTip("Taille de la grille");
    gridCellSizeSpinBox -> setRange(1, 2000);
    gridCellSizeSpinBox ->setValue(50);
    gridCellSizeSpinBox->setEnabled(false);
    gridCellSizeSpinBox->setStyleSheet(GameFramework::loadStyleSheetString("spinboxStyle.qss"));

    // Création du bouton de sauvegarde
    saveButton = new QPushButton(QIcon(GameFramework::imagesPath() + "icons/saveIcon.png"), "Sauvegarder");
    saveButton->setToolTip("Sauvegarder");
    saveButton->setStyleSheet(GameFramework::loadStyleSheetString("buttonStyle.qss"));

    // Création du bouton de chargement
    loadButton = new QPushButton(QIcon(GameFramework::imagesPath() + "icons/loadIcon.png"), "Charger");
    loadButton->setToolTip("Charger");
    loadButton->setStyleSheet(GameFramework::loadStyleSheetString("buttonStyle.qss"));

    // Création du bouton d'importation
    importButton = new QPushButton(QIcon(GameFramework::imagesPath() + "icons/importIcon.png"), "Importer");
    importButton->setToolTip("Importer");
    importButton->setStyleSheet(GameFramework::loadStyleSheetString("buttonStyle.qss"));
}

//! Connexion des signaux des boutons avec les slots correspondants
void EditorActionPanel::connectSignals() const {
    // Connexion les signaux des boutons
    connect(snapToSpritesCheckBox, &QCheckBox::stateChanged, this, &EditorActionPanel::snapToSpritesCheckBoxStateChanged);
    connect(alignToGridCheckBox, &QCheckBox::stateChanged, this, &EditorActionPanel::alignToGridCheckBoxStateChanged);
    connect(gridCellSizeSpinBox, QOverload<int>::of(&QSpinBox::valueChanged), this, &::EditorActionPanel::girdCellSizeSpinBoxValueChanged);
    connect(editSceneButton, &QPushButton::clicked, this, &EditorActionPanel::editSceneButtonClicked);
    connect(addButton, &QPushButton::clicked, this, &EditorActionPanel::addButtonClicked);
    connect(removeButton, &QPushButton::clicked, this, &EditorActionPanel::deleteButtonClicked);
    connect(undoButton, &QPushButton::clicked, this, &EditorActionPanel::undoButtonClicked);
    connect(redoButton, &QPushButton::clicked, this, &EditorActionPanel::redoButtonClicked);
    connect(selectAllButton, &QPushButton::clicked, this, &EditorActionPanel::selectAllButtonClicked);
    connect(deselectAllButton, &QPushButton::clicked, this, &EditorActionPanel::deselectAllSprites);
    connect(duplicateButton, &QPushButton::clicked, this, &EditorActionPanel::duplicateButtonClicked);
    connect(addBackgroundButton, &QPushButton::clicked, this, &EditorActionPanel::addBackgroundButtonClicked);
    connect(removeBackgroundButton, &QPushButton::clicked, this, &EditorActionPanel::removeBackgroundButtonClicked);
    connect(saveButton, &QPushButton::clicked, this, &EditorActionPanel::saveButtonClicked);
    connect(loadButton, &QPushButton::clicked, this, &EditorActionPanel::loadButtonClicked);
    connect(importButton, &QPushButton::clicked, this, &EditorActionPanel::importButtonClicked);
}

/*********************
 * Gestion des signaux des inputs
 ********************/

//! Slot appelé lors du clic sur le bouton de sauvegarde
void EditorActionPanel::saveButtonClicked() {
    m_pEditorManager->save("");
}

//! Slot appelé lors du clic sur le bouton de chargement
void EditorActionPanel::loadButtonClicked() {
    m_pEditorManager->load("");
}

//! Slot appelé lors du clic sur le bouton d'importation
void EditorActionPanel::importButtonClicked() {
    m_pEditorManager->import("");
}

//! Slot appelé lors du clic sur le bouton d'ajout de sprite
void EditorActionPanel::addButtonClicked() {
    m_pEditorManager->createNewEditorSprite();
}

//! Slot appelé lors du clic sur le bouton de suppression de sprite
void EditorActionPanel::deleteButtonClicked() {
    // Suppression des sprites sélectionnés
    m_pEditorManager->deleteSelectedEditorSprites();
}

//! Slot appelé lors du clic sur le bouton d'annulation de la dernière action
void EditorActionPanel::undoButtonClicked() {
    // Rétablir la dernière action annulée
    m_pEditorManager->undo();
}

//! Slot appelé lors du clic sur le bouton de rétablissement de la dernière action annulée
void EditorActionPanel::redoButtonClicked() {
    // Annuler la dernière action
    m_pEditorManager->redo();
}

//! Slot appelé lors du clic sur le bouton de sélection de tous les sprites
void EditorActionPanel::selectAllButtonClicked() {
    // Sélectionner tous les sprites
    m_pEditorManager->selectAllEditorSprites();
}

//! Slot appelé lors du clic sur le bouton de déselection de tous les sprites
void EditorActionPanel::deselectAllSprites() {
    m_pEditorManager->unselectAllEditorSprites();
}

//! Slot appelé lors du clic sur le bouton de duplication de sprite
void EditorActionPanel::duplicateButtonClicked() {
    // Dupliquer les sprites sélectionnés
    m_pEditorManager->duplicateSelectedEditorSprites();
}

//! Slot appelé lors du clic sur le bouton d'ajout d'arrière-plan
void EditorActionPanel::addBackgroundButtonClicked() {
    m_pEditorManager->setBackGroundImage();
}

//! Slot appelé lors du clic sur le bouton de suppression d'arrière-plan
void EditorActionPanel::removeBackgroundButtonClicked() {
    m_pEditorManager->removeBackGroundImage();
}

//! Slot appelé lors du clic sur le bouton d'édition de la scène
void EditorActionPanel::editSceneButtonClicked() {
    // Ouverture de la fenêtre d'édition de la scène
    m_pEditorManager->showSceneEditDialog();
}

//! Slot appelé lors du changement de l'état de la case à cocher de l'alignement aux sprites
void EditorActionPanel::snapToSpritesCheckBoxStateChanged(int state) {
    m_pEditorManager->setSnapEnabled(state == Qt::Checked);

    // Si l'alignement aux sprites est activé, on désactive l'alignement à la grille
    if (state == Qt::Checked) {
        alignToGridCheckBox->setChecked(false);
        m_pEditorManager->setGridEnabled(false);
    }
}

//! Slot appelé lors du changement de l'état de la case à cocher d'alignement à la grille
void EditorActionPanel::alignToGridCheckBoxStateChanged(int state) {
    m_pEditorManager->setGridEnabled(state == Qt::Checked);

    // Si l'alignement à la grille est activé, on désactive l'alignement aux sprites
    if (state == Qt::Checked) {
        snapToSpritesCheckBox->setChecked(false);
        m_pEditorManager->setSnapEnabled(false);
    }

    // Activation ou désactivation de la taille des cellules de la grille
    gridCellSizeSpinBox->setEnabled(state == Qt::Checked);
}

//! Slot appelé lors du changement de la taille des cellules de la grille
void EditorActionPanel::girdCellSizeSpinBoxValueChanged(int value) {
    m_pEditorManager->setGridCellSize(value);
}