/**
\file
\brief    Déclaration de la classe Sprite.
\author   JCO
\date     janvier 2014
*/
#ifndef SPRITE_H
#define SPRITE_H

// décommenter pour rendre le boundingrect de tous les sprites visible.
//#define DEBUG_BRECT
// décommenter pour rendre la shape de tous les sprites visible.
//#define DEBUG_SHAPE
// décommenter pour afficher dans la sortie de debug le nombre de sprites existants
#define DEBUG_SPRITE_COUNT

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QPixmap>
#include <QTimer>

class GameScene;
class SpriteTickHandler;

//! \brief Classe qui représente un élément d'animation graphique 2D.
//!
//! Cette classe met à disposition différentes méthodes permettant de gérer simplement un sprite animé.
//! Un sprite est un élément graphique qui affiche une image bitmap. Cet élément graphique doit appartenir à une scène.
//! Il possède un certain nombre de propriétés, associées à des méthodes, influençant son apparence et sa position :
//! - `pos` : la position 2D  du sprite
//! - `x` : la position sur l'axe horizontal du sprite
//! - `y` : la position sur l'axe vertical du sprite
//! - `z` : l'ordre z du sprite
//! - `rotation` : l'angle de rotation du sprite
//! - `scale` : un facteur de grossissement/rappetissment du sprite
//! - `opacity` : l'opacité du sprite, valeur décimal comprise en 0 et 1, 0 étant transparent et 1 opaque.
//!
//! Toutes ces propriétés peuvent être animées de façon très simple au moyen d'un objet QPropertyAnimation.
//!
//! D'autres méthodes permettent de connaître la géométrie et le placement du sprite : width(), height(), posX(), posY(), left(), right(), top(), bottom().
//! Il est possible d'obtenir le rectangle dans lequel le sprite est inscrit avec globalBoundingBox().
//! Il est possible d'obtenir la forme exacte du sprite avec globalShape().
//!
//! L'apparence du sprite n'est déterminée que par une seule image. Toutefois, il est possible d'en mémoriser plusieurs, afin de changer facilement d'apparence. Il est également possible de faire changer automatiquement ces images dans le but d'obtenir un sprite animé.
//!
//! La méthode addAnimationFrame() permet d'ajouter une image au sprite. Si plusieurs images sont ajoutées, elles sont conservées dans une liste qui préserve l'ordre d'ajout des images.
//!
//! La méthode setCurrentAnimationFrame() permet de spécifier quelle image doit être affichée (l'indice de la première image est 0). La méthode currentAnimationFrame() indique quelle image est actuellement affichée par le sprite.
//!
//! La méthode startAnimation() permet de démarrer l'animation des images. La méthode stopAnimation() permet
//! de stopper l'animation des images, soit immédiatement (IMMEDIATE_STOP) soit à la fin du cycle (END_OF_CYCLE_STOP).
//! La vitesse d'animation peut être réglée avec setAnimationSpeed() ou au moment de démarrer l'animation.
//!
//! Il est également possible de demander au sprite d'émettre un signal chaque fois que l'animation est terminée, avec la méthode setEmitSignalEndOfAnimationEnabled(). Cela permet par exemple de connecter ce signal au slot deleteLater() du même objet, afin de
//! détruire automatiquement le sprite dès que l'animation est terminée (par exemple pour afficher une explosion).
//!
//! Comme la classe Sprite spécialise la classe QGraphicsPixmapItem, qui elle-même spécialise la class QGraphicsItem, toutes les méthodes de QGraphicsItem sont accessibles au sprite.
//!
//! \section sprite_on_scene Intéger un sprite à une scène
//! Lorsqu'un sprite est créé, il faut le placer sur une scène pour qu'il apparaisse, au
//! moyen de la méthode GameScene::addSpriteToScene(). Celle-ci en prend alors la
//! propriété et se chargera de la détruire lorsque la scène sera détruite.
//!
//! Une fois ajouté à la scène, il est possible de l'abonner au tick afin qu'il
//! reçoive la cadence du jeu, grâce à la méthode GameScene::registerSpriteForTick().
//! Dès qu'un sprite est abonné à la cadence, sa méthode virtuelle tick() est
//! automatiquement appelée.
//!
//! Attention : si le sprite est détruit, il faut s'assurer au préalable de le
//! désabonner avec GameScene::unregisterSpriteFromTick().
//!
//! Avant d'être détruit, un sprite émet le signal spriteDestroyed().
//!
//! \section sprite_pos Positionnement du sprite
//! Lorsqu'un sprite est positionné sur la scène au moyen de setPos(), c'est en réalité
//! le coin supérieur gauche du sprite qui est positionné à la coordonnée donnée.
//!
//! Ce point de positionnement, appelé aussi hotspot, peut être
//! déplacé avec la méthode setOffet().
//!
//! Cela permet par exemple de positionner le sprite avec son centre plutôt qu'avec son
//! coin supérieur gauche.
//!
//! \section sprite_transform Transformation du sprite
//!
//! Il est possible d'appliquer une transformation sur le sprite, par exemple pour le tourner,
//! changer sa taille ou appliquer un effet miroir avec les méthodes setRotation() et setScale().
//!
//! Si des transformations plus complexes doivent être réalisées, la méthode setTransformations()
//! permet d'appliquer une liste de transformations à un sprite.
//!
//! Le point de transformation peut être défini avec setTransformOriginPoint().
//!
//! \section tick_handler Le gestionnaire de cadence
//!
//! Un sprite peut être déplacé de plusieurs façons différents au sein d'une scène.
//!
//! Une première solution est qu'un élément externe se charge de la position d'un
//! sprite en utilisant les propriétés publiques du sprite, telle que x (setX()),
//! y (setY()) et z (setZ()) ou la fonction setPos().
//!
//! Une autre solution est de laisser le sprite gérer seul son déplacement en se
//! basant sur la cadence du jeu.
//!
//! Pour faire cela, il suffit d'attribuer au sprite un gestionnaire de cadence (SpriteTickHandler())
//! qui contiendra le code nécessaire pour faire bouger le sprite.
//!
//! Si un gestionnaire de cadence est attribué au sprite, à chaque tick, le gestionnaire
//! de cadence est appelé (tick()).
//!
//! Une dernière solution est  de spécialiser la classe Sprite afin de surcharger
//! la méthode tick().
//!
class Sprite : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ opacity WRITE setOpacity NOTIFY opacityChanged FINAL)
    Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled NOTIFY enabledChanged)
    Q_PROPERTY(bool visible READ isVisible WRITE setVisible NOTIFY visibleChanged FINAL)
    Q_PROPERTY(QPointF pos READ pos WRITE setPos FINAL)
    Q_PROPERTY(qreal x READ x WRITE setX NOTIFY xChanged FINAL)
    Q_PROPERTY(qreal y READ y WRITE setY NOTIFY yChanged FINAL)
    Q_PROPERTY(qreal z READ zValue WRITE setZValue NOTIFY zChanged FINAL)
    Q_PROPERTY(qreal rotation READ rotation WRITE setRotation NOTIFY rotationChanged)
    Q_PROPERTY(qreal scale READ scale WRITE setScale NOTIFY scaleChanged)
    Q_PROPERTY(QPointF transformOriginPoint READ transformOriginPoint WRITE setTransformOriginPoint)
    Q_PROPERTY(int currentAnimationFrame READ currentAnimationFrame WRITE setCurrentAnimationFrame)

public:
    enum AnimationStopMode {
        IMMEDIATE_STOP,
        END_OF_CYCLE_STOP
    };

    Sprite(QGraphicsItem* pParent = nullptr);
    Sprite(const QPixmap& rPixmap, QGraphicsItem* pParent = nullptr);
    Sprite(const QString& rImagePath, QGraphicsItem* pParent = nullptr);
    virtual ~Sprite() override;

    void addAnimationFrame(const QPixmap& rPixmap);
    void setCurrentAnimationFrame(int frameIndex);
    int currentAnimationFrame() const;
    void clearAnimationFrames();
    void showNextAnimationFrame();
    void setAnimationSpeed(int frameDuration);
    void stopAnimation(AnimationStopMode stopMode = IMMEDIATE_STOP);
    void startAnimation();
    void startAnimation(int frameDuration);
    bool isAnimationRunning() const;

    void addAnimation();
    void clearAnimations();
    int animationCount() const;
    void setActiveAnimation(int index);

    void setEmitSignalEndOfAnimationEnabled(bool enabled);
    bool isEmitSignalEndOfAnimationEnabled() const;

    QRectF globalBoundingRect() const { return mapRectToScene(boundingRect());  }
    QPainterPath globalShape() const { return mapToScene(shape()); }
    int width() const { return static_cast<int>(globalBoundingRect().width()); }
    int height() const { return static_cast<int>(globalBoundingRect().height()); }
    int left() const { return static_cast<int>(globalBoundingRect().left()); }
    int top() const { return static_cast<int>(globalBoundingRect().top()); }
    int right() const { return static_cast<int>(globalBoundingRect().right()); }
    int bottom() const { return static_cast<int>(globalBoundingRect().bottom()); }

    void setParentScene(GameScene* pScene);

    enum { SpriteItemType = UserType + 1 };
    virtual int type() const override { return SpriteItemType; }

    virtual void tick(long long elapsedTimeInMilliseconds);
    void registerForTick();
    void unregisterFromTick();

    void setTickHandler(SpriteTickHandler* pTickHandler);
    SpriteTickHandler* tickHandler() const;
    void removeTickHandler();

    GameScene* parentScene() const;

    void setDebugModeEnabled(bool enabled);

#ifdef QT_DEBUG
    virtual void paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget = nullptr) override;
#endif

signals:
    void animationFinished();
    void opacityChanged();
    void visibleChanged();
    void enabledChanged();
    void xChanged();
    void yChanged();
    void zChanged();
    void rotationChanged();
    void scaleChanged();
    void spriteDestroyed(Sprite*);

protected:
    QList<Sprite*> collidingSprites() const;
    QList<Sprite*> collidingSprites(const QRectF& rRect) const;
    QList<Sprite*> collidingSprites(const QPainterPath& rShape) const;
    GameScene* m_pParentScene;

private:
    static int s_spriteCount;
    static void displaySpriteCount();

    void init();

    SpriteTickHandler* m_pTickHandler;

    QTimer m_animationTimer;

    bool m_emitSignalEOA;
    bool m_animationStopLater = false;

    QList<QList <QPixmap>> m_animationList;
    int m_frameDuration;
    int m_currentAnimationFrame;
    int m_currentAnimationIndex;

    int m_customType;

    bool m_debugMode = false;

private slots:
    void onNextAnimationFrame();

};

#ifdef QT_DEBUG
    #include <QDebug>

    QDebug operator<<(QDebug dbg, const Sprite& sprite);
#endif

#endif // SPRITE_H
