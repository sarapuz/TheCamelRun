#ifndef MENUSCENE_H
#define MENUSCENE_H

#include <QGraphicsScene>
#include <QLabel>
#include <QPushButton>

class BackgroundItem;

class MenuScene : public QGraphicsScene
{
    Q_OBJECT

public:

    explicit MenuScene(QObject *parent = 0);

    BackgroundItem *mBkg;
    QPushButton *mStartBtn, *mSettingsBtn, *mHighBtn;

    BackgroundItem *mLogo;
    QLabel *mTitle;
};

#endif // MENUSCENE_H
