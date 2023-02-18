//
// Created by Noah on 18.02.2023.
//

#include <QPushButton>
#include <QHBoxLayout>
#include "SpriteDetailsPanel.h"

SpriteDetailsPanel::SpriteDetailsPanel(QWidget *pParent) : QWidget(pParent) {
    QHBoxLayout* layout = new QHBoxLayout(this);

    setLayout(layout);

    QPushButton* button = new QPushButton("Test", this);

    layout->addWidget(button);

}
