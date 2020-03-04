#ifndef MENUSCENE_H
#define MENUSCENE_H

#include <QGraphicsScene>
#include <QLabel>
#include <QPushButton>

class BackgroundItem;

class MenuScene : public QGraphicsScene
{
public:
    explicit MenuScene(QObject *parent = 0);
    QPushButton *mStartBtn, *mSettingsBtn, *mHighBtn;

private:
    BackgroundItem *mBkg;
    BackgroundItem *mLogo;
    QLabel *mTitle;
};

#endif // MENUSCENE_H
