//
// Created by blatnoa on 01.02.2023.
//

#ifndef WORLDBUILDR_ACTIONPANEL_H
#define WORLDBUILDR_ACTIONPANEL_H

#include <QWidget>

class EditorManager;
class QVBoxLayout;
class QPushButton;

class ActionPanel : public QWidget {
public:
    explicit ActionPanel(QWidget* pParent = nullptr);

    void initUI(EditorManager* editorManager);

private:
    EditorManager* m_pEditorManager = nullptr;

    QVBoxLayout* layout = nullptr;

    QPushButton* addButton = nullptr;
    QPushButton *removeButton = nullptr;

    QString loadSpriteImage();

private slots:
    void addButtonClicked();
    void deleteButtonClicked();

};


#endif //WORLDBUILDR_ACTIONPANEL_H
