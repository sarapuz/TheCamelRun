#include "settingsscene.h"
#include "backgrounditem.h"

#include <QFontDatabase>
#include <QIcon>
#include <QSettings>
#include <QDebug>
#include <QKeyEvent>


void SettingsScene::setSoundIcon(){
    QSettings settings;
    if (settings.value("Sound", "").toString() == "On")
        mSoundBtn->setIcon(QIcon(":/soundON.ico"));
    else
        mSoundBtn->setIcon(QIcon(":/soundOFF.ico"));
}

void SettingsScene::resetButtons()
{
    mLeftBtn->setEnabled(true);
    mRightBtn->setEnabled(true);
    mJumpBtn->setEnabled(true);

    mLeftBtn->setStyleSheet("");
    mRightBtn->setStyleSheet("");
    mJumpBtn->setStyleSheet("");
}

SettingsScene::SettingsScene(QObject *parent) : QGraphicsScene(parent)
{
      setSceneRect(0, 0, 1248, 585);
      mBkg = new BackgroundItem(QPixmap("://menuBkg.png"));
      addItem(mBkg);

      int id = QFontDatabase::addApplicationFont("://NEWfONT2.ttf");
      QString family = QFontDatabase::applicationFontFamilies(id).at(0);
      QFont fontFont(family, 25);

      mSettingsLb = new QLabel();
      mSettingsLb->setText("Set your camel adventure!");
      mSettingsLb->setFont(fontFont);
      mSettingsLb->setAttribute(Qt::WA_TranslucentBackground);
      mSettingsLb->setGeometry(QRect(this->width()/2 - mSettingsLb->width()/2, this->height()/6, mSettingsLb->width(), 40 ));

      mSettingsLb->setAlignment(Qt::AlignTop | Qt::AlignCenter );

      mBackBtn = new QPushButton();
      mBackBtn->setStyleSheet("QPushButton{border-image: url(://BackBtn.png);}\nQPushButton:hover{border-image: url(://BackBtnHv.png);}");
      mBackBtn->setAttribute(Qt::WA_TranslucentBackground);
      mBackBtn->setGeometry(this->width() - 100, 15, 80,80);

      mSoundLb = new QLabel();
      mSoundLb->setText("Sound: ");
      fontFont.setPointSize(18);
      mSoundLb->setFont(fontFont);
      mSoundLb->setAttribute(Qt::WA_TranslucentBackground);
      mSoundLb->setGeometry(QRect(this->width()/2 - 100, this->height()/3, mSettingsLb->width(), 40 ));

      mSoundBtn = new QPushButton();
      setSoundIcon();
      mSoundBtn->setAttribute(Qt::WA_TranslucentBackground);
      mSoundBtn->setStyleSheet("QPushButton:hover{background-color: rgb(255,211,155);}");
      mSoundBtn->setGeometry(this->width()/2, this->height()/3+4,  30,30);

      mKeysLb = new QLabel();
      mKeysLb->setText("Update your keys:");
      mKeysLb->setFont(fontFont);
      mKeysLb->setAttribute(Qt::WA_TranslucentBackground);
      mKeysLb->setGeometry(QRect(this->width()/2 - 120, (this->height()/3) + 60, 240, 40 ));

      fontFont.setPointSize(16);
      mLeftLb = new QLabel();
      mLeftLb->setText("Left: ");
      mLeftLb->setFont(fontFont);
      mLeftLb->setAttribute(Qt::WA_TranslucentBackground);
      mLeftLb->setGeometry(QRect(this->width()/2 - 100, (this->height()/3) + 110, 70, 40 ));

      mRightLb = new QLabel();
      mRightLb->setText("Right: ");
      mRightLb->setFont(fontFont);
      mRightLb->setAttribute(Qt::WA_TranslucentBackground);
      mRightLb->setGeometry(QRect(this->width()/2 - 100, (this->height()/3) + 150, 70, 40 ));

      mJumpLb = new QLabel();
      mJumpLb->setText("Jump: ");
      mJumpLb->setFont(fontFont);
      mJumpLb->setAttribute(Qt::WA_TranslucentBackground);
      mJumpLb->setGeometry(QRect(this->width()/2 - 100, (this->height()/3) + 190, 70, 40 ));


      QSettings settings;
      mLeftBtn = new QPushButton();
      mLeftBtn->setText(settings.value("LeftKey", "").toString());
      mLeftBtn->setStyleSheet("QPushButton:hover{background-color: rgb(255,211,155);}");
      mLeftBtn->setGeometry(QRect(this->width()/2 , (this->height()/3) + 110, 100, 36));

      mRightBtn = new QPushButton();
      mRightBtn->setText(settings.value("RightKey", "").toString());
      mRightBtn->setStyleSheet("QPushButton:hover{background-color: rgb(255,211,155);}");
      mRightBtn->setGeometry(QRect(this->width()/2 , (this->height()/3) + 150, 100, 36));

      mJumpBtn = new QPushButton();
      mJumpBtn->setText(settings.value("JumpKey", "").toString());
      mJumpBtn->setStyleSheet("QPushButton:hover{background-color: rgb(255,211,155);}");
      mJumpBtn->setGeometry(QRect(this->width()/2 , (this->height()/3) + 190, 100, 36));



      this->addWidget(mJumpBtn);
      this->addWidget(mRightBtn);
      this->addWidget(mLeftBtn);
      this->addWidget(mLeftLb);
      this->addWidget(mRightLb);
      this->addWidget(mJumpLb);
      this->addWidget(mKeysLb);
      this->addWidget(mSoundLb);
      this->addWidget(mSoundBtn);
      this->addWidget(mSettingsLb);
      this->addWidget(mBackBtn);

      connect(mLeftBtn, SIGNAL(clicked()), this, SLOT(changeLeftKey()));
      connect(mRightBtn, SIGNAL(clicked()), this, SLOT(changeRightKey()));
      connect(mJumpBtn, SIGNAL(clicked()), this, SLOT(changeJumpKey()));
}

void SettingsScene::changeLeftKey()
{
    mLeftBtn->setStyleSheet("background-color: #C0C0C0");
    mRightBtn->setEnabled(false);
    mJumpBtn->setEnabled(false);
}

void SettingsScene::changeRightKey()
{
    mLeftBtn->setEnabled(false);
    mRightBtn->setStyleSheet("background-color: #C0C0C0");
    mJumpBtn->setEnabled(false);
}

void SettingsScene::changeJumpKey()
{
    mLeftBtn->setEnabled(false);
    mRightBtn->setEnabled(false);
    mJumpBtn->setStyleSheet("background-color: #C0C0C0");
}


void SettingsScene::keyPressEvent(QKeyEvent *event){

    QSettings settings;
    if (!mRightBtn->isEnabled() && !mJumpBtn->isEnabled()){
        QString newKey = QVariant::fromValue(Qt::Key(event->key())).toString();
        emit leftKeyChanged(event->key());

        mRightBtn->setEnabled(true);
        mJumpBtn->setEnabled(true);
        mLeftBtn->setStyleSheet("");
        mLeftBtn->setText(settings.value("LeftKey", "").toString());
    }
    else if(!mLeftBtn->isEnabled() && !mJumpBtn->isEnabled()){
        QString newKey = QVariant::fromValue(Qt::Key(event->key())).toString();
        emit rightKeyChanged(event->key());

        mLeftBtn->setEnabled(true);
        mJumpBtn->setEnabled(true);
        mRightBtn->setStyleSheet("");
        mRightBtn->setText(settings.value("RightKey", "").toString());
    }
    else if(!mLeftBtn->isEnabled() && !mRightBtn->isEnabled()){
        QString newKey = QVariant::fromValue(Qt::Key(event->key())).toString();
        emit jumpKeyChanged(event->key());

        mRightBtn->setEnabled(true);
        mLeftBtn->setEnabled(true);
        mJumpBtn->setStyleSheet("");
        mJumpBtn->setText(settings.value("JumpKey", "").toString());
    }


}
