#include "winningscene.h"
#include "backgrounditem.h"

#include <QFontDatabase>
#include <QKeyEvent>
#include <QRect>
#include <QDebug>

WinningScene::WinningScene(QObject *parent) : QGraphicsScene(parent)
{
    setSceneRect(0, 0, 1248, 585);
    mBkg = new BackgroundItem(QPixmap("://menuBkg.png"));
    addItem(mBkg);

    int id = QFontDatabase::addApplicationFont("://NEWfONT2.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont fontFont(family, 25);

    mCaptionLb = new QLabel();
    mCaptionLb->setText("Congrats! You saved this poor camel!");
    mCaptionLb->setFont(fontFont);
    mCaptionLb->setGeometry(QRect(this->width()/2 - 325, 50,650,80));
    mCaptionLb->setAttribute(Qt::WA_TranslucentBackground);
    mCaptionLb->setAlignment(Qt::AlignCenter );
    addWidget(mCaptionLb);

    mMsgLb = new QLabel();
    mMsgLb->setText("Enter your nickname!");
    fontFont.setPointSize(18);
    mMsgLb->setFont(fontFont);
    mMsgLb->setAttribute(Qt::WA_TranslucentBackground);
    mMsgLb->setGeometry(QRect(this->width()/2 - 325, 180,650,50));
    mMsgLb->setAlignment(Qt::AlignCenter);
    addWidget(mMsgLb);


    mNameLe = new QLineEdit();
    mNameLe->setFocus();
    mNameLe->setPlaceholderText("Name");
    mNameLe->setMaxLength(10);
    mNameLe->setGeometry(QRect(this->width()/2 - 150, 250, 300, 30));
    addWidget(mNameLe);

    mConfirmBtn = new QPushButton();
    mConfirmBtn->setStyleSheet("border-image: url(://SubmitBtn.png);");
    mConfirmBtn->setAttribute(Qt::WA_TranslucentBackground);
    mConfirmBtn->setGeometry(QRect(this->width()/2 - 40 , 325, 80, 80));
    addWidget(mConfirmBtn);

    connect(mConfirmBtn, SIGNAL(clicked()), this, SLOT(saveNameAndExit()));

    mNameLe->setFocus();
}

void WinningScene::saveNameAndExit()
{
    emit candidateForHighscore(mTempLvl, mNameLe->text(), mTempCoins);
    emit levelWon(mTempLvl);
    mNameLe->setText("");
}

