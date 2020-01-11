#include "losingscene.h"
#include "backgrounditem.h"

#include <QKeyEvent>

LosingScene::LosingScene(QObject *parent) : QGraphicsScene(parent)
{
      setSceneRect(0, 0, 1248, 585);
      mBkg = new BackgroundItem(QPixmap("://menuBkg.png"));
      addItem(mBkg);

      mLosingLb = new QLabel();
      mLosingLb->setText("You shorten the world for another camel! \n Press Q to continue.");
      mLosingLb->setAlignment(Qt::AlignTop | Qt::AlignCenter );
      addWidget(mLosingLb);
}

void LosingScene::keyPressEvent(QKeyEvent *event)
{

    if (event->isAutoRepeat()) {
        return;
    }
    if (event->key() == Qt::Key_Q) {
        emit backToMenu();
    }

//    emit backToMenu();
}
