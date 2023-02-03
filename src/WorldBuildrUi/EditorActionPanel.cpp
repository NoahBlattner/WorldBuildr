//
// Created by blatnoa on 03.02.2023.
//

#include <iostream>
#include "EditorActionPanel.h"
#include "EditorButton.h"
#include "resources.h"
#include <QBrush>

EditorActionPanel::EditorActionPanel() {
    setRect(0, 0, 200, 200);
    setBrush(QBrush(QColor(240, 240, 240, 100)));

    auto* button = new EditorButton(GameFramework::imagesPath() + "tutorial/pacman_1.png",
                                    []() { std::cout << "Hello World" << std::endl; },
                                    this);
    button->setPos(0, 0);
    button->setScale(.2);
}
