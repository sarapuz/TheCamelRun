#include "coin.h"

#include <QParallelAnimationGroup>
#include <QPen>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>


Coin::Coin(QGraphicsItem *parent) :
    QGraphicsEllipseItem(parent),
    mExplosion(false)
{
    setPen(QPen(QColor(218, 165, 32), 2));
    setBrush(QColor(255, 223, 0));
    setRect(-12, -12, 24, 24);
}


int Coin::type() const
{
    return Type;
}

void Coin::explode()
{
    if (mExplosion) {
        return;
    }

    mExplosion = true;
    QParallelAnimationGroup *group = new QParallelAnimationGroup(this);
    //QSequentialAnimationGroup *group = new QSequentialAnimationGroup(this);

    QPropertyAnimation *scaleAnimation = new QPropertyAnimation(this, "rect");
    scaleAnimation->setDuration(700);
    QRectF r = rect();
    scaleAnimation->setStartValue(r);
    scaleAnimation->setEndValue(QRectF(r.topLeft() - r.bottomRight(), r.size() * 2));
    scaleAnimation->setEasingCurve(QEasingCurve::OutQuart);
    group->addAnimation(scaleAnimation);

    QPropertyAnimation *fadeAnimation = new QPropertyAnimation(this, "opacity");
    fadeAnimation->setDuration(700);
    fadeAnimation->setStartValue(1);
    fadeAnimation->setEndValue(0);
    fadeAnimation->setEasingCurve(QEasingCurve::OutQuart);
    group->addAnimation(fadeAnimation);

    connect(group, &QParallelAnimationGroup::finished,
            this, &Coin::deleteLater);
    group->start();
}

bool Coin::explosion() const
{
    return mExplosion;
}

void Coin::setExplosion(bool explosion)
{
    mExplosion = explosion;
}
