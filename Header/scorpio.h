#ifndef SCORPIO_H
#define SCORPIO_H

#include "backgrounditem.h"

#include <QGraphicsPixmapItem>
#include <QTimer>

class Scorpio: public QGraphicsPixmapItem
{
public:
    explicit Scorpio(int positionX = 100, int radius = 100, QGraphicsItem *parent = nullptr);
    enum {Type = UserType + 5};
    int type() const;

    int positionX() const;
    void setPositionX(int positionX);
    int direction() const;
    void setDirection(int direction);
    int velocity() const;
    void setVelocity(int velocity);
    int positionY() const;
    void setPositionY(int positionY);
    int damage() const;
    void setDamage(int damage);

private:
    QTimer mTimer;
    int mVelocity;
    int mPositionX;
    int mPositionY;
    int mRadius;
    int mDirection;
    int mDamage;
};

#endif // SCORPIO_H
