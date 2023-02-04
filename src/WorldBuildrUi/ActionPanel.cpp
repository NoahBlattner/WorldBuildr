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

//! Copie une image sélectionnée dans le dossier d'images de l'éditeur
//! et retourne le chemin de la nouvelle image
//! \return Le chemin de l'image
QString ActionPanel::loadImageToEditor() {
    // Ouverture d'une boîte de dialogue pour charger une image
    QString imagePath = QFileDialog::getOpenFileName(this, tr("Load Image"), GameFramework::imagesPath(), tr("Image Files (*.png *.jpg *.bmp)"));
    QString newImagePath = GameFramework::imagesPath() + "editorImages/" + imagePath.split("/").last();
    QFile::copy(imagePath, newImagePath);

    return newImagePath;
}

/*********************
 * Gestion des signaux des boutons
 ********************/

//! Slot appelé lors du clic sur le bouton d'ajout de sprite
void ActionPanel::addButtonClicked() {
    QString imagePath = loadImageToEditor();

    if(!imagePath.isEmpty()){ // Si l'image n'est pas vide
        // Création d'un nouveau sprite
        m_pEditorManager->createEditorSprite(imagePath);
    }
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

