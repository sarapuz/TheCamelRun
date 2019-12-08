
#include <QWidget>
#include <QDebug>

#include "gamemanager.h"
#include "menuscene.h"
#include "gamescene.h"
#include "settingsscene.h"
#include "highscorescene.h"
#include "losingscene.h"

GameManager::GameManager(QWidget *parent) : QMainWindow(parent)
{
    mCurrentState = SceneStates::MenuScene;
    mMenuScene = new MenuScene();
    mGameScene = new GameScene();
    mSettingsScene = new SettingsScene();
    mHighscoreScene = new HighscoreScene();
    mLosingScene = new LosingScene();


    // Starting scenes from menu
    connect(mMenuScene->mStartBtn, SIGNAL(clicked()), this, SLOT(startGame()));
    connect(mMenuScene->mSettingsBtn, SIGNAL(clicked()), this, SLOT(startSettings()));
    connect(mMenuScene->mHighBtn, SIGNAL(clicked()), this, SLOT(startHighscore()));


    // Back to menu
    connect(mGameScene->mBackBtn, SIGNAL(clicked()), this, SLOT(startMenu()));
    connect(mGameScene->mBackBtn, SIGNAL(clicked()), mGameScene, SLOT(resetScene()));
    connect(mSettingsScene->mBackBtn, SIGNAL(clicked()), this, SLOT(startMenu()));
    connect(mHighscoreScene->mBackBtn, SIGNAL(clicked()), this, SLOT(startMenu()));

    // Other xD
    connect(mGameScene, SIGNAL(healthBarChanged(int)), mGameScene, SLOT(updateHealthBar(int)));
    connect(mGameScene, SIGNAL(youLost()), mGameScene, SLOT(resetScene()));
    connect(mGameScene, SIGNAL(youLost()), this, SLOT(startYouLost()));
    connect(mLosingScene, SIGNAL(backToMenu()), this, SLOT(startMenu()));


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

void GameManager::startMenu(){

    renderScene(mMenuScene);
}

void GameManager::startHighscore(){
    renderScene(mHighscoreScene);
}

void GameManager::startYouLost()
{
    renderScene(mLosingScene);
}
