#ifndef PLATFORM_H
#define PLATFORM_H
#include <QGraphicsPixmapItem>

class Platform : public QGraphicsPixmapItem
{
public:
    explicit Platform(QGraphicsItem *parent = 0);

    enum { Type = UserType + 1 };
    int type() const;

};

#endif // PLATFORM_H
