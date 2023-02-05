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

    // Connexion des signaux
    connect(addButton, &QPushButton::clicked, this, &ActionPanel::addButtonClicked);
    connect(removeButton, &QPushButton::clicked, this, &ActionPanel::deleteButtonClicked);
    connect(selectAllButton, &QPushButton::clicked, this, &ActionPanel::selectAllButtonClicked);

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

