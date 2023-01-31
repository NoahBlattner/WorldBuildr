/**
  \file
  \brief    Déclaration de la classe GameCanvas.
  \author   JCO
  \date     janvier 2015
*/
#include "gamecanvas.h"

#include "WorldBuildrGame/gamecore.h"
#include "gamescene.h"
#include "gameview.h"

#include <limits>

#include <QDebug>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsItem>
#include <QGraphicsTextItem>
#include <QKeyEvent>

const int DEFAULT_TICK_INTERVAL = 20;

#ifdef QT_DEBUG
const int STAT_TRIGGER_INTERVAL = 1000;
#endif

//!
//! Construit le canvas de jeu, qui se charge de faire l'interface entre GameView, GameScene et GameCore.
//! \param pView    La vue qui affiche les scènes du jeu.
//! \param pParent  Objet parent.
GameCanvas::GameCanvas(GameView* pView, QObject* pParent) : QObject(pParent) {
    m_pView = pView;
    m_pGameCore = nullptr;
    m_pDetailedInfosItem = nullptr;

    m_keepTicking = false;

    m_tickInterval = DEFAULT_TICK_INTERVAL;

    m_tickTimer.setSingleShot(false);
    m_tickTimer.setInterval(m_tickInterval);
    m_tickTimer.setTimerType(Qt::PreciseTimer); // Important pour avoir un précision suffisante sous Windows
    connect(&m_tickTimer, &QTimer::timeout, this, &GameCanvas::onTick);

    initDetailedInfos();

    // Il faut installer un filtre au niveau de GameView, afin de maîtriser complètement l'effet
    // des touches du clavier.
    // Filtrer au niveau de GameScene les touches du clavier est trop tard : QGraphicsView (et plus
    // particulièrement QAbstractScrollArea) prendraient alors en charge certaines touches
    // liées au scrolling : les flèches de gauche, droite, haut et bas ainsi que PageUp et PageDown.
    // Les autres événements, en particulier ceux liés à la souris, ne peuvent pas être traités
    // dans GameView, c'est trop tôt : Pour ces événements, QGraphicsView les transforme dans le système
    // de coordonnées de la scène avant de les transmettre à la scène. C'est une fois transformés
    // qu'il faut les filtrer.
    m_pView->installEventFilter(this);

    QTimer::singleShot(0, this, SLOT(onInit()));
}

//!
//! Destructeur.
//!
GameCanvas::~GameCanvas()
{
    stopTick();

    delete m_pGameCore;
    m_pGameCore = nullptr;
}

//! Construit une scène de jeu et lui installe un filtre d'événements pour intercepter les
//! événements clavier et souris.
//! La scène créée n'est pas automatiquement affichée.
//! \see setCurrentScene()
GameScene* GameCanvas::createScene() {
    GameScene* pScene = new GameScene(this);
    pScene->installEventFilter(this);
    return pScene;
}

//! Construit une scène de jeu et lui installe un filtre d'événements pour intercepter les
//! événements clavier et souris.
//! La scène créée n'est pas automatiquement affichée.
//! \see setCurrentScene()
GameScene* GameCanvas::createScene(const QRectF& rSceneRect) {
    GameScene* pScene = new GameScene(rSceneRect, this);
    pScene->installEventFilter(this);
    return pScene;
}

//! Construit une scène de jeu et lui installe un filtre d'événements pour intercepter les
//! événements clavier et souris.
//! La scène créée n'est pas automatiquement affichée.
//! \see setCurrentScene()
GameScene* GameCanvas::createScene(qreal x, qreal y, qreal width, qreal height) {
    GameScene* pScene = new GameScene(x, y, width, height, this);
    pScene->installEventFilter(this);
    return pScene;
}

//! Change la scène de jeu actuellement affichée.
void GameCanvas::setCurrentScene(GameScene* pScene) {
    m_pView->setScene(pScene);
}

//! \return un pointeur sur la scène qui est actuellement affichée par GameView.
GameScene* GameCanvas::currentScene() const {
    return static_cast<GameScene*>(m_pView->scene());
}

//! Détermine la scène qui sera affichée comme HUD.
//! \param pHudScene Scène à afficher comme HUD.
void GameCanvas::setHudScene(QGraphicsScene* pHudScene) {
    if (hudScene())
        hudScene()->removeItem(m_pDetailedInfosItem);

    m_pView->setHudScene(pHudScene);
    pHudScene->addItem(m_pDetailedInfosItem);
}

//! \return la scène utilisée comme HUD.
QGraphicsScene* GameCanvas::hudScene() const {
    return m_pView->hudScene();
}

//!
//! Démarre la génération d'un tick sur une base de temps régulière,
//! donnée en paramètre.
//! \param tickInterval  Intervalle de temps (en millisecondes) entre chaque tick. Si cette valeur est
//! inférieure à zéro, l'intervalle de temps précédent est utilisé.
//!
void GameCanvas::startTick(int tickInterval)  {
    if (tickInterval != KEEP_PREVIOUS_TICK_INTERVAL) {
        m_tickInterval = tickInterval;
        m_tickTimer.setInterval(tickInterval);
    }

#ifdef QT_DEBUG
    resetStatistics();
#endif
    m_keepTicking = true;
    m_lastUpdateTime.start();
    m_tickTimer.start();
}

//!
//! Arrête la génération du tick.
//!
void GameCanvas::stopTick()  {
    m_keepTicking = false;
    m_tickTimer.stop();
}

//!
//! \return un booléen indiquant si le tick est généré (true) ou non (false).
//!
bool GameCanvas::isTicking() const {
    return m_keepTicking;
}

//! Enclenche le suivi du déplacement de la souris.
void GameCanvas::startMouseTracking() {
    m_pView->setMouseTracking(true);
}

//! Déclenche le suivi du déplacement de la souris.
void GameCanvas::stopMouseTracking() {
    m_pView->setMouseTracking(false);
}

//!
//! \return la position actuelle de la souris, dans le système de coordonnées de la scène actuelle.
//!
QPointF GameCanvas::currentMousePosition() const
{
    return m_pView->mapToScene(m_pView->mapFromGlobal(QCursor::pos()));
}

//! Filtre et dispatch les événements intéressants de la scène.
bool GameCanvas::eventFilter(QObject* pWatched, QEvent* pEvent)
{
    // Les événements claviers doivent être traités au niveau de GameView, sinon, certaines touches
    // auront un effet indésirable pris en charge par QGraphicsView (ou plus exactement QAbstractScrollBar, qui
    // gère le comportement des touches fléchées et de PageUp et PageDown).
    // Pour que l'événement ne soit plus traité plus loin (et donc soit absorbé ici), on retourne true, comme
    // expliqué dans la doc de QObject::eventFilter() :
    //
    // "In your reimplementation of this function, if you want to filter the event out, i.e. stop it being handled
    // further, return true; otherwise return false."
    //
    if (qobject_cast<QGraphicsView*>(pWatched)) {
        switch (pEvent->type())  {
        case QEvent::KeyPress:   this->keyPressed(static_cast<QKeyEvent*>(pEvent));   return true;
        case QEvent::KeyRelease: this->keyReleased(static_cast<QKeyEvent *>(pEvent)); return true;
        default : return QObject::eventFilter(pWatched, pEvent);
        }
    }

    // Les événements restants proviennent de GameScene.
    // Attention : les événements de la souris, une fois traités par GameCanevas, doivent pouvoir être gérés plus
    // loin, par exemple dans le cas où la scène intègre des QWidget. Il faut donc retourner false.
    switch (pEvent->type())  {
    case QEvent::GraphicsSceneMouseMove:    this->mouseMoved(static_cast<QGraphicsSceneMouseEvent*>(pEvent));          return false;
    case QEvent::GraphicsSceneMousePress:   this->mouseButtonPressed(static_cast<QGraphicsSceneMouseEvent*>(pEvent));  return false;
    case QEvent::GraphicsSceneMouseRelease: this->mouseButtonReleased(static_cast<QGraphicsSceneMouseEvent*>(pEvent)); return false;
    default : return QObject::eventFilter(pWatched, pEvent);
    }
}

//! Initialise l'affichage des informations détaillées.
void GameCanvas::initDetailedInfos()
{
    m_pDetailedInfosItem = new QGraphicsTextItem("");
    m_pDetailedInfosItem->setDefaultTextColor(Qt::blue);
    m_pDetailedInfosItem->setPos(0,20);
    m_pDetailedInfosItem->setZValue(std::numeric_limits<qreal>::max()); // Toujours devant les autres items
    m_pDetailedInfosItem->hide();
    QFont textFont = m_pDetailedInfosItem->font();
    textFont.setPixelSize(15);
    m_pDetailedInfosItem->setFont(textFont);
}

//! Gère l'appui sur une touche du clavier.
//! Les répétitions automatiques sont ignorées.
void GameCanvas::keyPressed(QKeyEvent* pKeyEvent) {
    // Supprimer ce premier test si la répétition de touche doit être signalée.
    if (pKeyEvent->isAutoRepeat())
        pKeyEvent->ignore();
    else {
        m_pGameCore->keyPressed(pKeyEvent->key());

        if (pKeyEvent->modifiers()==(Qt::ShiftModifier|Qt::ControlModifier)) {
            switch (pKeyEvent->key()) {
            case Qt::Key_I:
                if (m_pDetailedInfosItem)
                    m_pDetailedInfosItem->setVisible(!m_pDetailedInfosItem->isVisible());
                break;
            case Qt::Key_P:
                m_tickTimer.setInterval(m_tickTimer.interval()+1);
                qDebug() << "Tick interval set to " << m_tickTimer.interval();
                break;
            case Qt::Key_M:
                m_tickTimer.setInterval(m_tickTimer.interval()-1);
                qDebug() << "Tick interval set to " << m_tickTimer.interval();
                break;
            }
        }
        pKeyEvent->accept();
    }
}

//! Gère le relâchement d'une touche du clavier.
void GameCanvas::keyReleased(QKeyEvent* pKeyEvent) {
    // Supprimer ce premier test si la répétition de touche doit être signalée.
    if (pKeyEvent->isAutoRepeat())
        pKeyEvent->ignore();
    else {
        m_pGameCore->keyReleased(pKeyEvent->key());
        pKeyEvent->accept();
    }
}

//! Gère le déplacement de la souris.
//! Pour que cet événement soit pris en compte, la propriété MouseTracking de GameView
//! doit être enclenchée.
void GameCanvas::mouseMoved(QGraphicsSceneMouseEvent* pMouseEvent) {
    m_pGameCore->mouseMoved(pMouseEvent->scenePos(), previousMousePosition);
    previousMousePosition = pMouseEvent->scenePos();
}

//! Gère l'événement d'appui sur un bouton de la souris.
void GameCanvas::mouseButtonPressed(QGraphicsSceneMouseEvent* pMouseEvent) {
    m_pGameCore->mouseButtonPressed(pMouseEvent->scenePos(), pMouseEvent->buttons());
    previousMouseButtons = pMouseEvent->buttons();
}

//! Gère l'événement de relâchement d'un bouton de la souris.
void GameCanvas::mouseButtonReleased(QGraphicsSceneMouseEvent* pMouseEvent) {
    // Retrouver les boutons qui ont été relâchés.
    Qt::MouseButtons releasedButtons = previousMouseButtons & ~pMouseEvent->buttons();
    m_pGameCore->mouseButtonReleased(pMouseEvent->scenePos(), releasedButtons);
}

//! Instancie un objet de type GameCore, en charge de la logique du jeu.
//! L'instanciation est faite uniquement lorsque tous les objets sont en place,
//! afin que GameCore n'appelle pas la fonction startTick alors que le signaux
//! ne sont pas encore connectés.
void GameCanvas::onInit() {
    // Mise en place d'un HUD par défaut
    QGraphicsScene* pHud = new QGraphicsScene(0,0, m_pView->width(), 50);
    setHudScene(pHud);

    m_pGameCore = new GameCore(this, this);
}


//! Traite le tick : le temps exact écoulé entre ce tick et le tick précédent
//! est mesuré et l'objet GameCore est lui-même informé du tick.
//! Poursuit la génération du tick si nécessaire.
void GameCanvas::onTick() {
    long long elapsedTime = m_lastUpdateTime.elapsed();

    // On évite une division par zéro (peu probable, mais on sait jamais)
    if (elapsedTime < 1)
        elapsedTime = 1;

    m_lastUpdateTime.start();

    // Statistiques
    m_tickCount++;
    m_totalElapsedTime += elapsedTime;

    // Tick
    m_pGameCore->tick(elapsedTime);
    currentScene()->tick(elapsedTime);

    if (m_pDetailedInfosItem && m_pDetailedInfosItem->isVisible())
        m_pDetailedInfosItem->setPlainText(QString("FPS : %1, Elapsed : %2ms, Tick duration : %3ms")
                                      .arg(1000/elapsedTime)
                                      .arg(elapsedTime)
                                      .arg(m_lastUpdateTime.elapsed()));

#ifdef QT_DEBUG
    // Statistiques
    m_statsTrigger -= elapsedTime;
    if (m_statsTrigger < 0) {
        //qDebug() << "Tick count :" << m_tickCount << ". Mean tick duration : " << m_totalElapsedTime / m_tickCount << ". Min duration : " << m_minTickDuration << ". Max duration : " << m_maxTickDuration;
        resetStatistics();
    }

    if (m_lastUpdateTime.elapsed() > m_maxTickDuration)
        m_maxTickDuration = m_lastUpdateTime.elapsed();
    else if (m_lastUpdateTime.elapsed() < m_minTickDuration)
        m_minTickDuration = m_lastUpdateTime.elapsed();
#endif
}

#ifdef QT_DEBUG
//! Remet à zéro les compteurs pour les statistiques en mode debug.
void GameCanvas::resetStatistics() {
    m_tickCount = 0;
    m_totalElapsedTime = 0;
    m_maxTickDuration = std::numeric_limits<int>::min();
    m_minTickDuration = std::numeric_limits<int>::max();

    m_statsTrigger = STAT_TRIGGER_INTERVAL;
}
#endif
