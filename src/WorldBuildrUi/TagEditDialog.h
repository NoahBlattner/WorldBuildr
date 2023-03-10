/**
 * @file TagEditDialog.h
 * @brief Définition de la classe TagEditDialog.
 * @author Noah Blattner
 * @date Mars 2023
 */

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

/**
 * @brief Fenêtre de dialogue permettant d'éditer les tags d'un sprite.
 *
 * Cette fenêtre permet d'ajouter, de supprimer et de sélectionner des tags.
 *
 * Elle affiche une liste de tout les tags et permet d'en ajouter ou d'en supprimer.
 * Ainsi qu'un champ de texte qui montre le tag du sprite lié à la fenêtre.
 */
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

    QPushButton* applyTagButton;
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
