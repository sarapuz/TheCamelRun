#include "losingscene.h"
#include "backgrounditem.h"

LosingScene::LosingScene(QObject *parent) : QGraphicsScene(parent)
{
      setSceneRect(0, 0, 1248, 585);
      mBkg = new BackgroundItem(QPixmap("://bkg.png"));
      addItem(mBkg);

      mLosingLb = new QLabel();
      mLosingLb->setText("You shorten the world for another camel! \n PRess any key to continue.");
      mLosingLb->setAlignment(Qt::AlignTop | Qt::AlignCenter );
      addWidget(mLosingLb);
}

void LosingScene::keyPressEvent(QKeyEvent *event)
{
    emit backToMenu();
}
