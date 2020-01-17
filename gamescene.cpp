#include "gamescene.h"
#include "backgrounditem.h"
#include "player.h"
#include "cactus.h"
#include "floor.h"
#include "tree.h"
#include "coin.h"
#include "scorpio.h"

#include <QCoreApplication>
#include <QDateTime>
#include <QDebug>
#include <QEventLoop>
#include <QKeyEvent>
#include <QFont>
#include <QFontDatabase>
#include <QGraphicsEllipseItem>
#include <QSettings>
#include <QRandomGenerator>
#include <QThreadPool>
#include <QtConcurrent>

GameScene::GameScene(QObject *parent) :
    QGraphicsScene(parent),
    mLevel(1),
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
    mGroundLevel = mGroundLevel - 39;
    mMinY = mMinY - 39;
    mTimer.setInterval(10);
    connect(&mTimer, &QTimer::timeout, this, &GameScene::movePlayer);
    mTimerEn.setInterval(10);

    mBackBtn = new QPushButton();
    mBkg = new BackgroundItem(QPixmap("://platformer_bkg.jpg"));
    mPlayer = new Player();
    mTree = new QGraphicsRectItem(0, 0, mFieldWidth, 300);
    mHealthBar = new QProgressBar();
    mCoinsLabel = new QLabel();
    mCoinsPicture = new QGraphicsEllipseItem();
    mLvlLabel = new QLabel();

    // <Set background
    setSceneRect(0, 0, 1248, 585);
    this->addItem(mBkg);
    //>

    // <Draw health bar
    int id = QFontDatabase::addApplicationFont("://NEWfONT2.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont fontFont(family, 15);
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

    // <Set coins label: presents number of coins acquired
    fontFont.setPointSize(30);
    mCoinsLabel->setText(QString("x %1").arg(mPlayer->wealth()));
    mCoinsLabel->setGeometry(QRect(55,15,120,50));
    mCoinsLabel->setAttribute(Qt::WA_TranslucentBackground);
    mCoinsLabel->setFont(fontFont);
    mCoinsLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    this->addWidget(mCoinsLabel);
    //>

    // <Picture of coin next to the coin label - clarification
    mCoinsPicture->setPen(QPen(QColor(218, 165, 32), 2));
    mCoinsPicture->setBrush(QColor(255, 223, 0));
    mCoinsPicture->setRect(0, 0, 45, 45);
    mCoinsPicture->setPos(5,10);
    this->addItem(mCoinsPicture);
    //>

    // <Create label which indicates which level is in progress
    mLvlLabel->setText(QString("Lvl %1").arg(mLevel));
    mLvlLabel->setFont(fontFont);
    mLvlLabel->setGeometry(QRect(width()/2, 15, 150, 50));
    mLvlLabel->setAttribute(Qt::WA_TranslucentBackground);
    addWidget(mLvlLabel);
    // >

    // <Create back button
    fontFont.setPointSize(20);
    mBackBtn->setStyleSheet("border-image: url(://back.png);");
    mBackBtn->setFont(fontFont);
    mBackBtn->setGeometry(this->width() - 110, 15, 80,80);
    mBackBtn->setEnabled(true);
    mBackBtn->setAttribute(Qt::WA_TranslucentBackground);
    this->addWidget(mBackBtn);
    //>

    // <Placing tree which represents end of level

    this->addItem(mTree);
    this->addItem(mPlayer );
    //>


/*    initPlayField();
    initLevelOne();
    */
    startLevel(mLevel);

    // Animation for jumping
    mJumpAnimation->setTargetObject(this);
    mJumpAnimation->setPropertyName("jumpFactor");
    mJumpAnimation->setStartValue(0);
    mJumpAnimation->setKeyValueAt(0.5, 1);
    mJumpAnimation->setEndValue(0);
    mJumpAnimation->setDuration(800);
    mJumpAnimation->setEasingCurve(QEasingCurve::OutInQuad);

    // Animation for sliding off cactus
    mSlideAnimation->setTargetObject(this);
    mSlideAnimation->setPropertyName("slideFactor");
    mSlideAnimation->setStartValue(0);
    mSlideAnimation->setKeyValueAt(0.5, 0.3);
    mSlideAnimation->setEndValue(0);
    mSlideAnimation->setDuration(600);
    mSlideAnimation->setEasingCurve(QEasingCurve::OutInQuad);

    connect(mJumpAnimation, &QAbstractAnimation::finished, this, &GameScene::checkCollidingV);
    connect(mSlideAnimation, &QAbstractAnimation::finished, this, &GameScene::checkCollidingV);
}


/*!
 * \brief initialize play field with standard assets.
 *
 * GameScene::initPlayField initialize position of player in regards to the scene
 */
void GameScene::initPlayField()
{
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

    // <Set position of tree
    mTree->setPen(Qt::NoPen);
    mTree->setPos(0, mGroundLevel - 260);
    Tree *t= new Tree(mTree);
    t->setPos(mMaxX - t->boundingRect().width(), 0);
    // >
    mLvlLabel->setText(QString("Lvl %1").arg(mLevel));
    mBackBtn->setEnabled(true);
}

/*!
* \brief initiates floor holes, cacti and enemies for level 1
*
* GameScene::initLevelOne initiates floor holes, cacti and enemies.
 */
void GameScene::initLevelOne(){


    // Setting scorpions
    mScorpios = new QGraphicsRectItem(0,0,mFieldWidth,60);
    mScorpios->setPen(Qt::NoPen);
    QList<int> scorpioPos({1150, 2050, 3300, 3900});
    for(int i = 0; i < 4; i++){
        Scorpio *s = new Scorpio(scorpioPos[i], 200 *(i+1), mScorpios);
        s->setPos(s->positionX(),s->positionY());
    }
    addItem(mScorpios);
    connect(&mTimerEn, &QTimer::timeout, this, &GameScene::moveScorpio);

    // Setting the floor and holes
    mFloor = new QGraphicsRectItem(0,0,mFieldWidth,39);
    mFloor->setPen(Qt::NoPen);
    QList<int> notVisible({10,11,12,13, 22,23,24, 39,40,41,42, 80,81,82});
    int numOfTiles = mFieldWidth / 39;
    for(int i = 0; i < numOfTiles; i++){
        if (notVisible.contains(i)){
            Floor *f_tile = new Floor(mFloor);
            f_tile->setPos(i*39, mGroundLevel);
            f_tile->setOpacity(0);
            f_tile->setVisible(false);
        }
        else{
            Floor *f_tile = new Floor(mFloor);
            f_tile->setPos(i*39, mGroundLevel);
            f_tile->setVisible(true);
        }
    }
    this->addItem(mFloor);

    // Adding cacti
    mCacti = new QGraphicsRectItem(0, 0, mFieldWidth, 100 );
    mCacti->setPen(Qt::NoPen);
    mCacti->setPos(0, mGroundLevel - 100);
    const int xRange = (mMaxX - mMinX - 200) * 0.94;
    QList<int> lowerBound({int(mMinX) + 100, 597, 996, 1678, 2200, 3300, 4000});
    QList<int> upperBound({             330, 788, 1481, 2190, 3090, 4000, xRange});
    QList<int> numOfCacti({1,1,2,2,3,2, 1});
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < numOfCacti[i]; j++){
            Cactus *c = new Cactus(mCacti, mGenerator.bounded(1,3));
            int temp = mGenerator.bounded(lowerBound[i], upperBound[i]);
            c->setPos(temp, mCacti->boundingRect().height() - c->boundingRect().height()/2);
        }
    }
    this->addItem(mCacti);

    mCoins = new QGraphicsRectItem(0,0,mFieldWidth, mJumpHeight);
    mCoins->setPen(Qt::NoPen);
    mCoins->setPos(0, mGroundLevel - mJumpHeight);
    for (int i = 0; i < 15; ++i) {
        Coin *co = new Coin(mCoins);
        co->setPos(mMinX + 200 + qrand() % xRange, qrand() % mJumpHeight - 39 - co->boundingRect().height());
    }
    addItem(mCoins);
}


/*! \brief initiates floor holes, cacti and enemies for level 2
*
* GameScene::initLevelOne initiates floor holes, cacti and enemies.
 */
void GameScene::initLevelTwo(){

    mScorpios = new QGraphicsRectItem(0,0,mFieldWidth,60);
    mScorpios->setPen(Qt::NoPen);
    QList<int> scorpioPos({1050,1700,2650,3500,3900});
    for(int i = 0; i < scorpioPos.length(); i++){
        Scorpio *s = new Scorpio(scorpioPos[i], 85 *(i+1), mScorpios);
        s->setPos(s->positionX(),s->positionY());
        s->setDamage(2);
    }
    addItem(mScorpios);


    // Adding cacti
    mCacti = new QGraphicsRectItem(0, 0, mFieldWidth, 100 );
    mCacti->setPen(Qt::NoPen);
    mCacti->setPos(0, mGroundLevel - 100);
    const int xRange = (mMaxX - mMinX - 200) * 0.94;
    QList<int> lowerBound({int(mMinX) + 100, 527, 976, 1678, 2224, 2600, 3300, 4000});
    QList<int> upperBound(             {310, 818, 1471,2005, 2600, 3090, 4000, xRange});
    QList<int> numOfCacti({1,2,1,2,2,2,2,3});
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < numOfCacti[i]; j++){
            Cactus *c = new Cactus(mCacti, mGenerator.bounded(1,4));
            int temp = mGenerator.bounded(lowerBound[i], upperBound[i]);
            c->setPos(temp, mCacti->boundingRect().height() - c->boundingRect().height()/2);
        }
    }
    this->addItem(mCacti);
    // Setting the floor and holes for level ONE
    mFloor = new QGraphicsRectItem(0,0,mFieldWidth,39);
    mFloor->setPen(Qt::NoPen);
    QList<int> notVisible({10,11,12,13, 22,23,24, 39,40,41,42, 55,56, 80,81,82,83});
    int numOfTiles = mFieldWidth / 39;
    for(int i = 0; i < numOfTiles; i++){
        if (notVisible.contains(i)){
            Floor *f_tile = new Floor(mFloor);
            f_tile->setPos(i*39, mGroundLevel);
            f_tile->setOpacity(0);
            f_tile->setVisible(false);
        }
        else{
            Floor *f_tile = new Floor(mFloor);
            f_tile->setPos(i*39, mGroundLevel);
            f_tile->setVisible(true);
        }
    }
    this->addItem(mFloor);

    mCoins = new QGraphicsRectItem(0,0,mFieldWidth, mJumpHeight);
    mCoins->setPen(Qt::NoPen);
    mCoins->setPos(0, mGroundLevel - mJumpHeight);
    for (int i = 0; i < 35; ++i) {
        Coin *co = new Coin(mCoins);
        co->setPos(mMinX + 200 + qrand() % xRange, qrand() % mJumpHeight - 39 - co->boundingRect().height());
    }
    addItem(mCoins);
}

void GameScene::startLevel(int lvl){
    //mPlayer->setLive(true);
    initPlayField();

    switch (lvl) {
        case 1:{
            initLevelOne();
            break;
        }
        case 2:{
            initLevelTwo();
            break;
        }
    }
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
    if(event->key() == mRightKey){
        addHorizontalInput(1);
    }
    else if(event->key() == mLeftKey){
        addHorizontalInput(-1);
    }
    else if(event->key() == mJumpKey){
        mVelocity = 3;
        if(!mPlayer->sinking()){
            jump();
        }
    }
}

void GameScene::keyReleaseEvent(QKeyEvent *event)
{
    if (event->isAutoRepeat()) {
        return;
    }
    if(event->key() == mRightKey){
        addHorizontalInput(-1);
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
    if((checkCollidingH() && mCollidingDirection == mPlayer->direction()) || !mPlayer->live())
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
    applyParallax(ratio, mScorpios);

    if(mCurrentX >= (mMaxX - 100)){
        mPlayer->setLive(false);
        emit youWon(mLevel, mPlayer->wealth());
    }
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
    mPlayer->setLive(false);

    mTimerEn.stop();

    delete mCacti;
    delete mFloor;
    delete mCoins;
    delete mScorpios;
    mPlayer->setDirection(0);
    mHorizontalInput = 0;
    mGroundLevel = 583 - 39;

    mPlayer->setPos(mMinX, mGroundLevel - mPlayer ->boundingRect().height() / 2);
    mCurrentX = mMinX;
    mCurrentY = mGroundLevel - mPlayer ->boundingRect().height() / 2;    
    mPlayer->setOffset(-mPlayer->pixmap().width() / 2, -mPlayer->pixmap().height() / 2);
    QTransform t = mPlayer->transform();
    t.reset();
    mPlayer->setTransform(t);
    mColliding = false;
    mCollidingDirection = 1;
    applyParallax(1, mBkg);
}

/*! resets Player for new game
 *
 * used when the game is lost, so the coins and health are reset to initial values
 */
void GameScene::resetPlayer(){
    mPlayer->setSinking(false);
    mPlayer->setWealth(0);
    emit coinGathered();
    mPlayer->setCurrentHealth(mPlayer->maxHealth());
    mHealthBar->setValue(mPlayer->maxHealth());
    setLevel(1);
}

void GameScene::updateCoinCounter()
{
    mCoinsLabel->setText(QString("x %1").arg(mPlayer->wealth()));
}
/*Cac
void GameScene::moveScorpio()
{
    if (!mPlayer->live())
        return;
    for(QGraphicsItem* scorpion : mScorpios->childItems()){
        if (!mPlayer->live())
            return;
        if( Scorpio *scorp = qgraphicsitem_cast<Scorpio*>(scorpion)) {

            for(int i = 0; i < scorp->radius(); i++){
                if (!mPlayer->live())
                    return;
                scorp->setPositionX(scorp->positionX() + scorp->velocity() * scorp->direction());
                if (!mPlayer->live())
                    return;
                scorp->setX(scorp->positionX());
                delay(10);
            }
            scorp->setDirection(scorp->direction()* (-1));
        }
    }
}
*/

void GameScene::moveScorpio(){

    for(int i = 0; i < 300; i++){
        for(QGraphicsItem* scorpion : mScorpios->childItems()){
            if (!mPlayer->live())
                return;
            if( Scorpio *scorp = qgraphicsitem_cast<Scorpio*>(scorpion)) {
                if (!mPlayer->live())
                    return;
                scorp->setPositionX(scorp->positionX() + scorp->velocity() * scorp->direction());
                if (!mPlayer->live())
                    return;
                scorp->setX(scorp->positionX());
            }
        }
        delay(10);
    }

    for(QGraphicsItem* scorpion : mScorpios->childItems()){
        if (!mPlayer->live())
            return;
        if( Scorpio *scorp = qgraphicsitem_cast<Scorpio*>(scorpion)) {
            if (!mPlayer->live())
                return;
            scorp->setDirection(scorp->direction()* (-1));
        }
    }
}


void GameScene::updateHealthBar(int)
{
    mHealthBar->setValue(mPlayer->currentHealth());
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

int GameScene::level() const
{
    return mLevel;
}

void GameScene::setLevel(int level)
{
    mLevel = level;
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

void GameScene::slide()
{
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
    mCurrentX += 1;
    mPlayer->setX(mCurrentX - mWorldShift);
    mPlayer->setY(y);

}

/*!
 * \brief GameScene::applyParallax moves elements with the scene
 * \param ratio is ratio of moved item between element and scene
 * \param item is item which is being moved
 */
void GameScene::applyParallax(qreal ratio, QGraphicsItem* item)
{
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
        if (Coin *c = dynamic_cast<Coin*>(item)) {  //
            if(!c->explosion()){
                mPlayer->addCoin();
                emit coinGathered();
                c->explode();
            }
        }
        else if (Floor *f = dynamic_cast<Floor*>(item)) {
            if (!f->visible() && mPlayer->live()){
                sinking();
                return true;
            }
        }
        else if (Cactus *c = dynamic_cast<Cactus*>(item)) {
            if(!mColliding && mPlayer->causeDamage(c->getDamage())){
                mColliding = true;
                mCollidingDirection = mPlayer->direction();
                emit healthBarChanged(c->getDamage());
                if(mPlayer->currentHealth() <= 0) {
                    mPlayer->setLive(false);
                    disconnect(&mTimerEn, &QTimer::timeout, this, &GameScene::moveScorpio);
                    emit youLost();
                }
            }
            return true;
        }/*
        else if(Scorpio *s = qgraphicsitem_cast<Scorpio*>(item)){
            mPlayer->causeDamage(s->damage());
            emit healthBarChanged(s->damage());
            if(mPlayer->currentHealth() <= 0) {
                mPlayer->setLive(false);
                emit youLost();
            }
        }*/
        /*
        else if(Tree *t = qgraphicsitem_cast<Tree*>(item)){
            emit youWon(mLevel, mPlayer->wealth());
            return true;
        }
        */
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
        if (Coin *c = dynamic_cast<Coin*>(item)) {
            if(!c->explosion()){
                mPlayer->addCoin();
                emit coinGathered();
                c->explode();
            }
        }/*
        else if(Tree *t = qgraphicsitem_cast<Tree*>(item)){
            delay(15);
            emit youWon(mLevel, mPlayer->wealth());
            return true;
        }*/
        else if (Floor *f = qgraphicsitem_cast<Floor*>(item)) {
            if (!f->visible() && mPlayer->live()){
                sinking();    
            }
            return true;
        }
        else if (Cactus *c = qgraphicsitem_cast<Cactus*>(item)) {
            if(!mColliding && mPlayer->causeDamage(c->getDamage())){
                mGroundLevel = mMinY - c->boundingRect().height();
                mColliding = true;
                // simulacija preskakanja!
                slide();

                emit healthBarChanged(c->getDamage());
                if(mPlayer->currentHealth() <= 0){
                    mPlayer->setLive(false);
                    disconnect(&mTimerEn, &QTimer::timeout, this, &GameScene::moveScorpio);
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
void GameScene::sinking()
{
    mPlayer->setLive(false);
    mBackBtn->setEnabled(false);
    mPlayer->setSinking(true);
    // Rotacija
    mPlayer->setTransformationMode(Qt::SmoothTransformation);
    mPlayer->setOffset(10,-20);
    QTransform t = mPlayer->transform();
    for(int i = 0; i < 9; i++){
        t.rotate(10);
        mPlayer->setTransform(t);
        mCurrentX = mCurrentX + mPlayer->direction() * 10;
        mPlayer->setX(mCurrentX - mWorldShift);
        mCurrentY = mCurrentY + 10;
        mPlayer->setY(mCurrentY++);
        delay(70);
    }

    while(mPlayer->y() - mPlayer->boundingRect().height() < mMinY){
        mPlayer->setY(mCurrentY++);
        mPlayer->causeDamage(mPlayer->currentHealth());
        emit healthBarChanged(mPlayer->currentHealth());
        delay(15);
    }
    disconnect(&mTimerEn, &QTimer::timeout, this, &GameScene::moveScorpio);
    emit youLost();
}

void GameScene::revivePlayer()
{
    mPlayer->setLive(true);
}

void GameScene::killPlayer()
{
    mPlayer->setLive(false);
    disconnect(&mTimerEn, &QTimer::timeout, this, &GameScene::moveScorpio);
    delay(100);
}
