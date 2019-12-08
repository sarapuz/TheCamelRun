#ifndef CACTUS_H
#define CACTUS_H

#include <QGraphicsPixmapItem>
#include "backgrounditem.h"


/*!
  \class Cactus
  \brief The Cactus class represents cactus objects in the game.

  Cactus class defines cactus objects in the game. Cacti have damage, which damages player when colliding with it.
  Cactus class is  derived from QGraphicsPixmapItem class, which is derived from QGraphicsItems class.
*/

class Cactus : public QGraphicsPixmapItem
{

public:
    explicit Cactus(QGraphicsItem *parent = 0);

    int getHeight() const;
    int getWidth() const;
    void mSetX(int);


    int getDamage() const;
    void setDamage(int damage);

    enum { Type = UserType + 1 };
    int type() const;


private:
    int mHeight;
    int mWidth;
    int mDamage;
};

#endif // CACTUS_H
