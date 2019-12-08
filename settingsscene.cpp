#include "settingsscene.h"
#include "backgrounditem.h"

SettingsScene::SettingsScene(QObject *parent) : QGraphicsScene(parent)
{
      setSceneRect(0, 0, 1248, 585);
      mBkg = new BackgroundItem(QPixmap("://bkg.png"));
      addItem(mBkg);

      mSettingsLb = new QLabel();
      mSettingsLb->setText("Set your camel adventure!");
      mSettingsLb->setAlignment(Qt::AlignTop | Qt::AlignCenter );

      mBackBtn = new QPushButton();
      mBackBtn->setText("Back");
      mBackBtn->setGeometry(this->width() - 100, 5, 90,30);

      this->addWidget(mSettingsLb);
      this->addWidget(mBackBtn);
}
