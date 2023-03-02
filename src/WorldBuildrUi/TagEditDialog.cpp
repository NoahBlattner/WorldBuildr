//
// Created by blatnoa on 02.03.2023.
//

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QLabel>
#include <QPushButton>
#include <QInputDialog>
#include <QErrorMessage>
#include <QMessageBox>
#include "EditorManager.h"
#include "EditorSprite.h"
#include "TagsManager.h"
#include "resources.h"

#include "TagEditDialog.h"

TagEditDialog::TagEditDialog(EditorSprite* sprite, QWidget* parent) : QDialog(parent) {
    m_pSprite = sprite;

    initInputs();
    initLayout();

    setWindowTitle("Edit tags");
}

//! Initialise les layouts
void TagEditDialog::initLayout() {
    // Création des layouts
    auto* mainLayout = new QHBoxLayout();
    mainLayout->setAlignment(Qt::AlignCenter);

    // Tag list layout
    auto* tagListLayout = new QVBoxLayout();
    tagListLayout->setAlignment(Qt::AlignCenter);

    // Button layout
    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->setAlignment(Qt::AlignCenter);

    // Ajouter les boutons
    buttonLayout->addWidget(addTagButton);
    buttonLayout->addWidget(removeTagButton);

    // Ajout des composants dans le layout de tag
    tagListLayout->addLayout(buttonLayout);
    tagListLayout->addWidget(tagList);

    auto* currentTagLayout = new QVBoxLayout();
    currentTagLayout->setAlignment(Qt::AlignCenter);

    currentTagLayout->addWidget(new QLabel("Current tag:"));
    currentTagLayout->addWidget(m_pCurrentTagEdit);
    currentTagLayout->addWidget(applyTagButton);
    currentTagLayout->addWidget(clearTagButton);

    // Ajout du layout dans le layout principal
    mainLayout->addLayout(tagListLayout);
    mainLayout->addLayout(currentTagLayout);

    // Ajout du layout principal au dialog
    setLayout(mainLayout);
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
    addTagButton = new QPushButton("Add");
    removeTagButton = new QPushButton("Remove");
    applyTagButton = new QPushButton("Choose selected tag");
    clearTagButton = new QPushButton("Choose no tag");

    // Style des boutons
    QString buttonStyle = GameFramework::loadStyleSheetString("buttonStyle.qss");
    addTagButton->setStyleSheet(buttonStyle);
    removeTagButton->setStyleSheet(buttonStyle);
    applyTagButton->setStyleSheet(buttonStyle);
    clearTagButton->setStyleSheet(buttonStyle);

    // Connexion des signaux
    connect(tagList, &QListWidget::itemDoubleClicked, this, &TagEditDialog::onTagListItemDoubleClicked);

    connect(addTagButton, &QPushButton::clicked, this, &TagEditDialog::onAddTagButtonClicked);
    connect(removeTagButton, &QPushButton::clicked, this, &TagEditDialog::onDeleteTagButtonClicked);
    connect(applyTagButton, &QPushButton::clicked, this, &TagEditDialog::onSelectButtonClicked);
}

//! Met à jour le tag du sprite. Met également à jour le QLineEdit
//! \param tag Nouveau tag
void TagEditDialog::setTagOnSprite(const QString &tag) {
    m_pSprite->setTag(tag);
    m_pCurrentTagEdit->setText(tag);
}

//! Met à jour la liste des tags
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

//! Slot appelé lors du double clic sur un tag
void TagEditDialog::onTagListItemDoubleClicked(QListWidgetItem *item) {
    setTagOnSprite(item->text());
}

//! Slot appelé lors du clic sur le bouton d'ajout
void TagEditDialog::onAddTagButtonClicked() {
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
void TagEditDialog::onDeleteTagButtonClicked() {
    QList<QListWidgetItem*> selectedItems = tagList->selectedItems();

    if (selectedItems.isEmpty()) { // Si aucun tag n'est sélectionné
        return;
    }

    // Suppression du tag
    TagsManager::removeTag(selectedItems.first()->text());

    updateList();
}

//! Slot appelé lors du clic sur le bouton de sélection de tag
void TagEditDialog::onSelectButtonClicked() {
    QList<QListWidgetItem*> selectedItems = tagList->selectedItems();

    if (selectedItems.isEmpty()) { // Si aucun tag n'est sélectionné
        return;
    }

    // Sélection du tag
    setTagOnSprite(selectedItems.first()->text());
}

//! Slot appelé lors du clic sur le bouton de désélection de tag
void TagEditDialog::onNoTagButtonClicked() {
    // Suppression du tag du sprite
    m_pSprite->removeTag();
}

