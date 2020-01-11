#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QProgressBar>
#include <QTimer>
#include <QLabel>

class BackgroundItem;
class Player;
class Cactus;
class Tree;

/*!
  \class GameScene
  \brief The GameScene class hold the whole game.

  This is long description.
*/
class GameScene : public QGraphicsScene
{
    Q_OBJECT
    Q_PROPERTY(qreal jumpFactor
               READ jumpFactor
               WRITE setJumpFactor
               NOTIFY jumpFactorChanged)
    Q_PROPERTY(qreal slideFactor
               READ slideFactor
               WRITE setSlideFactor
               NOTIFY slideFactorChanged)
public:
    explicit GameScene(QObject *parent = 0);

    BackgroundItem *mBkg;
    QPushButton *mBackBtn;

    void initPlayField();
    qreal jumpFactor() const;
    void setJumpFactor(const qreal &jumpFactor);

    QPainterPath shape() const;

    int collidingDirection() const;
    void setCollidingDirection(int collidingDirection);
    qreal slideFactor() const;

    void setSlideFactor(const qreal &slideFactor);
protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void initLevelOne();

private slots:
    void movePlayer();
    void checkTimer();
    void resetScene();
    void updateCoinCounter();

public slots:
    void updateHealthBar(int);
    void changeLeftKey(int);
    void changeRightKey(int);
    void changeJumpKey(int);

signals:
    void jumpFactorChanged(qreal);
    void slideFactorChanged(qreal);
    void healthBarChanged(int);
    void youLost();
    void youWon();
    void coinGathered();

private:

    Player* mPlayer;
    QGraphicsRectItem *mCacti;
    QGraphicsRectItem *mFloor;
    QGraphicsRectItem *mTree;
    QGraphicsRectItem *mCoins;

    QProgressBar *mHealthBar;
    QGraphicsEllipseItem *mCoinsPicture;
    QLabel *mCoinsLabel;

    QTimer mTimer;
    int mVelocity;
    int mWorldShift;
    qreal mMinX, mMaxX, mMinY;
    qreal mFieldWidth, mGroundLevel;
    qreal mCurrentX;
    qreal mCurrentY;
    int mHorizontalInput;
    qreal mJumpFactor;
    QPropertyAnimation *mJumpAnimation;
    qreal mSlideFactor;
    QPropertyAnimation *mSlideAnimation;
    int mJumpHeight;
    bool mColliding;
    int mCollidingDirection;

    Qt::Key mLeftKey;
    Qt::Key mRightKey;
    Qt::Key mJumpKey;


    void jump();
    void addHorizontalInput(int input);
    void applyParallax(qreal ratio, QGraphicsItem *item);

    void checkColliding();
    bool checkCollidingH();
    bool checkCollidingV();
    void sinking();
    void slide();

};

#endif // GAMESCENE_H
