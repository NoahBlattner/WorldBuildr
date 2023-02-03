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

//! Initialise le HUD avec le manager d'éditeur
//! \param editorManager Le manager d'éditeur à lier au HUD
void ActionPanel::initUI(EditorManager *editorManager) {
    m_pEditorManager = editorManager;

    // Création du layout
    layout = new QVBoxLayout(this);

    // Création du bouton d'ajout de sprite
    addButton = new QPushButton(QIcon(GameFramework::imagesPath() + "icons/addIcon.png"), "Ajouter un sprite");
    addButton->setToolTip("Ajouter un sprite");
    addButton->setStyleSheet(loadStyleSheet(GameFramework::stylePath() + "buttonStyle.qss"));
    layout->addWidget(addButton);

    // Connexion des signaux
    connect(addButton, &QPushButton::clicked, this, &ActionPanel::addButtonClicked);

    // TODO Autres boutons
}

//! Charge une image pour un sprite
QString ActionPanel::loadSpriteImage() {
    return QFileDialog::getOpenFileName(this, tr("Load Image"), GameFramework::imagesPath(), tr("Image Files (*.png *.jpg *.bmp)"));
}

//! Slot appelé lors du clic sur le bouton d'ajout de sprite
void ActionPanel::addButtonClicked() {
    // Charger une image
    QString imagePath = loadSpriteImage();

    if(!imagePath.isEmpty()){ // Si l'image n'est pas vide
        // Création d'un nouveau sprite
        m_pEditorManager->createEditorSprite(imagePath);
    }
}

//! Charge un string de style depuis un fichier
QString ActionPanel::loadStyleSheet(const QString& path) {
    QFile file(path);
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    return styleSheet;
}

