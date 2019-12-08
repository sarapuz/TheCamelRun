#include "floor.h"

Floor::Floor(QGraphicsItem *parent) :
    QGraphicsPixmapItem(parent)
{
    QPixmap pixmap("://floor_tile.png");
    setPixmap(pixmap);
}

int Floor::type() const
{
    return Type;
}

bool Floor::visible() const
{
    return mVisible;
}

void Floor::setVisible(bool visible)
{
    mVisible = visible;
}

/*
QPainterPath Floor::shape() const
{
    return QPainterPath();
}
*/
