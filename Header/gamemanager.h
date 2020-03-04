#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <QMainWindow>
#include <QVector>
#include <QMap>
#include <QGraphicsView>
#include <QSettings>


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

public:
    explicit GameManager(QWidget *parent = nullptr);

private:
    QGraphicsView mView;
    MenuScene *mMenuScene;
    GameScene *mGameScene;
    SettingsScene *mSettingsScene;
    HighscoreScene *mHighscoreScene;
    LosingScene *mLosingScene;
    WinningScene *mWinningScene;
    QSound *mSound;

    void renderScene(QGraphicsScene*);

signals:
   void currentStateChanged();
   void soundChanged();

private slots:
   void nextLevel(int);

public slots:
    void startGame();
    void startSettings();
    void startMenu();
    void startHighscore();
    void startYouLost();
    void startYouWon(int, int);
    void changeSoundMode();
    void playSound();
};

#endif // GAMEMANAGER_H
