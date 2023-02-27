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
#include <QVBoxLayout>
#include <QFileDialog>
#include <QGroupBox>

EditorActionPanel::EditorActionPanel(QWidget* pParent) : QWidget(pParent){
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
void EditorActionPanel::bindEditorManager(EditorManager *editorManager) {
    m_pEditorManager = editorManager;
}

/*********************
 * Initialisation
 ********************/

//! Initialise le layout
void EditorActionPanel::initLayout() {
    // Grouper les boutons
    layoutActionsCreation->addWidget(addButton);
    layoutActionsCreation->addWidget(duplicateButton);
    layoutActionsCreation->addWidget(removeButton);

    layoutActionsHistorique->addWidget(undoButton);
    layoutActionsHistorique->addWidget(redoButton);

    layoutActionsSelection->addWidget(selectAllButton);
    layoutActionsSelection->addWidget(deselectAllButton);

    layoutActionsFond->addWidget(addBackgroundButton);
    layoutActionsFond->addWidget(removeBackgroundButton);

    // Ajout des layouts dans les groupes de boutons
    groupeActionsCreation->setLayout(layoutActionsCreation);
    groupeActionsCreation -> setStyleSheet(GameFramework::loadStyleSheetString("groupboxStyle.qss"));
    groupeActionsHistorique->setLayout(layoutActionsHistorique);
    groupeActionsHistorique -> setStyleSheet(GameFramework::loadStyleSheetString("groupboxStyle.qss"));
    groupeActionsSelection->setLayout(layoutActionsSelection);
    groupeActionsSelection -> setStyleSheet(GameFramework::loadStyleSheetString("groupboxStyle.qss"));
    groupeActionsFond->setLayout(layoutActionsFond);
    groupeActionsFond -> setStyleSheet(GameFramework::loadStyleSheetString("groupboxStyle.qss"));

    // Ajout des groupes de boutons dans le layout principal
    mainLayout->addWidget(groupeActionsCreation);
    mainLayout->addWidget(groupeActionsHistorique);
    mainLayout->addWidget(groupeActionsSelection);
    mainLayout->addWidget(groupeActionsFond);
}

//! Initialise les boutons
void EditorActionPanel::initButtons() {
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
    addBackgroundButton->setToolTip("Ajouter un arrière-plan");
    addBackgroundButton->setStyleSheet(GameFramework::loadStyleSheetString("buttonStyle.qss"));

    // Création d'un bouton de suppression d'arrière-plan
    removeBackgroundButton = new QPushButton(QIcon(GameFramework::imagesPath() + "icons/removeBackgroundIcon.png"), "Supprimer l'arrière-plan");
    removeBackgroundButton->setToolTip("Supprimer l'arrière-plan");
    removeBackgroundButton->setStyleSheet(GameFramework::loadStyleSheetString("buttonStyle.qss"));
}

//! Connexion des signaux des boutons avec les slots correspondants
void EditorActionPanel::connectSignals() const {
    // Connexion les signaux des boutons
    connect(addButton, &QPushButton::clicked, this, &EditorActionPanel::addButtonClicked);
    connect(removeButton, &QPushButton::clicked, this, &EditorActionPanel::deleteButtonClicked);
    connect(undoButton, &QPushButton::clicked, this, &EditorActionPanel::undoButtonClicked);
    connect(redoButton, &QPushButton::clicked, this, &EditorActionPanel::redoButtonClicked);
    connect(selectAllButton, &QPushButton::clicked, this, &EditorActionPanel::selectAllButtonClicked);
    connect(deselectAllButton, &QPushButton::clicked, this, &EditorActionPanel::deselectAllSprites);
    connect(duplicateButton, &QPushButton::clicked, this, &EditorActionPanel::duplicateButtonClicked);
    connect(addBackgroundButton, &QPushButton::clicked, this, &EditorActionPanel::addBackgroundButtonClicked);
    connect(removeBackgroundButton, &QPushButton::clicked, this, &EditorActionPanel::removeBackgroundButtonClicked);
}

/*********************
 * Gestion des signaux des boutons
 ********************/

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