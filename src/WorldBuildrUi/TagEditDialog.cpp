//
// Created by blatnoa on 02.03.2023.
//

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QPushButton>
#include <QInputDialog>
#include <QErrorMessage>
#include <QMessageBox>
#include "EditorManager.h"
#include "EditorSprite.h"

#include "TagEditDialog.h"
#include "TagsManager.h"

TagEditDialog::TagEditDialog(EditorSprite* sprite, QWidget* parent) : QDialog(parent) {
    m_pSprite = sprite;

    initInputs();
    initLayout();

    setWindowTitle("Edit tags");
}

//! Initialise les layouts
void TagEditDialog::initLayout() {
    // Création des layouts
    auto* mainLayout = new QHBoxLayout(this);
    mainLayout->setAlignment(Qt::AlignCenter);

    // Tag list layout
    auto* tagListLayout = new QVBoxLayout(this);
    tagListLayout->setAlignment(Qt::AlignCenter);

    // Button layout
    auto* buttonLayout = new QHBoxLayout(this);
    buttonLayout->setAlignment(Qt::AlignCenter);

    // Ajouter les boutons
    buttonLayout->addWidget(addButton);
    buttonLayout->addWidget(removeButton);

    // Ajout des composants dans le layout de tag
    tagListLayout->addLayout(buttonLayout);
    tagListLayout->addWidget(tagList);

    auto* currentTagLayout = new QVBoxLayout(this);
    currentTagLayout->setAlignment(Qt::AlignRight);

    currentTagLayout->addWidget(m_pCurrentTagEdit);
    currentTagLayout->addWidget(selectButton);

    // Ajout du layout dans le layout principal
    mainLayout->addLayout(tagListLayout);
    mainLayout->addLayout(currentTagLayout);

    // Ajout du layout principal au dialog
    setLayout(mainLayout);

    // TODO : Fixer le crash causé par des layouts
}

//! Initialise les inputs
void TagEditDialog::initInputs() {
    // Tag list
    tagList = new QListWidget(this);
    tagList->setSelectionMode(QAbstractItemView::SelectionMode::SingleSelection);
    updateList();

    // Current tag edit
    m_pCurrentTagEdit = new QLineEdit(this);
    m_pCurrentTagEdit->setReadOnly(true);

    // Création des boutons
    addButton = new QPushButton("Add");
    removeButton = new QPushButton("Remove");
    selectButton = new QPushButton("Choose selected tag");

    // Connexion des signaux
    connect(addButton, &QPushButton::clicked, this, &TagEditDialog::onAddButtonClicked);
    connect(removeButton, &QPushButton::clicked, this, &TagEditDialog::onRemoveButtonClicked);
    connect(selectButton, &QPushButton::clicked, this, &TagEditDialog::onSelectButtonClicked);
}

// Met à jour la liste des tags
void TagEditDialog::updateList() {
    tagList->clear();

    // Ajout des tags
    QList<QString> tags = TagsManager::getTags();
    for (const QString& tag : tags) {
        tagList->addItem(tag);
    }

    // Tri de la liste
    tagList->sortItems();
}

/*****************************************
 * SLOTS
 *****************************************/

//! Slot appelé lors du clic sur le bouton d'ajout
void TagEditDialog::onAddButtonClicked() {
    QString newTag = QInputDialog::getText(this, "New tag", "Enter the name of the new tag");

    if (newTag.isEmpty()) { // Si le tag est vide
        return;
    }

    if (TagsManager::getTags().contains(newTag)) { // Si le tag existe déjà
        // Affichage d'un message d'erreur
        QMessageBox::critical(this, "Error", "This tag already exists");
        return;
    }

    // Ajout du tag
    TagsManager::addTag(newTag);

    updateList();
}

//! Slot appelé lors du clic sur le bouton de suppression
void TagEditDialog::onRemoveButtonClicked() {
    QList<QListWidgetItem*> selectedItems = tagList->selectedItems();

    if (selectedItems.isEmpty()) { // Si aucun tag n'est sélectionné
        return;
    }

    // Suppression du tag
    TagsManager::removeTag(selectedItems.first()->text());

    updateList();
}

//! Slot appelé lors du clic sur le bouton d'ok
void TagEditDialog::onSelectButtonClicked() {
    QList<QListWidgetItem*> selectedItems = tagList->selectedItems();

    if (selectedItems.isEmpty()) { // Si aucun tag n'est sélectionné
        return;
    }

    // Ajout du tag
    qDebug() << "Tag selected: " << selectedItems.first()->text();
    // TODO
    //m_pSprite->setTag(selectedItems.first()->text());
    // Fermeture de la fenêtre
    close();
}

