#include "scorpio.h"
#include "player.h"


Scorpio::Scorpio(int position, int radius, QGraphicsItem *parent):
    QGraphicsPixmapItem(parent),
    mVelocity(1),
    mPositionY(0),
    mDirection(1),
    mDamage(1)
{
    mPositionX = position;
    mRadius = radius;
    QPixmap pixmap("://scorpio.png");
    setPixmap(pixmap);
    setOffset(-pixmap.width() / 2, -pixmap.height() / 2);
    mPositionY = 583-39 -pixmap.height()/2;
}

int Scorpio::type() const
{
    return Type;
}

int Scorpio::positionX() const
{
    return mPositionX;
}

void Scorpio::setPositionX(int positionX)
{
    mPositionX = positionX;
}

int Scorpio::direction() const
{
    return mDirection;
}

void Scorpio::setDirection(int direction)
{
    mDirection = direction;

}

int Scorpio::velocity() const
{
    return mVelocity;
}

void Scorpio::setVelocity(int velocity)
{
    mVelocity = velocity;
}

int Scorpio::positionY() const
{
    return mPositionY;
}

void Scorpio::setPositionY(int positionY)
{
    mPositionY = positionY;
}

int Scorpio::damage() const
{
    return mDamage;
}

void Scorpio::setDamage(int damage)
{
    mDamage = damage;
}
