//
// Created by blatnoa on 01.02.2023.
//

#include "EditorHudScene.h"
#include "EditorManager.h"
#include "EditorButton.h"
#include "resources.h"
#include "GameScene.h"
#include <iostream>
#include "EditorActionPanel.h"

EditorHudScene::EditorHudScene(EditorManager* editorManager) {
    m_pEditorManager = editorManager;

    // Build UI
    addItem(new EditorActionPanel());
}
