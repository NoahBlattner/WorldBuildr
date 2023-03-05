//
// Created by Noah on 05.03.2023.
//

#ifndef WORLDBUILDR_SCENEEDITDIALOG_H
#define WORLDBUILDR_SCENEEDITDIALOG_H

class GameScene;

#include <QDialog>

class SceneEditDialog : public QDialog {

    Q_OBJECT

public:
    explicit SceneEditDialog(GameScene* scene, QWidget* pParent = nullptr);

private:
    GameScene* m_scene;

    QSpinBox* m_widthInput;
    QSpinBox* m_heightInput;

    QPushButton* m_okButton;

    void initLayout();
    void initInputs();

private slots:
    void onOkButtonClicked();
};


#endif //WORLDBUILDR_SCENEEDITDIALOG_H
