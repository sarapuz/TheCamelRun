#include "cactus.h"

#include <QDebug>


/*!
    \brief Cactus::Cactus(QGraphicsItem *parent)
    Constructs an instance of Cactus class.
 */
Cactus::Cactus(QGraphicsItem *parent) :
    QGraphicsPixmapItem(parent),
    mDamage(1)
{
    QPixmap pixmap("://cactus2.png");
    setPixmap(pixmap);
    setOffset(-pixmap.width() / 2, -pixmap.height() / 2);


    mHeight = pixmap.height();
    mWidth = pixmap.width();

}

int Cactus::getHeight() const
{
    return mHeight;
}


int Cactus::getWidth() const
{
    return mWidth;
}

void Cactus::mSetX(int)
{

}

int Cactus::type() const
{
    return Type;
}

int Cactus::getDamage() const
{
    return mDamage;
}

void Cactus::setDamage(int damage)
{
    mDamage = damage;
}
