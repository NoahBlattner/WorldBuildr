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

    QGroupBox* groupeActionsSelection = new QGroupBox("Sélection");
    QVBoxLayout* layoutActionsSelection = new QVBoxLayout();
    QPushButton* selectAllButton;
    QPushButton* deselectAllButton;

    QGroupBox* groupeActionsAutre = new QGroupBox("Autres actions");
    QVBoxLayout* layoutActionsAutre = new QVBoxLayout();
    QPushButton* addBackgroundButton;

    void initButtons();

private slots:
    void addButtonClicked();
    void deleteButtonClicked();
    void selectAllButtonClicked();
    void deselectAllSprites();
    void duplicateButtonClicked();
    void addBackgroundButtonClicked();

    void initLayout();

    void connectSignals() const;
};


#endif //WORLDBUILDR_ACTIONPANEL_H
