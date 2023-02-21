//
// Created by Noah on 18.02.2023.
//

#ifndef WORLDBUILDR_SPRITEDETAILSPANEL_H
#define WORLDBUILDR_SPRITEDETAILSPANEL_H


#include <QWidget>
#include <QGroupBox>
#include <QStringList>

class QVBoxLayout;
class QHBoxLayout;
class QSpinBox;
class QComboBox;
class EditorManager;
class EditorSprite;

//! Classe de panneau de détails d'un sprite.
class SpriteDetailsPanel : public QWidget {

    Q_OBJECT

public slots:
    void onBindSprite(EditorSprite* sprite);
    void onUnbindSprite();

    void onSpriteModified();

    void onPosStepChanged(int newStepIndex);
    void onSizeStepChanged(int newStepIndex);
    void onRotationStepChanged(int newStepIndex);

    void onXPosFieldEdited(int value);
    void onYPosFieldEdited(int value);

    void onWidthFieldEdited(int value);
    void onHeightFieldEdited(int value);

    void onRotationFieldEdited(int value);

public:
    explicit SpriteDetailsPanel(QWidget* pParent = nullptr);

    void bindEditorManager(EditorManager* pEditorManager);

private:
    const QStringList PIXEL_STEPS = {"1 px", "5 px", "10 px", "25 px", "50 px", "100 px"};
    const QStringList ROTATION_STEPS = {"1°", "5°", "10°", "15°", "30°", "45°", "90°"};

    EditorManager* m_pEditorManager = nullptr;
    EditorSprite* m_pSprite = nullptr;

    QHBoxLayout* mainLayout = nullptr;

    QHBoxLayout* optionsLayout;
    QVBoxLayout* positionLayout;
    QVBoxLayout* sizeLayout;

    QComboBox* posStepEdit;
    QComboBox* sizeStepEdit;
    QComboBox* rotationStepEdit;

    QSpinBox* xPositionEdit;
    QSpinBox* yPositionEdit;

    QSpinBox* widthEdit;
    QSpinBox* heightEdit;

    QSpinBox* rotationEdit;

    void initLayout();
    void initInputs();
    void connectSignals();

    void updatePanel();
};


#endif //WORLDBUILDR_SPRITEDETAILSPANEL_H
