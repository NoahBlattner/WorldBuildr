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

    QPushButton* addButton;
    QPushButton* duplicateButton;
    QPushButton* removeButton;

    QPushButton* undoButton;
    QPushButton* redoButton;

    QPushButton* selectAllButton;
    QPushButton* deselectAllButton;

    QPushButton* addBackgroundButton;
    QPushButton* removeBackgroundButton;

    QPushButton* saveButton;
    QPushButton* loadButton;
    QPushButton* importButton;

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
    void saveButtonClicked();
    void loadButtonClicked();
    void importButtonClicked();
};


#endif //WORLDBUILDR_EDITORACTIONPANEL_H
