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
class QListWidgetItem;

#include <QDialog>

class TagEditDialog : public QDialog {

    Q_OBJECT

public:

    TagEditDialog(EditorSprite* sprite, QWidget* parent);

private:

    EditorSprite* m_pSprite;

    QPushButton* addTagButton;
    QPushButton* removeTagButton;

    QListWidget* tagList;

    QLineEdit* m_pCurrentTagEdit;

    QPushButton* selectButton;
    QPushButton* clearTagButton;

    void initLayout();
    void initInputs();

    void updateList();

    void setTagOnSprite(const QString& tag);

private slots:
    void onTagListItemDoubleClicked(QListWidgetItem* item);

    void onAddTagButtonClicked();
    void onDeleteTagButtonClicked();

    void onSelectButtonClicked();
    void onNoTagButtonClicked();
};


#endif //WORLDBUILDR_TAGEDITDIALOG_H
