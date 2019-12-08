#ifndef PLAYER_H
#define PLAYER_H

#include <QGraphicsPixmapItem>

class Player : public QGraphicsPixmapItem
{
 public:
    explicit Player(QGraphicsItem *parent = 0);

    int direction() const;
    void setDirection(int direction);

    int currentHealth() const;
    void setCurrentHealth(int health);

    int maxHealth() const;
    void setMaxHealth(int maxHealth);

    bool causeDamage(int);


private:
    int mDirection;
    int mCurrentHealth;
    int mMaxHealth;
};

#endif // PLAYER_H
