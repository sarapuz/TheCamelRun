#include "Header/losingscene.h"
#include "Header/backgrounditem.h"

#include <QKeyEvent>
#include <QFontDatabase>


LosingScene::LosingScene(QObject *parent) : QGraphicsScene(parent)
{
      setSceneRect(0, 0, 1248, 585);
      mBkg = new BackgroundItem(QPixmap("://menuBkg.png"));
      addItem(mBkg);

      int id = QFontDatabase::addApplicationFont("://NEWfONT2.ttf");
      QString family = QFontDatabase::applicationFontFamilies(id).at(0);
      QFont fontFont(family, 25);

      mLosingLb = new QLabel();
      mLosingLb->setText("You have shorten the world for another camel! \n Press any key to continue.");
      mLosingLb->setFont(fontFont);
      mLosingLb->setAttribute(Qt::WA_TranslucentBackground);
      mLosingLb->setAlignment(Qt::AlignTop | Qt::AlignCenter );
      mLosingLb->setGeometry(0,height()/2 - 50, width(),80);
      addWidget(mLosingLb);
}

void LosingScene::keyPressEvent(QKeyEvent *event)
{
    emit backToMenu();
}
