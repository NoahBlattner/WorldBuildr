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
class QDoubleSpinBox;
class QComboBox;
class QListWidget;
class EditorManager;
class EditorSprite;

//! Classe qui affiche les détails éditable d'un sprite.
//! Elle est automatiquement instanciée par MainFrm.
//!
//! Après la création, il faut lier un éditeur avec la méthode bindEditorManager().
//!
//! Pour que la classe puisse afficher les détails d'un sprite, il faut lier un sprite
//! Un sprite peut être lié avec la méthode onBindSprite().
//! Un sprite peut être délié avec la méthode onUnbindSprite().
//! Si un sprite est lié, la classe affiche les détails du sprite.
//! Si aucun sprite n'est lié, la classe désactive les champs d'édition.
//!
//! La classe est automatiquement liée à un sprite lorsqu'un sprite est sélectionné dans l'éditeur.
//! La classe est automatiquement déliée à un sprite lorsque le sprite lié est déselectionné ou supprimé.
//!
//! Les champs de détails sont mis à jour automatiquement lorsque le sprite lié est modifié.
//! Le sprite lié est modifié instantanément lorsque l'utilisateur modifie les champs d'édition.
class SpriteDetailsPanel : public QWidget {

    Q_OBJECT

public slots:
    void onBindSprite(EditorSprite* sprite);
    void onUnbindSprite();

    void onSpriteModified();

    void onPosStepChanged(int newStepIndex);
    void onRotationStepChanged(int newStepIndex);

    void onXPosFieldEdited(int value);
    void onYPosFieldEdited(int value);
    void onZPosFieldEdited(int value);

    void onScaleFieldEdited(double newScale);

    void onRotationFieldEdited(int value);

    void onOpacityFieldEdited(int value);

    void onEditTagsButtonClicked();

public:
    explicit SpriteDetailsPanel(QWidget* pParent = nullptr);

    void bindEditorManager(EditorManager* pEditorManager);

private:
    const QStringList PIXEL_STEPS = {"1 px", "5 px", "10 px", "25 px", "50 px", "100 px"};
    const QStringList ROTATION_STEPS = {"1°", "5°", "10°", "15°", "30°", "45°", "90°"};

    EditorManager* m_pEditorManager = nullptr;
    EditorSprite* m_pSprite = nullptr;

    QHBoxLayout* mainLayout = nullptr;

    QComboBox* m_pPosStepEdit;
    QComboBox* m_pRotationStepEdit;

    QSpinBox* m_pXPositionEdit;
    QSpinBox* m_pYPositionEdit;
    QSpinBox* m_pZPositionEdit;

    QDoubleSpinBox* m_pScaleEdit;

    QSpinBox* m_pRotationEdit;

    QSpinBox* m_pOpacityEdit;

    QPushButton* m_pSetTagButton;

    bool m_ignoreFieldEdited = false;

    void initLayout();
    void initInputs();
    void connectSignals();

    void updatePanel();

    void disconnectSpriteSignals() const;

    void connectSpriteSignals() const;
};


#endif //WORLDBUILDR_SPRITEDETAILSPANEL_H
