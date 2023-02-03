//
// Created by blatnoa on 01.02.2023.
//

#include "EditorHud.h"
#include "EditorManager.h"
#include "EditorButton.h"
#include "resources.h"
#include "GameScene.h"
#include "EditorActionPanel.h"
#include "GameCore.h"
#include <QPushButton>
#include <QGraphicsLinearLayout>

EditorHud::EditorHud(QWidget* pParent) : QGraphicsScene(pParent){
}

void EditorHud::initHud(EditorManager *editorManager) {
    m_pEditorManager = editorManager;

    actionPanel = new EditorActionPanel(this);
    //actionPanel->setAlignment(Qt::AlignLeft | Qt::AlignCenter);
    addItem(actionPanel);

}

