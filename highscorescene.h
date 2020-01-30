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

    QPushButton *mBackBtn;
    QList<QPair<QString, int> > mScoreLv1;
    QList<QPair<QString, int> > mScoreLv2;
    QTableWidget *mLv1Tb;
    QTableWidget *mLv2Tb;
    QPushButton *mResetBtn;

    void updateHighscore();

private:
    BackgroundItem *mBkg;
    QLabel *mHeadingLb;
    QLabel *mHeadingLv1Lb;
    QLabel *mHeadingLv2Lb;
    QLabel *mResetLb;

public slots:
    void updateTables(int lvl, QString name, int score);
    void resetTables();
};

#endif // HIGHSCORESCENE_H
