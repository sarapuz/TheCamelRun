#include "platform.h"

Platform::Platform(QGraphicsItem *parent) :
    QGraphicsPixmapItem(parent)
{
    QPixmap pixmap("://");
    setPixmap(pixmap);
    setOffset(-pixmap.width() / 2, -pixmap.height() / 2);
}


int Platform::type() const
{
    return Type;
}
