#ifndef WINNINGSCENE_H
#define WINNINGSCENE_H

#include <QGraphicsScene>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>

class BackgroundItem;

class WinningScene:public QGraphicsScene
{
    Q_OBJECT
public:
    WinningScene(QObject *parent = 0);

    BackgroundItem *mBkg;
    QLabel *mCaptionLb;
    QLabel *mMsgLb;

    QLineEdit *mNameLe;
    QPushButton *mConfirmBtn;

    int mTempLvl;
    int mTempCoins;

protected:
    //void keyPressEvent(QKeyEvent *event);

signals:
    void backToMenu(); 
    void candidateForHighscore(int,QString,int);
    void levelWon(int);

public slots:
    void saveNameAndExit();
};

#endif // WINNINGSCENE_H
