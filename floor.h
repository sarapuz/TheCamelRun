#ifndef FLOOR_H
#define FLOOR_H

#include <QGraphicsPixmapItem>

/*!
  \class Floor
  \brief The Floor class represents floor in the game.

  Floor class defines floor object in the game. Floor is set during initialization of the levels.
  It is not set through all of the level, so the player can fall.
*/
class Floor : public QGraphicsPixmapItem
{
public:
    Floor(QGraphicsItem *parent = 0);

    enum { Type = UserType + 2 };
    int type() const;

public:
    bool mVisible;
   // virtual QPainterPath shape() const;

    bool visible() const;
    void setVisible(bool visible);
};

#endif // FLOOR_H
