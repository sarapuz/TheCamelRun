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

    int wealth() const;
    void setWealth(int wealth);

    bool causeDamage(int);
    void addCoin();

private:
    int mDirection;
    int mCurrentHealth;
    int mMaxHealth;
    int mWealth;
};

#endif // PLAYER_H
