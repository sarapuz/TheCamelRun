#ifndef TREE_H
#define TREE_H

#include <QGraphicsPixmapItem>
#include "backgrounditem.h"


class Tree : public QGraphicsPixmapItem
{
public:
    explicit Tree(QGraphicsItem *parent = 0);
    enum { Type = UserType + 3};
    int type() const;

};

#endif // TREE_H
