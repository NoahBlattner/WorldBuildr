/**
  \file
  \brief    Définition de la classe Sprite.
  \author   JCO
  \date     janvier 2014
*/
#include "sprite.h"

#include <QDebug>
#include <QPainter>

#include "gamescene.h"
#include "spritetickhandler.h"

int Sprite::s_spriteCount = 0;

const int NO_CURRENT_FRAME = -1;

//! Construit un sprite et l'initialise.
//! Le sprite n'a pas d'apparence particulière et n'affichera rien
//! tant qu'une image ne lui sera pas fournie avec addAnimationFrame().
//! \param pParent  Pointeur sur le parent (afin d'obtenir une destruction automatique de cet objet).
Sprite::Sprite(QGraphicsItem* pParent) : QGraphicsPixmapItem(pParent) {
    init();
}

//! Construit un sprite et l'initialise.
//! Le sprite utilisera l'image fournie pour son apparence.
//! \param rPixmap   Image à utiliser pour l'apparence du sprite.
//! \param pParent   Pointeur sur le parent (afin d'obtenir une destruction automatique de cet objet).
Sprite::Sprite(const QPixmap& rPixmap, QGraphicsItem* pParent) : QGraphicsPixmapItem(pParent) {
    init();
    addAnimationFrame(rPixmap);
}

//! Construit un sprite et l'initialise.
//! Le sprite utilisera l'image fournie pour son apparence.
//! \param rImagePath  Chemin vers l'image à utiliser pour l'apparence du sprite.
//! \param pParent     Pointeur sur le parent (afin d'obtenir une destruction automatique de cet objet).
Sprite::Sprite(const QString& rImagePath, QGraphicsItem* pParent) : Sprite(QPixmap(rImagePath), pParent) {

}

//! Destructeur.
Sprite::~Sprite() {
    emit spriteDestroyed(this);

#ifdef DEBUG_SPRITE_COUNT
    s_spriteCount--;
    displaySpriteCount();
#endif
    if (m_pTickHandler != nullptr) {
        delete m_pTickHandler;
        m_pTickHandler = nullptr;
    }
}

//! Ajoute une image au cycle d'animation.
//! \param rPixmap  Image à ajouter.
void Sprite::addAnimationFrame(const QPixmap& rPixmap) {
    m_animationList[m_currentAnimationIndex] << rPixmap;
    onNextAnimationFrame();
}

//! Change l'image d'animation présentée par le sprite.
//! L'image doit avoir été au préalable ajoutée aux images d'animation avec addAnimationFrame().
//! \param frameIndex   Index (à partir de zéro) de l'image à utiliser.
void Sprite::setCurrentAnimationFrame(int frameIndex) {
    if (m_animationList[m_currentAnimationIndex].isEmpty())
        return;

    if (frameIndex < 0 || frameIndex >= m_animationList[m_currentAnimationIndex].count())
        frameIndex = 0;

    m_currentAnimationFrame = frameIndex;
    setPixmap(m_animationList[m_currentAnimationIndex][frameIndex]);
}

//! \return l'index de l'image d'animation actuellement affichée.
int Sprite::currentAnimationFrame() const {
    return m_currentAnimationFrame;
}

//! Efface toutes les images du sprite.
//! Le sprite devient invisible.
void Sprite::clearAnimationFrames() {
    m_animationList[m_currentAnimationIndex].clear();
    m_currentAnimationFrame = NO_CURRENT_FRAME;
    setPixmap(QPixmap()); // On enlève l'image du sprite afin d'éviter toute confusion.
}

//! Affiche l'image suivante.
void Sprite::showNextAnimationFrame() {
    onNextAnimationFrame();
}

//! Change la vitesse d'animation.
//! \param frameDuration   Durée d'une image en millisecondes.
void Sprite::setAnimationSpeed(int frameDuration) {
    if (frameDuration <= 0)
        m_animationTimer.stop();
    else if (frameDuration != m_frameDuration) {
        m_frameDuration = frameDuration;
        m_animationTimer.setInterval(frameDuration);
    }
}

//! Arrête l'animation.
//! Si le mode de stop est AnimationStopMode::IMMEDIATE_STOP, l'animation
//! est immédiatement stoppée.
//! Si le mode de stop est AnimationStopMode::END_OF_CYCLE_STOP, l'animation
//! termine son cycle avant de se stopper.
void Sprite::stopAnimation(AnimationStopMode stopMode) {
    if (!isAnimationRunning())
        return;

    if (stopMode == IMMEDIATE_STOP)
        m_animationTimer.stop();
    else if (stopMode == END_OF_CYCLE_STOP)
        m_animationStopLater = true;
}

//! Démarre l'animation.
//! La vitesse d'animation utilisée est celle qui a été
//! spécifiée avec setAnimationSpeed().
void Sprite::startAnimation() {
    m_currentAnimationFrame = NO_CURRENT_FRAME;
    onNextAnimationFrame();
    m_animationTimer.start();
}

//! Démarre l'animation à la vitesse donnée.
//! Cette vitesse donnée remplace la précédente.
//! \param frameDuration   Durée d'une image en millisecondes.
void Sprite::startAnimation(int frameDuration)  {
    setAnimationSpeed(frameDuration);
    startAnimation();
}

//! \return un booléen qui indique si l'animation est en cours.
bool Sprite::isAnimationRunning() const {
    return m_animationTimer.isActive();
}

//! Ajoute une animation supplémentaire à ce sprite.
//! \see clearAnimations()
//! \see setActiveAnimation()
//! \see animationCount()
void Sprite::addAnimation() {
    m_animationList.append(QList<QPixmap>());

}

//! Efface toutes les animations d'un sprite mais en préserve une vide.
void Sprite::clearAnimations() {
    m_animationList.clear();
    addAnimation();
    m_currentAnimationIndex = 0;
}

//! \return le nombre d'animations que possède ce sprite.
int Sprite::animationCount() const {
    return m_animationList.count();
}

//!
//! Indique l'animation active utilisée pour l'affichage du sprite.
//! Si l'index donné est invalide, c'est l'animation par défaut qui
//! est activée.
//! \param index Index de l'animation à activer.
//!
void Sprite::setActiveAnimation(int index) {
    if (index < 0 || index >= m_animationList.count())
        index = 0;

    m_currentAnimationIndex = index;
    setCurrentAnimationFrame(0);
}

//! Choisi si le signal animationFinished() doit être émis chaque fois que l'animation
//! est terminée.
//! \param enabled   Indique si le signal doit être émis (true) ou non (false).
void Sprite::setEmitSignalEndOfAnimationEnabled(bool enabled) {
    m_emitSignalEOA = enabled;
}

//! \return un booléen qui indique si le signal animationFinished() est émis
//! chaque fois que l'animation est terminée.
bool Sprite::isEmitSignalEndOfAnimationEnabled() const {
    return m_emitSignalEOA;
}

//! Mémorise la scène à laquelle appartient ce sprite.
//! \param pScene  Scène à laquelle appartient ce sprite.
void Sprite::setParentScene(GameScene* pScene) {
    m_pParentScene = pScene;
}

#ifdef QT_DEBUG
//! Dessine le sprite, avec sa boundingbox qui l'entoure.
void Sprite::paint(QPainter* pPainter, const QStyleOptionGraphicsItem* pOption, QWidget* pWidget) {
    QGraphicsPixmapItem::paint(pPainter, pOption, pWidget);

#ifdef DEBUG_BRECT
    pPainter->setPen(Qt::cyan);
    pPainter->drawRect(this->boundingRect());
#endif
#ifdef DEBUG_SHAPE
    pPainter->setPen(Qt::magenta);
    pPainter->drawPath(this->shape());
#endif

    if (m_debugMode) {
        //pPainter->setClipRect(this->boundingRect()); // Pour ne pas dessiner en dehors du sprite

        pPainter->setPen(Qt::cyan);
        pPainter->drawRect(this->boundingRect());

        pPainter->setPen(Qt::magenta);
        pPainter->drawPath(this->shape());

        // Afficher la position x,y selon le système de coordonnées global
        QFont font = pPainter->font();
        font.setPixelSize(static_cast<int>(10.0/scale()));
        font.setStyleStrategy(QFont::NoAntialias);
        pPainter->setFont(font);
        pPainter->setPen(Qt::white);
        pPainter->drawText(boundingRect().bottomLeft(), QString(" X %0   Y %1").arg(this->left()).arg(this->top()));

        // Suppression de la mise à l'échelle
        pPainter->save();
        pPainter->scale(1.0/this->scale(), 1.0/this->scale());

        // Afficher le point chaud
        pPainter->setPen(Qt::yellow);
        pPainter->drawPoint(0,0);
        pPainter->drawEllipse(QPoint(0,0), 5,5);

        // Rétablissement de la mise à l'échelle
        pPainter->restore();
    }
}
#endif

//! Enregistre ce sprite auprès de la scène afin qu'il soit informé de la
//! cadence et que la fonction tick() soit appelée en cadence.
void Sprite::registerForTick() {
    Q_ASSERT(m_pParentScene != nullptr);
    m_pParentScene->registerSpriteForTick(this);
}

//! Désolidarise ce sprite de la cadence.
void Sprite::unregisterFromTick() {
    Q_ASSERT(m_pParentScene != nullptr);
    m_pParentScene->unregisterSpriteFromTick(this);
}

//! Cadence.
//! Pour qu'un sprite soit cadencé, il doit s'enregister avec la méthode
//! registerForTick().
//! Si un gestionnaire de cadence est installé sur ce sprite, celui-ci est appelé.
//! \param elapsedTimeInMilliseconds  Temps écoulé depuis le tick précédent.
void Sprite::tick(long long elapsedTimeInMilliseconds) {
    if (m_pTickHandler)
        m_pTickHandler->tick(elapsedTimeInMilliseconds);
}

//! Attribue à ce sprite un gestionnaire de tick.
//! L'ancien gestionnaire est détruit.
//! Ce sprite prend la propriété du gestionnaire.
//! \param pTickHandler  Pointeur sur le nouveau gestionnaire.
void Sprite::setTickHandler(SpriteTickHandler* pTickHandler) {
    removeTickHandler();
    m_pTickHandler = pTickHandler;
    pTickHandler->setParentSprite(this);
    pTickHandler->init();
}

//! \return un pointeur sur le gestionnaire de tick actuel.
SpriteTickHandler*Sprite::tickHandler() const {
    return m_pTickHandler;
}

//! Supprime (et efface) le gestionnaire de tick actuel.
void Sprite::removeTickHandler() {
    if (m_pTickHandler != nullptr) {
        delete m_pTickHandler;
        m_pTickHandler = nullptr;
    }
}

//! \return un pointeur sur la scène à laquelle appartient ce sprite.
GameScene* Sprite::parentScene() const {
    return m_pParentScene;
}

//! Active ou désactive le mode de déboguage de ce sprite.
//! \param enabled Active ou pas le mode de déboguage.
void Sprite::setDebugModeEnabled(bool enabled) {
    m_debugMode = enabled;
    this->update();
}

//! Affiche dans la sortie de debug le nombre de sprites existants.
void Sprite::displaySpriteCount() {
    qDebug() << "Nombre de sprites : " << s_spriteCount;
}

//! Construit la liste de tous les sprites en collision avec ce sprite.
//! Si la scène contient de nombreux sprites, cette méthode peut prendre du temps.
//! \return une liste de sprites en collision. Si aucun autre sprite ne collisionne
//! avec ce sprite, la liste retournée est vide.
QList<Sprite*> Sprite::collidingSprites() const {
    QList<Sprite*> collidingSpriteList;

    if (m_pParentScene != nullptr) {
        collidingSpriteList = m_pParentScene->collidingSprites(this);
    } else {
        qDebug() << "Le sprite ne fait pas partie d'une scène.";
    }
    return collidingSpriteList;
}

//! Construit la liste de tous les sprites en collision avec le rectangle donné
//! en paramètre, sauf ce sprite-même.
//! Si la scène contient de nombreux sprites, cette méthode peut prendre du temps.
//! \param rRect Rectangle avec lequel il faut tester les collisions.
//! \return une liste de sprites en collision.
QList<Sprite*> Sprite::collidingSprites(const QRectF& rRect) const {
    QList<Sprite*> collidingSpriteList;

    if (m_pParentScene != nullptr) {
        collidingSpriteList = m_pParentScene->collidingSprites(rRect);

        // Ce sprite lui-même collisionne avec le rectangle donné. Il faut donc l'ignorer.
        collidingSpriteList.removeAll(const_cast<Sprite*>(this));
    } else {
        qDebug() << "Le sprite ne fait pas partie d'une scène.";
    }
    return collidingSpriteList;
}

//! Construit la liste de tous les sprites en collision avec la forme donnée
//! en paramètre, sauf ce sprite-même.
//! Si la scène contient de nombreux sprites, cette méthode peut prendre du temps.
//! \param rShape Forme avec laquelle il faut tester les collisions.
//! \return une liste de sprites en collision.
QList<Sprite*> Sprite::collidingSprites(const QPainterPath& rShape) const {
    QList<Sprite*> collidingSpriteList;

    if (m_pParentScene != nullptr) {
        collidingSpriteList = m_pParentScene->collidingSprites(rShape);

        // Ce sprite lui-même collisionne avec le rectangle donné. Il faut donc l'ignorer.
        collidingSpriteList.removeAll(const_cast<Sprite*>(this));
    } else {
        qDebug() << "Le sprite ne fait pas partie d'une scène.";
    }
    return collidingSpriteList;
}

//! Initialise le sprite.
void Sprite::init() {
    m_pTickHandler = nullptr;
    m_pParentScene = nullptr;
    m_emitSignalEOA = false;
    m_frameDuration = 0;
    m_currentAnimationFrame = NO_CURRENT_FRAME;
    m_currentAnimationIndex = 0;

    // Par défaut, le sprite possède une (unique) liste d'animation
    addAnimation();

    m_customType = -1;
    connect(&m_animationTimer, &QTimer::timeout, this, &Sprite::onNextAnimationFrame);

#ifdef DEBUG_SPRITE_COUNT
    s_spriteCount++;
    displaySpriteCount();
#endif
}

//! Affiche l'image suivante de l'animation.
//! Si la dernière image est affichée, l'animation reprend au début et,
//! selon la configuration, le signal animationFinished() est émis.
void Sprite::onNextAnimationFrame() {
    if (m_animationList[m_currentAnimationIndex].isEmpty()) {
        m_currentAnimationFrame = NO_CURRENT_FRAME;
        return;
    }

    int PreviousAnimationFrame = m_currentAnimationFrame;
    ++m_currentAnimationFrame;
    if (m_currentAnimationFrame >= m_animationList[m_currentAnimationIndex].count()) {
        m_currentAnimationFrame = 0;
        if (m_emitSignalEOA)
            emit animationFinished();
        if (m_animationStopLater) {
            m_animationStopLater = false;
            stopAnimation(IMMEDIATE_STOP);
        }
    }
    if (PreviousAnimationFrame != m_currentAnimationFrame) {
        setPixmap(m_animationList[m_currentAnimationIndex][m_currentAnimationFrame]);
        update();
    }
}

#ifdef QT_DEBUG
QDebug operator<<(QDebug dbg, const Sprite& sprite) {
    QDebugStateSaver saver(dbg);

    dbg << "Sprite(brect : " << sprite.boundingRect();
    dbg << " global brect : " << sprite.globalBoundingRect();
    dbg << " x : " << sprite.x();
    dbg << " y : " << sprite.y();
    dbg << " left : " << sprite.left();
    dbg << " right : " << sprite.right();
    dbg << " top : " << sprite.top();
    dbg << " bottom : " << sprite.bottom();
    dbg << " width : " << sprite.width();
    dbg << " height : " << sprite.height();
    dbg << ")";
    return dbg;
}
#endif
