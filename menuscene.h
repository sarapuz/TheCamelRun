#ifndef MENUSCENE_H
#define MENUSCENE_H

#include <QGraphicsScene>
#include <QPushButton>

class BackgroundItem;

class MenuScene : public QGraphicsScene
{
    Q_OBJECT

public:

    explicit MenuScene(QObject *parent = 0);

    BackgroundItem *mBkg;
    QPushButton *mStartBtn, *mSettingsBtn, *mHighBtn;
};

#endif // MENUSCENE_H
