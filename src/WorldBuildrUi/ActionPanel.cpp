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

ActionPanel::ActionPanel(QWidget* pParent) : QWidget(pParent){
}

//! Initialise le UI avec le manager d'éditeur
//! \param editorManager Le manager d'éditeur à lier à l'UI
void ActionPanel::initUI(EditorManager *editorManager) {
    m_pEditorManager = editorManager;

    // Création du layout
    layout = new QVBoxLayout(this);
    layout->setAlignment(Qt::AlignCenter);

    // Création du bouton d'ajout de sprite
    addButton = new QPushButton(QIcon(GameFramework::imagesPath() + "icons/addIcon.png"), "Ajouter un sprite");
    addButton->setToolTip("Ajouter un sprite");
    addButton->setStyleSheet(GameFramework::loadStyleSheetString("buttonStyle.qss"));
    layout->addWidget(addButton);

    // Création d'un bouton de suppression de sprite
    removeButton = new QPushButton(QIcon(GameFramework::imagesPath() + "icons/deleteIcon.png"), "Supprimer les sprites sélectionnés");
    removeButton->setToolTip("Supprimer les sprites sélectionnés");
    removeButton->setStyleSheet(GameFramework::loadStyleSheetString("buttonStyle.qss"));
    layout->addWidget(removeButton);

    // Création d'un bouton de sélection de tous les sprites
    selectAllButton = new QPushButton(QIcon(GameFramework::imagesPath() + "icons/selectAllIcon.png"), "Sélectionner tous les sprites");
    selectAllButton->setToolTip("Sélectionner tous les sprites");
    selectAllButton->setStyleSheet(GameFramework::loadStyleSheetString("buttonStyle.qss"));
    layout->addWidget(selectAllButton);

    // Création d'un bouton de déselection de tous les sprites
    deselectAllButton = new QPushButton(QIcon(GameFramework::imagesPath() + "icons/deselectIcon.png"), "Désélectionner tous les sprites");
    deselectAllButton->setToolTip("Désélectionner tous les sprites");
    deselectAllButton->setStyleSheet(GameFramework::loadStyleSheetString("buttonStyle.qss"));
    layout->addWidget(deselectAllButton);

    // Création d'un bouton de duplication de sprite
    duplicateButton = new QPushButton(QIcon(GameFramework::imagesPath() + "icons/duplicateIcon.png"), "Dupliquer les sprites sélectionnés");
    duplicateButton->setToolTip("Dupliquer les sprites sélectionnés");
    duplicateButton->setStyleSheet(GameFramework::loadStyleSheetString("buttonStyle.qss"));
    layout->addWidget(duplicateButton);

    // Création d'un bouton de ajout d'arrière-plan
    addBackgroundButton = new QPushButton(QIcon(GameFramework::imagesPath() + "icons/backgroundIcon.png"), "Ajouter un arrière-plan");
    addBackgroundButton->setToolTip("Ajouter un arrière-plan");
    addBackgroundButton->setStyleSheet(GameFramework::loadStyleSheetString("buttonStyle.qss"));
    layout->addWidget(addBackgroundButton);

    // Connexion des signaux
    connect(addButton, &QPushButton::clicked, this, &ActionPanel::addButtonClicked);
    connect(removeButton, &QPushButton::clicked, this, &ActionPanel::deleteButtonClicked);
    connect(selectAllButton, &QPushButton::clicked, this, &ActionPanel::selectAllButtonClicked);
    connect(deselectAllButton, &QPushButton::clicked, this, &ActionPanel::deselectAllSprites);
    connect(duplicateButton, &QPushButton::clicked, this, &ActionPanel::duplicateButtonClicked);
    connect(addBackgroundButton, &QPushButton::clicked, this, &ActionPanel::addBackgroundButtonClicked);

    // TODO Autres boutons
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

