#ifndef HIGHSCORESCENE_H
#define HIGHSCORESCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QPushButton>

class BackgroundItem;

// TODO
class HighscoreScene : public QGraphicsScene
{
    Q_OBJECT

public:

    explicit HighscoreScene(QObject *parent = 0);

    BackgroundItem *mBkg;
    QPushButton *mBackBtn;
};

#endif // HIGHSCORESCENE_H
