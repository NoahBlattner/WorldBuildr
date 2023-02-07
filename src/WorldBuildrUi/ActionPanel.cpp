//
// Created by blatnoa on 01.02.2023.
//

#include "ActionPanel.h"
#include "EditorManager.h"
#include "resources.h"
#include "GameScene.h"
#include "GameCore.h"
#include <QPushButton>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QGroupBox>

ActionPanel::ActionPanel(QWidget* pParent) : QWidget(pParent){
    // Création du layout
    mainLayout = new QVBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    // Initialisation des boutons
    initButtons();

    // Initialisation du layout
    initLayout();

    // Connexion des signaux
    connectSignals();
}

//! Initialise le UI avec le manager d'éditeur
//! \param editorManager Le manager d'éditeur à lier à l'UI
void ActionPanel::bindEditorManager(EditorManager *editorManager) {
    m_pEditorManager = editorManager;
}

/*********************
 * Initialisation
 ********************/

//! Initialise le layout
void ActionPanel::initLayout() {
    // Grouper les boutons
    layoutActionsCreation->addWidget(addButton);
    layoutActionsCreation->addWidget(duplicateButton);
    layoutActionsCreation->addWidget(removeButton);

    layoutActionsSelection->addWidget(selectAllButton);
    layoutActionsSelection->addWidget(deselectAllButton);

    layoutActionsFond->addWidget(addBackgroundButton);
    layoutActionsFond->addWidget(removeBackgroundButton);

    // Ajout des layouts dans les groupes de boutons
    groupeActionsCreation->setLayout(layoutActionsCreation);
    groupeActionsSelection->setLayout(layoutActionsSelection);
    groupeActionsAutre->setLayout(layoutActionsFond);

    // Ajout des groupes de boutons dans le layout principal
    mainLayout->addWidget(groupeActionsCreation);
    mainLayout->addWidget(groupeActionsSelection);
    mainLayout->addWidget(groupeActionsAutre);
}

//! Initialise les boutons
void ActionPanel::initButtons() {
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
    addBackgroundButton->setToolTip("Ajouter un arrière-plan");
    addBackgroundButton->setStyleSheet(GameFramework::loadStyleSheetString("buttonStyle.qss"));

    // Création d'un bouton de suppression d'arrière-plan
    removeBackgroundButton = new QPushButton(QIcon(GameFramework::imagesPath() + "icons/removeBackgroundIcon.png"), "Supprimer l'arrière-plan");
    removeBackgroundButton->setToolTip("Supprimer l'arrière-plan");
    removeBackgroundButton->setStyleSheet(GameFramework::loadStyleSheetString("buttonStyle.qss"));
}

//! Connexion des signaux des boutons avec les slots correspondants
void ActionPanel::connectSignals() const {
    // Connexion les signaux des boutons
    connect(addButton, &QPushButton::clicked, this, &ActionPanel::addButtonClicked);
    connect(removeButton, &QPushButton::clicked, this, &ActionPanel::deleteButtonClicked);
    connect(selectAllButton, &QPushButton::clicked, this, &ActionPanel::selectAllButtonClicked);
    connect(deselectAllButton, &QPushButton::clicked, this, &ActionPanel::deselectAllSprites);
    connect(duplicateButton, &QPushButton::clicked, this, &ActionPanel::duplicateButtonClicked);
    connect(addBackgroundButton, &QPushButton::clicked, this, &ActionPanel::addBackgroundButtonClicked);
    connect(removeBackgroundButton, &QPushButton::clicked, this, &ActionPanel::removeBackgroundButtonClicked);
}

/*********************
 * Gestion des signaux des boutons
 ********************/

//! Slot appelé lors du clic sur le bouton d'ajout de sprite
void ActionPanel::addButtonClicked() {
    m_pEditorManager->createNewEditorSprite();
}

//! Slot appelé lors du clic sur le bouton de suppression de sprite
void ActionPanel::deleteButtonClicked() {
    // Suppression des sprites sélectionnés
    m_pEditorManager->deleteSelectedEditorSprites();
}

//! Slot appelé lors du clic sur le bouton de sélection de tous les sprites
void ActionPanel::selectAllButtonClicked() {
    // Sélectionner tous les sprites
    m_pEditorManager->selectAllEditorSprites();
}

//! Slot appelé lors du clic sur le bouton de déselection de tous les sprites
void ActionPanel::deselectAllSprites() {
    m_pEditorManager->unselectAllEditorSprites();
}

//! Slot appelé lors du clic sur le bouton de duplication de sprite
void ActionPanel::duplicateButtonClicked() {
    // Dupliquer les sprites sélectionnés
    m_pEditorManager->duplicateSelectedEditorSprites();
}

//! Slot appelé lors du clic sur le bouton d'ajout d'arrière-plan
void ActionPanel::addBackgroundButtonClicked() {
    m_pEditorManager->setBackGroundImage();
}

//! Slot appelé lors du clic sur le bouton de suppression d'arrière-plan
void ActionPanel::removeBackgroundButtonClicked() {
    m_pEditorManager->removeBackGroundImage();
}