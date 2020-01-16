#include "cactus.h"

#include <QDebug>


/*!
    \brief Cactus::Cactus(QGraphicsItem *parent)
    Constructs an instance of Cactus class.
 */
Cactus::Cactus(QGraphicsItem *parent, int level) :
    QGraphicsPixmapItem(parent),
    mDamage(1)
{
    QString pixmapPath;
    switch (level) {
        case 1:{
            pixmapPath = ":/cactusLvl1.png";
            mDamage = 1;
            break;
        }
        case 2: {
            pixmapPath = ":/cactusLv2.png";
            mDamage = 2;
            break;
        }
        case 3:{
            pixmapPath = ":/cactusLv3.png";
            mDamage = 3;
            break;
        }
    }
    QPixmap pixmap(pixmapPath);
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
