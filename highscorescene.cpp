#include "highscorescene.h"
#include "backgrounditem.h"

HighscoreScene::HighscoreScene(QObject *parent) : QGraphicsScene(parent)
{
      setSceneRect(0, 0, 1248, 585);
      mBkg = new BackgroundItem(QPixmap("://menuBkg.png"));
      addItem(mBkg);

      mBackBtn = new QPushButton();
      mBackBtn->setText("Back");
      mBackBtn->setGeometry(this->width() - 100, 5, 90,30);

      this->addWidget(mBackBtn);
}
