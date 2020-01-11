
#include <QWidget>
#include <QDebug>
#include <QApplication>
#include <QSound>
#include <QVector>

#include "gamemanager.h"
#include "menuscene.h"
#include "gamescene.h"
#include "settingsscene.h"
#include "highscorescene.h"
#include "losingscene.h"
#include "winningscene.h"

GameManager::GameManager(QWidget *parent) : QMainWindow(parent)
{
    QCoreApplication::setOrganizationName("PMF");
    QCoreApplication::setApplicationName("Cameeel");
    QSettings settings;
    settings.setValue("Sound", "On");
    settings.setValue("LeftKey", "Key_Left");
    settings.setValue("RightKey", "Key_Right");
    settings.setValue("JumpKey", "Key_Space");
    QList<QList<QString> > score_list;
    score_list.append(QList<QString>({"1", "Sara", "34"}));
    //settings.setValue("ScoreList", QVariant(score_list));


    mCurrentState = SceneStates::MenuScene;
    mMenuScene = new MenuScene();
    mSettingsScene = new SettingsScene();
    mGameScene = new GameScene();
    mHighscoreScene = new HighscoreScene();
    mLosingScene = new LosingScene();
    mWinningScene = new WinningScene();

    // Starting scenes from menu
    connect(mMenuScene->mStartBtn, &QPushButton::clicked, [this](){startGame();});
    //    connect(mMenuScene->mStartBtn, &QPushButton::clicked, this, &GameManager::startGame);
//    connect(mMenuScene->mStartBtn, SIGNAL(clicked()), this, SLOT(startGame()));
    connect(mMenuScene->mSettingsBtn, SIGNAL(clicked()), this, SLOT(startSettings()));
    connect(mMenuScene->mHighBtn, SIGNAL(clicked()), this, SLOT(startHighscore()));


    // Back to menu
    connect(mGameScene->mBackBtn, SIGNAL(clicked()), this, SLOT(startMenu()));
    connect(mGameScene->mBackBtn, SIGNAL(clicked()), mGameScene, SLOT(resetScene()));
    connect(mGameScene->mBackBtn, &QPushButton::clicked,  [this](){qDebug()<< "klik!";});

    connect(mSettingsScene->mBackBtn, &QPushButton::clicked, [this]() {mSettingsScene->resetButtons(); startMenu();});

    connect(mHighscoreScene->mBackBtn, SIGNAL(clicked()), this, SLOT(startMenu()));

    // Other xD
    connect(mGameScene, SIGNAL(healthBarChanged(int)), mGameScene, SLOT(updateHealthBar(int)));
    connect(mGameScene, SIGNAL(coinGathered()), mGameScene, SLOT(updateCoinCounter()));
    connect(mGameScene, SIGNAL(youLost()), mGameScene, SLOT(resetScene()));
    connect(mGameScene, SIGNAL(youLost()), this, SLOT(startYouLost()));
    connect(mLosingScene, SIGNAL(backToMenu()), this, SLOT(startMenu()));
    connect(mGameScene, SIGNAL(youWon()), mGameScene, SLOT(resetScene()));
    connect(mGameScene, SIGNAL(youWon()), this, SLOT(startYouWon()));

    connect(mSettingsScene->mSoundBtn, SIGNAL(clicked()), this, SLOT(changeSoundMode()));
    connect(mWinningScene, SIGNAL(backToMenu()), this, SLOT(startMenu()));

    connect(mSettingsScene, SIGNAL(leftKeyChanged(int)), mGameScene, SLOT(changeLeftKey(int)));
    connect(mSettingsScene, SIGNAL(rightKeyChanged(int)), mGameScene, SLOT(changeRightKey(int)));
    connect(mSettingsScene, SIGNAL(jumpKeyChanged(int)), mGameScene, SLOT(changeJumpKey(int)));


    mSound = new QSound("://DesertSound.wav");
    if (settings.value("Sound","").toString() == "On"){
        mSound->setLoops(QSound::Infinite);
        mSound->play();
    }

    renderScene(mMenuScene);

}


void GameManager::renderScene(QGraphicsScene *currentScene)
{
    mView.setRenderHint(QPainter::Antialiasing);
    mView.setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mView.setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    mView.setScene(currentScene);
    mView.setFixedSize(currentScene->sceneRect().size().toSize());
    mView.show();
}

void GameManager::startGame()
{
    renderScene(mGameScene);
}

void GameManager::startSettings()
{
    renderScene(mSettingsScene);
}

void GameManager::startMenu()
{

    renderScene(mMenuScene);
}

void GameManager::startHighscore()
{
    renderScene(mHighscoreScene);
}

void GameManager::startYouLost()
{
    renderScene(mLosingScene);
}

void GameManager::startYouWon()
{
    renderScene(mWinningScene);
}

void GameManager::changeSoundMode()
{
    QSettings settings;
    QString soundOffOn = settings.value("Sound", "").toString();

    if (soundOffOn == "On"){
        qDebug() << "Changing sound to OFF!";
        settings.setValue("Sound", "Off");
    }
    else{
        qDebug() << "Changing sound to ON!";
        settings.setValue("Sound", "On");

    }
    mSettingsScene->setSoundIcon();
    playSound();
}

void GameManager::playSound()
{
    QSettings settings;
    QString soundOffOn = settings.value("Sound", "").toString();

    if (soundOffOn == "On"){
        mSound->setLoops(QSound::Infinite);
        mSound->play();
    }
    else{
        mSound->stop();
    }
}
