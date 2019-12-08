#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QPropertyAnimation>
#include <QPushButton>
#include <QProgressBar>
#include <QTimer>

class BackgroundItem;
class Player;
class Cactus;

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

protected:
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void initLevelOne();

private slots:
    void movePlayer();
    void checkTimer();
    void resetScene();

public slots:
    void updateHealthBar(int);

signals:
    void jumpFactorChanged(qreal);
    void healthBarChanged(int);
    void youLost();

private:
    QProgressBar *mHealthBar;

    Player* mPlayer;
    QGraphicsRectItem *mCacti;
    QGraphicsRectItem *mFloor;

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
    int mJumpHeight;
    bool mColliding;
    int mCollidingDirection;

    void jump();
    void addHorizontalInput(int input);
    void applyParallax(qreal ratio, QGraphicsItem *item);

    void checkColliding();
    bool checkCollidingH();
    bool checkCollidingV();
    void sinking();

};

#endif // GAMESCENE_H
