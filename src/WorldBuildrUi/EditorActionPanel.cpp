//
// Created by blatnoa on 03.02.2023.
//

#include <iostream>

#include "EditorActionPanel.h"
#include "EditorButton.h"
#include "resources.h"
#include <QPushButton>
#include <QIcon>
#include <QGraphicsLinearLayout>
#include "EditorHud.h"
#include <QVBoxLayout>

EditorActionPanel::EditorActionPanel(EditorHud* parent) : QGraphicsWidget() {
    layout = new QGraphicsLinearLayout(Qt::Vertical, this);

    addButton = new QPushButton(QIcon(GameFramework::imagesPath() + "icons/add.png"), "Add sprite");

    layout->addItem(addButton);

    setLayout(layout);
}
