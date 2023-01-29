/**
  \file
  \brief    Déclaration de la classe MainFrm.
  \author   JCO
  \date     janvier 2014
  \version  1.0
*/
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>

namespace Ui {
class MainFrm;
}

class GameCanvas;

//! \brief Fenêtre principale.
//!
//! MainFrm représente la fenêtre principale, dans laquelle est placé
//! une instance de GameView.
//! Cette fenêtre gère une instance de GameCanvas, qui est en charge
//! de la cadence du jeu.
//!
//! Pour changer le titre de la fenêtre, il faut éditer le fichier `mainfrm.ui` et
//! modifier la propriété `windowTitle` de l'objet MainFrm.
class MainFrm : public QWidget
{
    Q_OBJECT

public:
    explicit MainFrm(QWidget* pParent = nullptr);
    ~MainFrm() override;

private:
    Ui::MainFrm *ui;

    GameCanvas* m_pGameCanvas;
};

#endif // WIDGET_H
