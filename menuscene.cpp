#include "menuscene.h"
#include "backgrounditem.h"

#include <QFontDatabase>
#include <QFont>


MenuScene::MenuScene(QObject *parent) :
    QGraphicsScene(parent)
{
      // <Set background
      setSceneRect(0, 0, 1248, 585);
      mBkg = new BackgroundItem(QPixmap("://menuBkg.png"));
      addItem(mBkg);
      // >

      int id = QFontDatabase::addApplicationFont("://NEWfONT2.ttf");
      QString family = QFontDatabase::applicationFontFamilies(id).at(0);
      QFont fontFont(family, 40);

      mLogo = new BackgroundItem(QPixmap("://CamelRun.png"));
      mLogo->setPos(this->width()/2 - 100, 20);
      addItem(mLogo);

      mTitle = new QLabel();
      mTitle->setText("The Camel Run");
      mTitle->setAttribute(Qt::WA_TranslucentBackground);
      mTitle->setGeometry(QRect(this->width()/2 - 225, 190, 450, 55));
      mTitle->setAlignment(Qt::AlignCenter);
      mTitle->setFont(fontFont);
      addWidget(mTitle);

      //fontFont.setPointSize(40);

      fontFont.setPointSize(25);
      // <Add buttons and label
      mStartBtn = new QPushButton();
      mStartBtn->setText("Start");
      mStartBtn->setFont(fontFont);
      mStartBtn->setGeometry(this->width()/2 - 100, this->height()/2 , 200, 50);
/*
      mStartBtn->setStyleSheet(
                  "background: transparent;"
                  "color: #000000;"
                  "border: 8px solid #000000;"
                  "border-radius: 5px;"
                  );
*/
      // Focus - not Working
      /*
       * mStartBtn->setFocus();
       * mStartBtn->setDefault(true);
       */
      mSettingsBtn = new QPushButton();
      mSettingsBtn->setText("Settings");
      mSettingsBtn->setFont(fontFont);
      mSettingsBtn->setGeometry(this->width()/2 - 100, this->height()/2 + 100, 200, 50);

      mHighBtn = new QPushButton();
      mHighBtn->setText("Highscore");
      mHighBtn->setFont(fontFont);
      mHighBtn->setGeometry(this->width()/2 - 100, this->height()/2 + 200, 200, 50);

      this->addWidget(mStartBtn);
      this->addWidget(mSettingsBtn);
      this->addWidget(mHighBtn);
      //>
}


