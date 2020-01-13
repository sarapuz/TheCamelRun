#ifndef HIGHSCORESCENE_H
#define HIGHSCORESCENE_H

#include <QObject>
#include <QGraphicsScene>
#include <QPushButton>
#include <QLabel>
#include <QTableWidget>

class BackgroundItem;

// TODO
class HighscoreScene : public QGraphicsScene
{
    Q_OBJECT

public:

    explicit HighscoreScene(QObject *parent = 0);

    BackgroundItem *mBkg;
    QPushButton *mBackBtn;

    // Mozda spremati i kao parove?
    QList<QPair<QString, int> > mScoreLv1;
    QList<QPair<QString, int> > mScoreLv2;

    QLabel *mHeadingLb;

    QLabel *mHeadingLv1Lb;
    QLabel *mHeadingLv2Lb;

    QTableWidget *mLv1Tb;
    QTableWidget *mLv2Tb;

    void updateHighscore();

public slots:
    void updateTables(int lvl, QString name, int score);
};

#endif // HIGHSCORESCENE_H
