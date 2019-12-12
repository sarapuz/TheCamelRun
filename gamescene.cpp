#include "gamescene.h"
#include "backgrounditem.h"
#include "player.h"
#include "cactus.h"
#include "floor.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QEventLoop>
#include <QKeyEvent>




GameScene::GameScene(QObject *parent) :
    QGraphicsScene(parent),
    mVelocity(3),
    mWorldShift(0),
    mMinX(0),
    mMaxX(0),
    mMinY(583),
    mFieldWidth(4992),
    mGroundLevel(583),
    mHorizontalInput(0),
    mJumpAnimation(new QPropertyAnimation(this)),
    mJumpHeight(180),
    mColliding(false),
    mCollidingDirection(0)
{
    /* zbog Floora */
    mGroundLevel = mGroundLevel - 39;
    mMinY = mMinY - 39;
    /* */

    mTimer.setInterval(10);
    connect(&mTimer, &QTimer::timeout, this, &GameScene::movePlayer);

    initPlayField();
    initLevelOne();

    mJumpAnimation->setTargetObject(this);
    mJumpAnimation->setPropertyName("jumpFactor");
    mJumpAnimation->setStartValue(0);
    mJumpAnimation->setKeyValueAt(0.5, 1);
    mJumpAnimation->setEndValue(0);
    mJumpAnimation->setDuration(800);
    mJumpAnimation->setEasingCurve(QEasingCurve::OutInQuad);

    //connect(this, SIGNAL(currentHealtChanged(qreal)), this, SLOT(updateHealthBar(qreal)));

}


/*!
 * \brief initialize play field with standard assets.
 *
 * GameScene::initPlayField initialize play field with standard assets such as
 * background, player, health bar, button for exiting the game
 */
void GameScene::initPlayField()
{
    // <Set background
    setSceneRect(0, 0, 1248, 585);
    mBkg = new BackgroundItem(QPixmap("://platformer_bkg.jpg"));
    addItem(mBkg);
    //>

    // <Set floor
    //?
    //>
    // <Draw player
    mPlayer = new Player();
    mMinX = mPlayer ->boundingRect().width() * 0.5;
    mMaxX = mFieldWidth - mPlayer ->boundingRect().width() * 0.5;
    mPlayer ->setPos(mMinX, mGroundLevel - mPlayer ->boundingRect().height() / 2);
    mCurrentX = mMinX;
    mCurrentY = mGroundLevel - mPlayer ->boundingRect().height() / 2;
    addItem(mPlayer );
    //>

    // <Draw health bar
    mHealthBar = new QProgressBar();
    mHealthBar->setMinimum(0);
    mHealthBar->setMaximum(mPlayer->maxHealth());
    mHealthBar->setValue(mPlayer->maxHealth());
    mHealthBar->setGeometry(this->width()/2 - mHealthBar->width()/2, 5, 150, 30);
    QPalette p = palette();
    p.setColor(QPalette::Highlight, Qt::darkGreen);
    mHealthBar->setPalette(p);
    this->addWidget(mHealthBar);
    //>

    // <Create buttons
    mBackBtn = new  QPushButton();
    mBackBtn->setText("Back");
    mBackBtn->setGeometry(this->width() - 100, 5, 90,30);
    this->addWidget(mBackBtn);
    //>


}

/*!
* \brief initiates floor holes, cacti and enemies
*
* GameScene::initLevelOne initiates floor holes, cacti and enemies.
 */
void GameScene::initLevelOne(){

    // Adding cacti to level ONE
    mCacti = new QGraphicsRectItem(0, 0, mFieldWidth, 100 );
    mCacti->setPen(Qt::NoPen);
    mCacti->setPos(0, mGroundLevel - 100);
    const int xRange = (mMaxX - mMinX) * 0.94;
    for (int i = 0; i < 5; ++i) {
        Cactus *c = new Cactus(mCacti);
          c->setPos(mMinX + 300 + qrand() % xRange, mCacti->boundingRect().height() - c->boundingRect().height()/2);
  //        c->setPos(mMinX + 200 + qrand() % xRange, mGroundLevel - c->boundingRect().height()/2);
    }
    this->addItem(mCacti);

    // Setting the floor and holes for level ONE
    mFloor = new QGraphicsRectItem(0,0,mFieldWidth,39);
    mFloor->setPen(Qt::NoPen);
    //mFloor->setPos(0, 2*mGroundLevel);
    int numOfTiles = mFieldWidth / 39;
    for(int i = 0; i < numOfTiles; i++){
        if( i < 10 || i > 13){
            Floor *f_tile = new Floor(mFloor);
            f_tile->setPos(i*39, mGroundLevel);
            f_tile->setVisible(true);
        }
        else{
            Floor *f_tile = new Floor(mFloor);
            f_tile->setPos(i*39, mGroundLevel);
            f_tile->setOpacity(0);
            f_tile->setVisible(false);
        }
    }
    this->addItem(mFloor);


}

void GameScene::addHorizontalInput(int input)
{
    mHorizontalInput += input;
    mPlayer->setDirection(qBound(-1, mHorizontalInput, 1));

    checkTimer();
}

void GameScene::keyPressEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) {
        return;
    }
    switch (event->key()) {
        case Qt::Key_Right:
            addHorizontalInput(1);
            break;
        case Qt::Key_Left:
            addHorizontalInput(-1);
            break;
        case Qt::Key_Space:
            mVelocity = 3;
            jump();
            break;
        case Qt::Key_1:
            //mHealthBar->setValue(3);
            if(mPlayer->causeDamage(2))
                emit healthBarChanged(2);
            break;
        default:
            break;
    }
}

void GameScene::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) {
        return;
    }
    switch (event->key()) {
    case Qt::Key_Right:
        addHorizontalInput(-1);
        break;
    case Qt::Key_Left:
        addHorizontalInput(1);
        break;
    case Qt::Key_Space:
        mVelocity = 2;
        return;
    default:
        break;
    }
}

/*!
 * \brief GameScene::movePlayer moves player left or right
 */
void GameScene::movePlayer()
{

    if(checkCollidingH() && mCollidingDirection == mPlayer->direction())
        return;

    const int direction = mPlayer->direction();
    if (0 == direction) {
        return;
    }
    const int dx = direction * mVelocity;
    qreal newX = qBound(mMinX, mCurrentX + dx, mMaxX);
    if (newX == mCurrentX) {
        return;
    }
    mCurrentX = newX;

    const int shiftBorder = 150;
    int rightShiftBorder = width() - shiftBorder;

    const int visiblePlayerPos = mCurrentX - mWorldShift;
    const int newWorldShiftRight = visiblePlayerPos - rightShiftBorder;
    if (newWorldShiftRight > 0) {
        mWorldShift += newWorldShiftRight;
    }
    const int newWorldShiftLeft = shiftBorder - visiblePlayerPos;
    if (newWorldShiftLeft > 0) {
        mWorldShift -= newWorldShiftLeft;
    }
    const int maxWorldShift = mFieldWidth - qRound(width());
    mWorldShift = qBound(0, mWorldShift, maxWorldShift);
    mPlayer->setX(mCurrentX - mWorldShift);
    //mPlayer->setY(mGroundLevel - mPlayer ->boundingRect().height() / 2);

    const qreal ratio = qreal(mWorldShift) / maxWorldShift;

    applyParallax(ratio, mBkg);
    applyParallax(ratio, mCacti);
    applyParallax(ratio, mFloor);


}

void GameScene::checkTimer()
{
    if (mPlayer->direction() == 0) {
        mTimer.stop();
    } else if (!mTimer.isActive()) {
        mTimer.start();
    }

}

/*!
 * \brief GameScene::resetScene resets scene for new game
 */
void GameScene::resetScene()
{
    mPlayer ->setPos(mMinX, mGroundLevel - mPlayer ->boundingRect().height() / 2);
    mCurrentX = mMinX;
    mCurrentY = mGroundLevel - mPlayer ->boundingRect().height() / 2;
    mPlayer->setCurrentHealth(mPlayer->maxHealth());
    mHealthBar->setValue(mPlayer->maxHealth());
    mColliding = false;
    mCollidingDirection = 1;
    mBackBtn->setEnabled(true);
}

void GameScene::updateHealthBar(int)
{
    mHealthBar->setValue(mPlayer->currentHealth());
    //mHealthBar->
}

int GameScene::collidingDirection() const
{
    return mCollidingDirection;
}

void GameScene::setCollidingDirection(int collidingDirection)
{
    mCollidingDirection = collidingDirection;
}

void GameScene::jump()
{
    if (QAbstractAnimation::Stopped == mJumpAnimation->state()) {
        mJumpAnimation->start();
    }    
}

qreal GameScene::jumpFactor() const
{
    return mJumpFactor;
}

/*!
 * \brief GameScene::setJumpFactor
 * \param jumpFactor
 */
void GameScene::setJumpFactor(const qreal &jumpFactor)
{

    if (mJumpFactor == jumpFactor) {
        return;
    }

    mJumpFactor = jumpFactor;
    emit jumpFactorChanged(mJumpFactor);
    checkCollidingV();

    qreal groundY = (mGroundLevel - mPlayer->boundingRect().height() / 2);
    qreal y = groundY - mJumpAnimation->currentValue().toReal() * mJumpHeight;
    mPlayer->setY(y);

}

/*!
 * \brief GameScene::applyParallax moves elements with the scene
 * \param ratio is ratio of moved item between element and scene
 * \param item is item which is being moved
 */
void GameScene::applyParallax(qreal ratio, QGraphicsItem* item) {
    item->setX(- ratio * (item->boundingRect().width() - width()));
}

QPainterPath GameScene::shape() const
{
    return QPainterPath();
}

void GameScene::checkColliding()
{
    for(QGraphicsItem* item: collidingItems(mPlayer)) {
        if (Cactus *c = qgraphicsitem_cast<Cactus*>(item)) {

            if(mPlayer->causeDamage(c->getDamage()))
                emit healthBarChanged(c->getDamage());
        }
    }
}

/*!
 * \brief GameScene::checkCollidingH checks horizontall collisions
 * \return \c true if colliding, \false if not
 */
bool GameScene::checkCollidingH()
{
    for(QGraphicsItem* item: collidingItems(mPlayer/*, Qt::ItemSelectionMode::IntersectsItemBoundingRect*/)) {
        if (Floor *f = qgraphicsitem_cast<Floor*>(item)) {
            if (!f->visible()){
                qDebug() << "Padanje";
                sinking();
                return true;
            }
        }
        if (Cactus *c = qgraphicsitem_cast<Cactus*>(item)) {
            if(!mColliding && mPlayer->causeDamage(c->getDamage())){
                qDebug() << "ccc" << c->getDamage();
                mColliding = true;
                mCollidingDirection = mPlayer->direction();
                emit healthBarChanged(c->getDamage());
                if(mPlayer->currentHealth() <= 0)
                    emit youLost();
            }
            return true;
        }
    }
    if (!mColliding && mGroundLevel!=mMinY){
        mGroundLevel = mMinY;
        mPlayer->setY(mGroundLevel - mPlayer ->boundingRect().height() / 2);
    }
    mColliding = false;
    mCollidingDirection = 0;
    return false;
}

/*!
 * \brief GameScene::checkCollidingV checks vertical collisions
 * \return \c true if colliding, \false if not
 */
bool GameScene::checkCollidingV()
{
    for(QGraphicsItem* item: collidingItems(mPlayer/*, Qt::ItemSelectionMode::IntersectsItemBoundingRect*/)) {
        if (Floor *f = qgraphicsitem_cast<Floor*>(item)) {
            if (!f->visible()){
                sinking();
            }
        }
        if (Cactus *c = qgraphicsitem_cast<Cactus*>(item)) {
            if(!mColliding && mPlayer->causeDamage(c->getDamage())){
                mGroundLevel = mMinY - c->boundingRect().height();
                qDebug() << "ccc" << c->getDamage();
                mColliding = true;
                emit healthBarChanged(c->getDamage());
                if(mPlayer->currentHealth() <= 0)
                    emit youLost();
            }
            return true;
        }
    }
    if (!mColliding){
        mGroundLevel = mMinY;
        mPlayer->setY(mGroundLevel - mPlayer ->boundingRect().height() / 2);
    }
    return false;
}


/*!
 * \brief delay pauses time
 * \param delayTime number of miliseconds to delay the time
 */
void delay(int delayTime){
    QTime dieTime = QTime::currentTime().addMSecs(delayTime);
    while(QTime::currentTime() < dieTime){
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
    }
}

/*!
 * \brief GameScene::sinking simulates sinking of camel
 */
void GameScene::sinking(){
    mBackBtn->setEnabled(false);
    while(mPlayer->y() - mPlayer->boundingRect().height() < mMinY){
        qDebug() << mPlayer->y();
        mPlayer->setY(mCurrentY++);
        mPlayer->causeDamage(mPlayer->currentHealth());
        emit healthBarChanged(mPlayer->currentHealth());
        delay(15);
    }
    emit youLost();
}

