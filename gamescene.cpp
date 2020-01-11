#include "gamescene.h"
#include "backgrounditem.h"
#include "player.h"
#include "cactus.h"
#include "floor.h"
#include "tree.h"
#include "coin.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QEventLoop>
#include <QKeyEvent>
#include <QFont>
#include <QFontDatabase>
#include <QGraphicsEllipseItem>
#include <QSettings>



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
    mSlideAnimation(new QPropertyAnimation(this)),
    mJumpHeight(180),
    mColliding(false),
    mCollidingDirection(0),
    mLeftKey(Qt::Key_Left),
    mRightKey(Qt::Key_Right),
    mJumpKey(Qt::Key_Space)
{

    /* zbog Floora */
    mGroundLevel = mGroundLevel - 39;
    mMinY = mMinY - 39;
    /* */

    mTimer.setInterval(10);
    connect(&mTimer, &QTimer::timeout, this, &GameScene::movePlayer);

    //>

    mBackBtn = new QPushButton();
    mBkg = new BackgroundItem(QPixmap("://platformer_bkg.jpg"));
    mPlayer = new Player();
    mTree = new QGraphicsRectItem(0, 0, mFieldWidth, 300);
    mHealthBar = new QProgressBar();

    mCoinsLabel = new QLabel();
    mCoinsPicture = new QGraphicsEllipseItem();

    initPlayField();
    initLevelOne();

    mJumpAnimation->setTargetObject(this);
    mJumpAnimation->setPropertyName("jumpFactor");
    mJumpAnimation->setStartValue(0);
    mJumpAnimation->setKeyValueAt(0.5, 1);
    mJumpAnimation->setEndValue(0);
    mJumpAnimation->setDuration(800);
    mJumpAnimation->setEasingCurve(QEasingCurve::OutInQuad);

    mSlideAnimation->setTargetObject(this);
    mSlideAnimation->setPropertyName("slideFactor");
    mSlideAnimation->setStartValue(0);
    mSlideAnimation->setKeyValueAt(0.5, 0.5);
    mSlideAnimation->setEndValue(0);
    mSlideAnimation->setDuration(600);
    mSlideAnimation->setEasingCurve(QEasingCurve::OutInQuad);



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
    this->addItem(mBkg);
    //>

    int id = QFontDatabase::addApplicationFont("://NEWfONT2.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont fontFont(family, 15);

    // <Draw player
    mMinX = mPlayer ->boundingRect().width() * 0.5;
    mMaxX = mFieldWidth - mPlayer ->boundingRect().width() * 0.5;
    mPlayer ->setPos(mMinX, mGroundLevel - mPlayer ->boundingRect().height() / 2);
    mCurrentX = mMinX;
    mCurrentY = mGroundLevel - mPlayer ->boundingRect().height() / 2;
    //>

    // <Draw health bar
    mHealthBar->setMinimum(0);
    mHealthBar->setMaximum(mPlayer->maxHealth());
    mHealthBar->setValue(mPlayer->maxHealth());
    mHealthBar->setGeometry(this->width()/2 - mHealthBar->width()/2, 15, 150, 30);
    mHealthBar->setFont(fontFont);
    QPalette p = palette();
    p.setColor(QPalette::Highlight, Qt::darkGreen);
    mHealthBar->setPalette(p);
    this->addWidget(mHealthBar);
    //>


    fontFont.setPointSize(30);
    mCoinsLabel->setText(QString("x %1").arg(mPlayer->wealth()));
    mCoinsLabel->setGeometry(QRect(55,15,100,50));
    mCoinsLabel->setAttribute(Qt::WA_TranslucentBackground);
    mCoinsLabel->setFont(fontFont);
    mCoinsLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    this->addWidget(mCoinsLabel);


    mCoinsPicture->setPen(QPen(QColor(218, 165, 32), 2));
    mCoinsPicture->setBrush(QColor(255, 223, 0));
    mCoinsPicture->setRect(0, 0, 45, 45);
    mCoinsPicture->setPos(5,10);
    this->addItem(mCoinsPicture);
    // <Create
    mBackBtn->setText("Back");
    mBackBtn->setGeometry(this->width() - 100, 15, 90,30);
    mBackBtn->setEnabled(true);

    this->addWidget(mBackBtn);

    mTree->setPen(Qt::NoPen);
    mTree->setPos(0, mGroundLevel - 260);
    Tree *t= new Tree(mTree);
    t->setPos(mMaxX - t->boundingRect().width(), 0);


    this->addItem(mTree);
    this->addItem(mPlayer );
}

/*!   mStartBtn->setStyleSheet(
                  "background: transparent;"
                  "color: #000000;"
                  "border: 8px solid #000000;"

* \brief initiates floor holes, cacti and enemies
*
* GameScene::initLevelOne initiates floor holes, cacti and enemies.
 */
void GameScene::initLevelOne(){

    // Adding cacti to level ONE
    mCacti = new QGraphicsRectItem(0, 0, mFieldWidth, 100 );
    mCacti->setPen(Qt::NoPen);
    mCacti->setPos(0, mGroundLevel - 100);
    const int xRange = (mMaxX - mMinX - 200) * 0.94;
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

    mCoins = new QGraphicsRectItem(0,0,mFieldWidth, mJumpHeight);
    mCoins->setPen(Qt::NoPen);
    mCoins->setPos(0, mGroundLevel - mJumpHeight);
    for (int i = 0; i < 15; ++i) {
        Coin *co = new Coin(mCoins);
        co->setPos(mMinX + 200 + qrand() % xRange, qrand() % mJumpHeight - 39 - co->boundingRect().height());
    }
    addItem(mCoins);

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
    /*
    switch (event->key()) {
        case Qt::Key_Right:
            addHorizontalInput(1);
            break;
        case mLeftKey:
            addHorizontalInput(-1);
            break;
        case Qt::Key_Space:
            mVelocity = 3;
            jump();
            break;
        default:
            break;
    }
    */
    if(event->key() == mRightKey){
        addHorizontalInput(1);
    }
    else if(event->key() == mLeftKey){
        addHorizontalInput(-1);
    }
    else if(event->key() == mJumpKey){
        mVelocity = 3;
        jump();
    }
}

void GameScene::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) {
        return;
    }
    /*
    switch (event->key()) {
    case Qt::Key_Right:
        addHorizontalInput(-1);
        break;
    case Qt::Key_Left:
        addHorizontalInput(1);
        break;
    case Qt::Key_Space:
        mVelocity = 3;
        return;
    default:
        break;
    }
    */
    if(event->key() == mRightKey){
        addHorizontalInput(-1);
       // emit youWon();
    }
    else if(event->key() == mLeftKey){
        addHorizontalInput(1);
    }
    else if(event->key() == mJumpKey){
        mVelocity = 3;
        return;
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

    const int shiftBorder = 450;
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
    applyParallax(ratio, mTree);
    applyParallax(ratio, mCoins);
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
    delete mCacti;
    delete mFloor;
    delete mCoins;
    mPlayer->setDirection(0);
    mHorizontalInput=0;
    mGroundLevel = 583 - 39;
    mPlayer ->setPos(mMinX, mGroundLevel - mPlayer ->boundingRect().height() / 2);
    mCurrentX = mMinX;
    mCurrentY = mGroundLevel - mPlayer ->boundingRect().height() / 2;
    mPlayer->setCurrentHealth(mPlayer->maxHealth());
    mPlayer->setOffset(-mPlayer->pixmap().width() / 2, -mPlayer->pixmap().height() / 2);
    QTransform t = mPlayer->transform();
    t.reset();
    mPlayer->setTransform(t);
    mHealthBar->setValue(mPlayer->maxHealth());
    mColliding = false;
    mCollidingDirection = 1;

    initPlayField();
    initLevelOne();
}

void GameScene::updateCoinCounter()
{
    mCoinsLabel->setText(QString("x %1").arg(mPlayer->wealth()));
}

void GameScene::updateHealthBar(int)
{
    mHealthBar->setValue(mPlayer->currentHealth());
    //mHealthBar->
}

void GameScene::changeLeftKey(int newKey)
{
    if(newKey == mRightKey || newKey == mJumpKey )
        return;

    QSettings settings;
    settings.setValue("LeftKey", QVariant::fromValue(Qt::Key(newKey)).toString());
    mLeftKey = Qt::Key(newKey);
}

void GameScene::changeRightKey(int newKey)
{
    if(newKey == mLeftKey || newKey == mJumpKey )
        return;

    QSettings settings;
    settings.setValue("RightKey", QVariant::fromValue(Qt::Key(newKey)).toString());
    mRightKey = Qt::Key(newKey);
}

void GameScene::changeJumpKey(int newKey)
{
    if(newKey == mRightKey || newKey == mLeftKey )
        return;

    QSettings settings;
    settings.setValue("JumpKey", QVariant::fromValue(Qt::Key(newKey)).toString());
    mJumpKey = Qt::Key(newKey);
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

void GameScene::slide(){
    if (QAbstractAnimation::Stopped == mSlideAnimation->state()) {
        mSlideAnimation->start();
    }
}

qreal GameScene::jumpFactor() const
{
    return mJumpFactor;
}

qreal GameScene::slideFactor() const
{
    return mSlideFactor;
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

void GameScene::setSlideFactor(const qreal &slideFactor)
{

    if (mSlideFactor == slideFactor) {
        return;
    }

    mSlideFactor = slideFactor;
    emit slideFactorChanged(mSlideFactor);

    qreal groundY = (mMinY - mPlayer->boundingRect().height() / 2);
    qreal y = groundY - mSlideAnimation->currentValue().toReal() * mJumpHeight;
    mCurrentX += 2;
    mPlayer->setX(mCurrentX - mWorldShift);
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
        if (Coin *c = qgraphicsitem_cast<Coin*>(item)) {
            if(!c->explosion()){
                mPlayer->addCoin();
                emit coinGathered();
                c->explode();
            }
        }
        if (Floor *f = qgraphicsitem_cast<Floor*>(item)) {
            if (!f->visible()){
                sinking();
                return true;
            }
        }
        if (Cactus *c = qgraphicsitem_cast<Cactus*>(item)) {
            if(!mColliding && mPlayer->causeDamage(c->getDamage())){
                mColliding = true;
                mCollidingDirection = mPlayer->direction();
                emit healthBarChanged(c->getDamage());
                if(mPlayer->currentHealth() <= 0)
                    emit youLost();
            }
            return true;
        }
        if(Tree *t = qgraphicsitem_cast<Tree*>(item)){
            delay(15);
            emit youWon();
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
        if (Coin *c = qgraphicsitem_cast<Coin*>(item)) {
            if(!c->explosion()){
                mPlayer->addCoin();
                emit coinGathered();
                c->explode();
            }
        }
        if(Tree *t = qgraphicsitem_cast<Tree*>(item)){
            delay(15);
            emit youWon();
            return true;
        }
        if (Floor *f = qgraphicsitem_cast<Floor*>(item)) {
            if (!f->visible()){
                sinking();
            }
        }
        if (Cactus *c = qgraphicsitem_cast<Cactus*>(item)) {
            if(!mColliding && mPlayer->causeDamage(c->getDamage())){
                mGroundLevel = mMinY - c->boundingRect().height();
                mColliding = true;
                // simulacija preskakanja!
                slide();

                emit healthBarChanged(c->getDamage());
                if(mPlayer->currentHealth() <= 0){
                    emit youLost();
                }

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
 * \brief GameScene::sinking simulates sinking of camel
 */
void GameScene::sinking(){
    mBackBtn->setEnabled(false);

    // Rotacija
    mPlayer->setTransformationMode(Qt::SmoothTransformation);
    mPlayer->setOffset(10,-20);
    QTransform t = mPlayer->transform();
    for(int i = 0; i < 9; i++){
        t.rotate(10);
        mPlayer->setTransform(t);
        mCurrentX = mCurrentX + mPlayer->direction() * 10;
        mPlayer->setX(mCurrentX);
        mCurrentY = mCurrentY + 10;
        mPlayer->setY(mCurrentY++);
        delay(70);
    }
    //mPlayer->setOffset(-mPlayer->pixmap().width() / 2, -mPlayer->pixmap().height() / 2);
    // Propadanje
    while(mPlayer->y() - mPlayer->boundingRect().height() < mMinY){
        mPlayer->setY(mCurrentY++);
        mPlayer->causeDamage(mPlayer->currentHealth());
        emit healthBarChanged(mPlayer->currentHealth());
        delay(15);
    }
    emit youLost();
}

