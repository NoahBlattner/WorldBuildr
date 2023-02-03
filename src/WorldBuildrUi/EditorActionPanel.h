//
// Created by blatnoa on 03.02.2023.
//

#ifndef WORLDBUILDR_EDITORACTIONPANEL_H
#define WORLDBUILDR_EDITORACTIONPANEL_H

#include <QGraphicsWidget>

class QGraphicsLinearLayout;
class EditorHud;
class QPushButton;

class EditorActionPanel : public QGraphicsWidget {
public:
    EditorActionPanel(EditorHud* parent = nullptr);

private:
    QGraphicsLinearLayout *layout = nullptr;

    QPushButton* addButton = nullptr;

};


#endif //WORLDBUILDR_EDITORACTIONPANEL_H
