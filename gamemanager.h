#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QMainWindow>
#include <QVector>
#include <QMap>
#include <QGraphicsView>
#include <QSettings>

#include "SceneStates.h"

class MenuScene;
class GameScene;
class SettingsScene;
class HighscoreScene;
class LosingScene;
class WinningScene;
class QSound;

/*!
  \class GameManager
  \brief The GameManager class manages rendering and changing of scenes/screens.

  GameManager class controlls changes between playing, settings and other screens. It plays the role of event controller.
*/
class GameManager : public QMainWindow
{
    Q_OBJECT
    //Q_PROPERTY(SceneStates::State mCurrentState READ currentState  WRITE setCurrentState NOTIFY currentStateChanged)

public:
    explicit GameManager(QWidget *parent = nullptr);

    // proba*

    QGraphicsView mView;
    SceneStates::State mCurrentState;
    MenuScene *mMenuScene;
    GameScene *mGameScene;
    SettingsScene *mSettingsScene;
    HighscoreScene *mHighscoreScene;
    LosingScene *mLosingScene;
    WinningScene *mWinningScene;

    QSound *mSound;
    //QMap<const SceneStates::State, QGraphicsScene> mSceneStates;

    void renderScene(QGraphicsScene*);

    SceneStates::State currentState(){return mCurrentState;}
    void setCurrentState(SceneStates::State newState){if (newState == mCurrentState) return; mCurrentState = newState;}

signals:
   void currentStateChanged();
   void soundChanged();

public slots:
    void startGame();
    void startSettings();
    void startMenu();
    void startHighscore();
    void startYouLost();
    void startYouWon(int, int);
    void changeSoundMode();
    void playSound();

private:
    void loadSettigs();
    void saveSettings();

};

#endif // GAMEMANAGER_H
