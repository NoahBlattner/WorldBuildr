//
// Created by blatnoa on 01.02.2023.
//

#include "EditorUi.h"
#include "EditorManager.h"
#include "EditorButton.h"
#include "resources.h"
#include "GameScene.h"
#include <iostream>

EditorUi::EditorUi(EditorManager* editorManager) {
    m_pEditorManager = editorManager;

    // Build UI
    EditorButton* button = new EditorButton(GameFramework::imagesPath() + "tutorial/pacman_1.png", []() { std::cout << "Hello World" << std::endl; });
    editorManager->getScene()->addItem(button);

}
