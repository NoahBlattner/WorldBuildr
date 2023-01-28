/**
  Déclaration de la classe GameView, qui affiche une scène.

  @author   JCO
  @date     Janvier 2015
 */
#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QGraphicsView>

//! \brief Classe de visualisation d'un espace 2D de jeu.
//!
//! GameView représente la vue utilisée par l'application pour afficher une scène (QGraphicsScene), généralement plus
//! spécifiquement la scène de jeu (GameScene)..
//!
//! Cette classe implémente les fonctionnalités suivantes :
//! - Possibilité d'adapter l'affichage de la scène à la taille de la fenêtre. Cette possibilité est déclanchée par défaut
//!   et peut être enclenchée avec setFitToScreenEnabled().
//! - Possibilité de "clipper" l'affichage de la scène, afin que tout élément en dehors de la surface de la scène soit
//!   caché. Cette possibilité est déclanchée par défaut et peut être enclenchée avec setClipSceneEnabled().
//! - Possibilité d'afficher une scène en tant que HUD (Head Up Display), afin d'afficher des
//!   informations par-dessus l'espace de jeu, avec la méthode setHudScene().
class GameView : public QGraphicsView
{
public:
    GameView(QWidget* pParent = nullptr);
    GameView(QGraphicsScene* pScene, QWidget* pParent = nullptr);
    ~GameView() override;

    void setFitToScreenEnabled(bool fitToScreenEnabled);
    bool isFitToScreenEnabled() const;

    void setClipSceneEnabled(bool clipSceneEnabled);
    bool isClipSceneEnabled() const;

    void setHudScene(QGraphicsScene* pHudScene);
    QGraphicsScene* hudScene() const;

protected:
    virtual void resizeEvent(QResizeEvent* pEvent) override;
    virtual void drawForeground(QPainter* pPainter, const QRectF& rRect) override;

private:
    void init();

    bool m_fitToScreen;
    bool m_clipScene;

    bool m_clippingRectUpToDate;
    QRectF m_clippingRect[4];

    QGraphicsScene* m_pHudScene = nullptr;
};

#endif // GAMEVIEW_H
