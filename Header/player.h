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
    bool sinking() const;
    void setSinking(bool sinking);
    bool live() const;
    void setLive(bool live);

private:
    int mDirection;
    int mCurrentHealth;
    int mMaxHealth;
    int mWealth;
    bool mSinking;
    bool mLive;
};

#endif // PLAYER_H
