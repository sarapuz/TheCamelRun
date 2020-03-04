#include "Header/gamemanager.h"
#include "Header/menuscene.h"
#include "Header/gamescene.h"
#include "Header/settingsscene.h"
#include "Header/highscorescene.h"
#include "Header/losingscene.h"
#include "Header/winningscene.h"

#include <QWidget>
#include <QDebug>
#include <QApplication>
#include <QSound>
#include <QVector>


GameManager::GameManager(QWidget *parent) : QMainWindow(parent)
{
    QCoreApplication::setOrganizationName("PMF");
    QCoreApplication::setApplicationName("The Camel Run");
    QSettings settings;
    // <If settings are not yet set, initate values
    if(settings.allKeys().length() == 0){
        settings.setValue("Sound", "On");
        settings.setValue("LeftKey", "Key_Left");
        settings.setValue("RightKey", "Key_Right");
        settings.setValue("JumpKey", "Key_Space");

        QList<QString> score({"No name", "0"});
        QString settingsAttribute;
        for(int i = 1; i < 3; i++){
            for(int j = 1; j < 6; j++){
                settingsAttribute = QString("Lvl%1_%2").arg(i).arg(j);
                settings.setValue(settingsAttribute, QVariant(score));
            }
        }
    }
    // >

    mMenuScene = new MenuScene();
    mSettingsScene = new SettingsScene();
    mGameScene = new GameScene();
    mHighscoreScene = new HighscoreScene();
    mLosingScene = new LosingScene();
    mWinningScene = new WinningScene();

    // <MAIN MENU: Starting scenes such as Settings, Highscore and Game from the Menu scene
    connect(mMenuScene->mStartBtn, &QPushButton::clicked, this, &GameManager::startGame);
    connect(mMenuScene->mSettingsBtn, &QPushButton::clicked, this, &GameManager::startSettings);
    connect(mMenuScene->mHighBtn, &QPushButton::clicked, this, &GameManager::startHighscore);
    //>

    // <BACK: Returning back to main menu from settings, highscore and game scene.
    // Game scene also needs to reset the scene and player.
    connect(mSettingsScene->mBackBtn, &QPushButton::clicked, [this]() {mSettingsScene->resetButtons(); startMenu();});
    connect(mHighscoreScene->mBackBtn, &QPushButton::clicked, this, &GameManager::startMenu);
    connect(mGameScene->mBackBtn, &QPushButton::clicked, [this](){mGameScene->killPlayer(); mGameScene->setLevel(0); nextLevel(mGameScene->level()); startMenu();});
    //>

    // <SETTINGS: Changing  keyboard controls
    connect(mSettingsScene, &SettingsScene::leftKeyChanged, mGameScene, &GameScene::changeLeftKey);
    connect(mSettingsScene, &SettingsScene::rightKeyChanged, mGameScene, &GameScene::changeRightKey);
    connect(mSettingsScene, &SettingsScene::jumpKeyChanged, mGameScene, &GameScene::changeJumpKey);
    // >

    // <SETTINGS: Turning on and off sound by clicking button in Settings scene
    connect(mSettingsScene->mSoundBtn, &QPushButton::clicked, this, &GameManager::changeSoundMode);
    // >

    // HIGHSCORE: Reseting hgihscores on press of a button
    connect(mHighscoreScene->mResetBtn, &QPushButton::clicked, mHighscoreScene, &HighscoreScene::resetTables);
    //>

    // <PLAYER: Updating player attributes such as coins and health
    connect(mGameScene, &GameScene::healthBarChanged, mGameScene, &GameScene::updateHealthBar);
    connect(mGameScene, &GameScene::coinGathered, mGameScene, &GameScene::updateCoinCounter);
    // >

    // <LOSING: When player loses game (helathbar drops to zero) youLost() signal is emitted
    // startYouLost is triggered - which then resets both Scene and Player and renders LosingScene
    // from Losing Scene signal backToMenu() triggers rendering of Main Menu
    connect(mGameScene, &GameScene::youLost, this, &GameManager::startYouLost);
    connect(mLosingScene, &LosingScene::backToMenu, this, &GameManager::startMenu);
    //>

    // <WINNING: When player  passes a level, a winning screen renders (signal youWon triggers slot startYouWon)
    // Player then writes his name and confirms with button click - signal! this signal is processed with slot updateTables
    // And next level is set (level 1 or 2, depending on current level).
    connect(mGameScene, &GameScene::youWon, this, &GameManager::startYouWon);
    connect(mWinningScene, &WinningScene::candidateForHighscore, mHighscoreScene, &HighscoreScene::updateTables);
    connect(mWinningScene, &WinningScene::levelWon, this, &GameManager::nextLevel);
    //>

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

void GameManager::nextLevel(int level)
{
    mGameScene->resetScene();
    switch(level){
        case 0:{
            mGameScene->resetPlayer();
            mGameScene->setLevel(1);
            mGameScene->startLevel(1);
            break;
        }
        case 1:{
            mGameScene->setLevel(2);
            mGameScene->startLevel(2);
            mGameScene->revivePlayer();
            renderScene(mGameScene);
            mGameScene->mTimerEn.start();
            break;
        }
        case 2:{
            mGameScene->resetPlayer();
            mGameScene->setLevel(1);
            mGameScene->startLevel(1);
            startMenu();
            break;
        }
    }
}

void GameManager::startGame()
{
    mGameScene->revivePlayer();
    renderScene(mGameScene);
    mGameScene->mTimerEn.start();
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
    mGameScene->resetScene();
    mGameScene->resetPlayer();
    mGameScene->startLevel(1);
    renderScene(mLosingScene);
}

void GameManager::startYouWon(int lvl, int coins)
{
    mWinningScene->mTempLvl = lvl;
    mWinningScene->mTempCoins = coins;
    renderScene(mWinningScene);
}

void GameManager::changeSoundMode()
{
    QSettings settings;
    QString soundOffOn = settings.value("Sound", "").toString();

    if (soundOffOn == "On"){
        settings.setValue("Sound", "Off");
    }
    else{
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
