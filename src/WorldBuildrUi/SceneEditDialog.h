/**
 * @file SceneEditDialog.h
 * @brief Définition de la classe SceneEditDialog.
 * @author Noah Blattner
 * @date Mars 2023
 */

#ifndef WORLDBUILDR_SCENEEDITDIALOG_H
#define WORLDBUILDR_SCENEEDITDIALOG_H

class GameScene;

#include <QDialog>

/**
 * @brief Fenêtre de dialogue permettant d'éditer la taille d'une scène.
 *
 * Cette fenêtre permet d'éditer la taille d'une scène.
 * Elle affiche deux champs de texte qui montrent la largeur et la hauteur de la scène.
 *
 * Elle permet de modifier la taille de la scène.
 *
 * Elle nécessite un pointeur vers la scène à éditer.
 */
class SceneEditDialog : public QDialog {

    Q_OBJECT

public:
    explicit SceneEditDialog(GameScene* scene, QWidget* pParent = nullptr);

private:
    GameScene* m_scene;

    QSpinBox* m_widthInput;
    QSpinBox* m_heightInput;

    QCheckBox* m_resizeObjectsInput;

    QPushButton* m_okButton;

    void initLayout();
    void initInputs();

private slots:
    void onOkButtonClicked();
};


#endif //WORLDBUILDR_SCENEEDITDIALOG_H
