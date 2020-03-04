#include "tree.h"

Tree::Tree(QGraphicsItem *parent):
    QGraphicsPixmapItem(parent)
{
    QPixmap pixmap("://tree.png");
    setPixmap(pixmap);
}

int Tree::type() const
{
    return Type;
}
