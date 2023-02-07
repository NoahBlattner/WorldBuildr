//
// Created by blatnoa on 01.02.2023.
//

#ifndef WORLDBUILDR_ACTIONPANEL_H
#define WORLDBUILDR_ACTIONPANEL_H

#include <QWidget>
#include <QGroupBox>
#include <QVBoxLayout>

class EditorManager;
class QPushButton;

class ActionPanel : public QWidget {
public:
    explicit ActionPanel(QWidget* pParent = nullptr);

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

    void initLayout();

    void connectSignals() const;
};


#endif //WORLDBUILDR_ACTIONPANEL_H
