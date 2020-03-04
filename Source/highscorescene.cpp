#include "Header/highscorescene.h"
#include "Header/backgrounditem.h"

#include <QFontDatabase>
#include <QHeaderView>
#include <QSettings>
#include <QDebug>
#include <QPair>
#include <QMessageBox>

HighscoreScene::HighscoreScene(QObject *parent) : QGraphicsScene(parent)
{
      setSceneRect(0, 0, 1248, 585);
      mBkg = new BackgroundItem(QPixmap("://menuBkg.png"));
      addItem(mBkg);

      int id = QFontDatabase::addApplicationFont("://NEWfONT2.ttf");
      QString family = QFontDatabase::applicationFontFamilies(id).at(0);
      QFont fontFont(family, 25);

      mBackBtn = new QPushButton();
      mBackBtn->setStyleSheet("QPushButton{border-image: url(://BackBtn.png);}\nQPushButton:hover{border-image: url(://BackBtnHv.png);}");
      mBackBtn->setAttribute(Qt::WA_TranslucentBackground);
      mBackBtn->setGeometry(this->width() - 100, 15, 80,80);

      // Heading of scene
      mHeadingLb = new QLabel();
      mHeadingLb->setText("Highscores");
      mHeadingLb->setFont(fontFont);
      mHeadingLb->setAttribute(Qt::WA_TranslucentBackground);
      mHeadingLb->setGeometry(QRect(this->width()/2 - mHeadingLb->width()/2, height()/8, mHeadingLb->width(),40));
      mHeadingLb->setAlignment(Qt::AlignCenter);

      QSettings settings;

      // Heading of scores for Level 1
      mHeadingLv1Lb = new QLabel();
      mHeadingLv1Lb->setText("Level 1");
      mHeadingLv1Lb->setFont(fontFont);
      mHeadingLv1Lb->setAttribute(Qt::WA_TranslucentBackground);
      mHeadingLv1Lb->setGeometry(QRect(width()/2 - 300, height()/3 - 40, mHeadingLv1Lb->width(),40));
      mHeadingLv1Lb->setAlignment(Qt::AlignLeft);
      // Heading for scores for level2
      mHeadingLv2Lb = new QLabel();
      mHeadingLv2Lb->setText("Level 2");
      mHeadingLv2Lb->setFont(fontFont);
      mHeadingLv2Lb->setAttribute(Qt::WA_TranslucentBackground);
      mHeadingLv2Lb->setGeometry(QRect(width()/2 + 100, height()/3 - 40, mHeadingLv1Lb->width(),40));
      mHeadingLv2Lb->setAlignment(Qt::AlignLeft);

      fontFont.setPointSize(20);

      // Table with best results for level 1
      mLv1Tb = new QTableWidget();
      mLv1Tb->setRowCount(5);
      mLv1Tb->setColumnCount(2);
      mLv1Tb->setShowGrid(false);
      mLv1Tb->setGeometry(width()/2 - 300, height()/3 + 20, 300, height()/2);
      mLv1Tb->verticalHeader()->setVisible(false);
      mLv1Tb->horizontalHeader()->setVisible(false);
      mLv1Tb->setStyleSheet(
                  "QTableWidget {background-color: transparent;}"
                  "QHeaderView::section {background-color: transparent;}"
                  "QHeaderView {background-color: transparent;}"
                  "QTableCornerButton::section {Background-color: transparent;}"
                  "QTableWidget::item {padding:10px;}");
      mLv1Tb->setFont(fontFont);
      for(int i = 0; i < 5; i++ ){
            mLv1Tb->setItem(i,0, new QTableWidgetItem(settings.value(QString("Lvl1_%1").arg(i+1), "").toStringList()[0]));
            mLv1Tb->setItem(i,1, new QTableWidgetItem(settings.value(QString("Lvl1_%1").arg(i+1), "").toStringList()[1]));
            (mLv1Tb->item(i,0))->setFlags(Qt::ItemIsEnabled);
            (mLv1Tb->item(i,1))->setFlags(Qt::ItemIsEnabled);
            QPair<QString, int> onePair(settings.value(QString("Lvl1_%1").arg(i+1), "").toStringList()[0], settings.value(QString("Lvl1_%1").arg(i+1), "").toStringList()[1].toInt());
            mScoreLv1.append(onePair);
      }
      mLv1Tb->setSelectionMode(QAbstractItemView::NoSelection);
      mLv1Tb->resizeColumnsToContents();

      // Table with best scores for level 2
      mLv2Tb = new QTableWidget();
      mLv2Tb->setRowCount(5);
      mLv2Tb->setColumnCount(2);
      mLv2Tb->setShowGrid(false);
      mLv2Tb->setGeometry(width()/2 + 100, height()/3 + 20, 300, height()/2);
      mLv2Tb->verticalHeader()->setVisible(false);
      mLv2Tb->horizontalHeader()->setVisible(false);
      mLv2Tb->setSelectionMode(QAbstractItemView::NoSelection);
      mLv2Tb->setStyleSheet(
                  "QTableWidget {background-color: transparent;}"
                  "QHeaderView::section {background-color: transparent;}"
                  "QHeaderView {background-color: transparent;}"
                  "QTableCornerButton::section {Background-color: transparent;}"
                  "QTableWidget::item {padding:10px;}");
      mLv2Tb->setFont(fontFont);
      for(int i = 0; i < 5; i++ ){
            mLv2Tb->setItem(i,0, new QTableWidgetItem(settings.value(QString("Lvl2_%1").arg(i+1), "").toStringList()[0]));
            mLv2Tb->setItem(i,1, new QTableWidgetItem(settings.value(QString("Lvl2_%1").arg(i+1), "").toStringList()[1]));
            (mLv2Tb->item(i,0))->setFlags(Qt::ItemIsEnabled);
            (mLv2Tb->item(i,1))->setFlags(Qt::ItemIsEnabled);
            QPair<QString, int> onePair(settings.value(QString("Lvl2_%2").arg(i+1), "").toStringList()[0], settings.value(QString("Lvl2_%1").arg(i+1), "").toStringList()[1].toInt());
            mScoreLv2.append(onePair);
      }
      mLv2Tb->resizeColumnsToContents();

      mLv1Tb->setFocusPolicy(Qt::NoFocus);
      mLv2Tb->setFocusPolicy(Qt::NoFocus);

      mResetLb = new QLabel();
      fontFont.setPointSize(15);
      mResetLb->setText("Reseting scores");
      mResetLb->setFont(fontFont);
      mResetLb->setAttribute(Qt::WA_TranslucentBackground);
      mResetLb->setGeometry(QRect(this->width()/2 - 150, height() -  135, 300,100));
      mResetLb->setAlignment(Qt::AlignCenter);

      mResetBtn = new QPushButton();
      mResetBtn->setStyleSheet("QPushButton{border-image: url(://ResetBtn.png);}\nQPushButton:hover{border-image: url(://ResetBtnHv.png);}");

      mResetBtn->setAttribute(Qt::WA_TranslucentBackground);
      mResetBtn->setGeometry(this->width()/2 - 40, height() -  180, 80,80);

      // Adding all widgets to scene
      addWidget(mHeadingLv1Lb);
      addWidget(mHeadingLv2Lb);
      addWidget(mLv1Tb);
      addWidget(mLv2Tb);
      addWidget(mHeadingLb);
      addWidget(mBackBtn);
      addWidget(mResetLb);
      addWidget(mResetBtn);
}

void HighscoreScene::updateHighscore()
{
    QSettings settings;
    QString settingsAttribute;
    for(int j = 1; j < 6; j++){
        settings.setValue(QString("Lvl1_%1").arg(j), QVariant(QList<QString>({mScoreLv1[j-1].first, QString("%1").arg(mScoreLv1[j-1].second)})));
    }
    for(int j = 1; j < 6; j++){
        settings.setValue(QString("Lvl2_%1").arg(j), QVariant(QList<QString>({mScoreLv2[j-1].first, QString("%1").arg(mScoreLv2[j-1].second)})));
    }

    for(int i = 0; i < 5; i++ ){
          mLv2Tb->setItem(i,0, new QTableWidgetItem(settings.value(QString("Lvl2_%1").arg(i+1), "").toStringList()[0]));
          mLv2Tb->setItem(i,1, new QTableWidgetItem(settings.value(QString("Lvl2_%1").arg(i+1), "").toStringList()[1]));
          (mLv2Tb->item(i,0))->setFlags(Qt::ItemIsEnabled);
          (mLv2Tb->item(i,1))->setFlags(Qt::ItemIsEnabled);

          QPair<QString, int> onePair(settings.value(QString("Lvl1_%2").arg(i+1), "").toStringList()[0], settings.value(QString("Lvl2_%1").arg(i+1), "").toStringList()[1].toInt());
          mScoreLv2.append(onePair);
    }

    for(int i = 0; i < 5; i++ ){
          mLv1Tb->setItem(i,0, new QTableWidgetItem(settings.value(QString("Lvl1_%1").arg(i+1), "").toStringList()[0]));
          mLv1Tb->setItem(i,1, new QTableWidgetItem(settings.value(QString("Lvl1_%1").arg(i+1), "").toStringList()[1]));
          (mLv1Tb->item(i,0))->setFlags(Qt::ItemIsEnabled);
          (mLv1Tb->item(i,1))->setFlags(Qt::ItemIsEnabled);

          QPair<QString, int> onePair(settings.value(QString("Lvl1_%1").arg(i+1), "").toStringList()[0], settings.value(QString("Lvl1_%1").arg(i+1), "").toStringList()[1].toInt());
          mScoreLv1.append(onePair);
    }

    mLv1Tb->resizeColumnsToContents();
    mLv2Tb->resizeColumnsToContents();
}

void HighscoreScene::updateTables(int lvl, QString name, int score)
{
    QSettings settings;
    if(lvl == 1){
        for(int i = 0; i < 5; i++){
            if (score >= mScoreLv1[i].second){
                mScoreLv1.insert(i, QPair<QString,int>(name, score));
                mScoreLv1.pop_back();
                updateHighscore();
                break;
            }
        }
    }
    else if(lvl == 2){
        for(int i = 0; i < 5; i++){
            if (score >= mScoreLv2[i].second){
                mScoreLv2.insert(i, QPair<QString,int>(name, score));
                mScoreLv2.pop_back();
                updateHighscore();
                break;
            }
        }
    }
}

void HighscoreScene::resetTables()
{

    QMessageBox msg;
    msg.setIcon(QMessageBox::NoIcon);
    msg.setText("Are you sure you want \n to reset all scores?");
    msg.setStandardButtons(QMessageBox::Save | QMessageBox::No);
    msg.setDefaultButton(QMessageBox::Save);
    msg.button(QMessageBox::Save)->setObjectName("Yes");
    msg.button(QMessageBox::No)->setObjectName("No");
    int id = QFontDatabase::addApplicationFont("://NEWfONT2.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);
    QFont fontFont(family, 15);

    msg.setFont(fontFont);
    msg.button(QMessageBox::Save)->setIcon(QIcon());
    msg.button(QMessageBox::No)->setIcon(QIcon());
    msg.button(QMessageBox::Save)->setText("");
    msg.button(QMessageBox::No)->setText("");
    msg.setStyleSheet("QPushButton#Yes{width: 60; height: 60; border-image: url(://SubmitBtn.png); }"
                      "QPushButton:hover#Yes{width: 60; height: 60; border-image: url(://SubmitBtnHv.png);}"
                      "QPushButton#No{width: 60; height: 60; border-image: url(://NoBtn.png); }"
                      "QPushButton:hover#No{width: 60; height: 60; border-image: url(://NoBtnHv.png);}"
                      "QMessageBox{background-color: rgb(229,243,255);}" );
    switch (msg.exec()) {
    case QMessageBox::Save :{
        break;
        }
    case QMessageBox::No : {
        return;
        }
    }

    // reseting list and calling update highscore
    mScoreLv1.clear();
    mScoreLv2.clear();
    for (int i = 0; i < 5; i++ ){
        mScoreLv1.append(QPair<QString,int>("no name", 0));
        mScoreLv2.append(QPair<QString,int>("no name", 0));
    }
    updateHighscore();
}
