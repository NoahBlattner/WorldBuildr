/**
 * @file SceneEditDialog.cpp
 * @brief Définition de la classe SceneEditDialog.
 * @author Noah Blattner
 * @date Mars 2023
 */

#include <QSpinBox>
#include <QLabel>
#include <QPushButton>
#include <QCheckBox>
#include "GameScene.h"
#include "resources.h"

#include "SceneEditDialog.h"

SceneEditDialog::SceneEditDialog(GameScene* scene, QWidget *pParent) {
    m_scene = scene;

    initInputs();
    initLayout();

    setWindowTitle("Edit Scene");

    connect(m_okButton, &QPushButton::clicked, this, &SceneEditDialog::onOkButtonClicked);
}

void SceneEditDialog::initLayout() {
    // Création du layout principal
    auto* mainLayout = new QVBoxLayout();
    mainLayout->setAlignment(Qt::AlignRight);

    // Création du layout pour l'hauteur
    auto* heightLayout = new QHBoxLayout();
    heightLayout->addWidget(new QLabel("Height:"));
    heightLayout->addWidget(m_heightInput);

    // Création du layout pour la largeur
    auto* widthLayout = new QHBoxLayout();
    widthLayout->addWidget(new QLabel("Width:"));
    widthLayout->addWidget(m_widthInput);

    // Ajout du layout au layout principal
    mainLayout->addLayout(heightLayout);
    mainLayout->addLayout(widthLayout);
    mainLayout->addWidget(m_resizeObjectsInput);
    mainLayout->addWidget(m_okButton);

    // Ajout du layout principal à la fenêtre
    setLayout(mainLayout);
}

void SceneEditDialog::initInputs() {
    // Création du spinbox pour la largeur
    m_widthInput = new QSpinBox();
    m_widthInput->setRange(100, 10000);
    m_widthInput->setValue(m_scene->width());
    m_widthInput->setStyleSheet(GameFramework::loadStyleSheetString("spinboxStyle.qss"));

    // Création du spinbox pour la hauteur
    m_heightInput = new QSpinBox();
    m_heightInput->setRange(100, 10000);
    m_heightInput->setValue(m_scene->height());
    m_heightInput->setStyleSheet(GameFramework::loadStyleSheetString("spinboxStyle.qss"));

    // Création du checkbox pour redimensionner les objets
    m_resizeObjectsInput = new QCheckBox("Resize objects");
    m_resizeObjectsInput->setChecked(true);
    m_resizeObjectsInput->setStyleSheet(GameFramework::loadStyleSheetString("checkboxStyle.qss"));

    // Création du bouton OK
    m_okButton = new QPushButton("Appliquer");
    m_okButton->setStyleSheet(GameFramework::loadStyleSheetString("buttonStyle.qss"));
}

//! Slot appelé lorsque le bouton OK est cliqué.
void SceneEditDialog::onOkButtonClicked() {
    m_scene->setSize(QSizeF(m_widthInput->value(), m_heightInput->value()) , m_resizeObjectsInput->isChecked());
    close();
}


