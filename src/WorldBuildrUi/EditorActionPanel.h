/**
 * @file EditorActionPanel.h
 * @brief Définition de la classe EditorActionPanel.
 * @author Noah Blattner
 * @date Janvier 2023
 */

#ifndef WORLDBUILDR_EDITORACTIONPANEL_H
#define WORLDBUILDR_EDITORACTIONPANEL_H

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>

class EditorManager;
class QPushButton;

//! Classe affichant le panneau d'actions de l'éditeur.
//! Elle permet d'interagir avec l'éditeur par une interface graphique.
//! Elle est automatiquement instanciée par l'MainFrm.
//!
//! Après la création, il faut lier un éditeur avec la méthode bindEditorManager().
class EditorActionPanel : public QWidget {
public:
    explicit EditorActionPanel(QWidget* pParent = nullptr);

    void bindEditorManager(EditorManager* editorManager);

private:
    EditorManager* m_pEditorManager = nullptr;

    QVBoxLayout* mainLayout = nullptr;

    QGroupBox* groupeActionsCreation = new QGroupBox("Création et suppression");
    QVBoxLayout* layoutActionsCreation = new QVBoxLayout();
    QPushButton* addButton;
    QPushButton* duplicateButton;
    QPushButton* removeButton;

    QGroupBox* groupeActionsHistorique = new QGroupBox("Historique");
    QVBoxLayout* layoutActionsHistorique = new QVBoxLayout();
    QPushButton* undoButton;
    QPushButton* redoButton;

    QGroupBox* groupeActionsSelection = new QGroupBox("Sélection");
    QVBoxLayout* layoutActionsSelection = new QVBoxLayout();
    QPushButton* selectAllButton;
    QPushButton* deselectAllButton;

    QGroupBox* groupeActionsFond = new QGroupBox("Autres actions");
    QVBoxLayout* layoutActionsFond = new QVBoxLayout();
    QPushButton* addBackgroundButton;
    QPushButton* removeBackgroundButton;

    void initButtons();
    void initLayout();
    void connectSignals() const;

private slots:
    void addButtonClicked();
    void deleteButtonClicked();
    void undoButtonClicked();
    void redoButtonClicked();
    void selectAllButtonClicked();
    void deselectAllSprites();
    void duplicateButtonClicked();
    void addBackgroundButtonClicked();
    void removeBackgroundButtonClicked();
};


#endif //WORLDBUILDR_EDITORACTIONPANEL_H
