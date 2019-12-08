#include "menuscene.h"
#include "backgrounditem.h"

MenuScene::MenuScene(QObject *parent) :
    QGraphicsScene(parent)
{
      // <Set background
      setSceneRect(0, 0, 1248, 585);
      mBkg = new BackgroundItem(QPixmap("://bkg.png"));
      addItem(mBkg);
      // >

      // <Add buttons and label
      mStartBtn = new QPushButton();
      mStartBtn->setText("Start");
      mStartBtn->setGeometry(this->width()/2 - 50, this->height()/2, 100, 30);
      // Focus - not Working
      /*
       * mStartBtn->setFocus();
       * mStartBtn->setDefault(true);
       */
      mSettingsBtn = new QPushButton();
      mSettingsBtn->setText("Settings");
      mSettingsBtn->setGeometry(this->width()/2 - 50, this->height()/2 + 60, 100, 30);

      mHighBtn = new QPushButton();
      mHighBtn->setText("Highscore");
      mHighBtn->setGeometry(this->width()/2 - 50, this->height()/2 + 120, 100, 30);

      this->addWidget(mStartBtn);
      this->addWidget(mSettingsBtn);
      this->addWidget(mHighBtn);
      //>
}


