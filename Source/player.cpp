#include "Header/player.h"


Player::Player(QGraphicsItem *parent)
    : QGraphicsPixmapItem(parent),
      mDirection(0),
      mCurrentHealth(10),
      mMaxHealth(10),
      mWealth(0),
      mSinking(false),
      mLive(true)
{
    QPixmap pixmap(":/camel.png");
    setPixmap(pixmap);
    setOffset(-pixmap.width() / 3, -pixmap.height() / 2);
}

int Player::direction() const
{
    return mDirection;
}

void Player::setDirection(int direction)
{
    mDirection = direction;
    if (mDirection != 0) {
        QTransform transform;
        if (mDirection < 0) {
            transform.scale(-1, 1);
        }
        setTransform(transform);
    }
}

int Player::currentHealth() const
{
    return mCurrentHealth;
}

void Player::setCurrentHealth(int health)
{
    if (mCurrentHealth == health) {
        return;
    }
    mCurrentHealth = health;
}

int Player::maxHealth() const
{
    return mMaxHealth;
}

void Player::setMaxHealth(int maxHealth)
{
    mMaxHealth = maxHealth;
}

bool Player::causeDamage(int damage)
{
    if (damage >= mCurrentHealth){
        mCurrentHealth = 0;
        return true;
        //endGame(false);
    }else if(damage > 0){
        setCurrentHealth(mCurrentHealth - damage);
        return true;
   }
    else
        return false;
}

int Player::wealth() const
{
    return mWealth;
}

void Player::addCoin(){
    mWealth++;
}

bool Player::sinking() const
{
    return mSinking;
}

void Player::setSinking(bool sinking)
{
    mSinking = sinking;
}

bool Player::live() const
{
    return mLive;
}

void Player::setLive(bool live)
{
    mLive = live;
}

void Player::setWealth(int wealth)
{
    mWealth = wealth;
}
