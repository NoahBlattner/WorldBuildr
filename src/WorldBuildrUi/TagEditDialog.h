//
// Created by blatnoa on 02.03.2023.
//

#ifndef WORLDBUILDR_TAGEDITDIALOG_H
#define WORLDBUILDR_TAGEDITDIALOG_H

class QVBoxLayout;
class QHBoxLayout;
class QListWidget;
class EditorManager;
class EditorSprite;
class QLineEdit;

#include <QDialog>

class TagEditDialog : public QDialog {

    Q_OBJECT

public:

    TagEditDialog(EditorSprite* sprite, QWidget* parent);

private:

    EditorSprite* m_pSprite;

    QPushButton* addButton;
    QPushButton* removeButton;

    QListWidget* tagList;

    QLineEdit* m_pCurrentTagEdit;

    QPushButton* selectButton;

    void initLayout();
    void initInputs();

    void updateList();

private slots:
    void onAddButtonClicked();
    void onRemoveButtonClicked();
    void onSelectButtonClicked();

};


#endif //WORLDBUILDR_TAGEDITDIALOG_H
