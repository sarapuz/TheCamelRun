#ifndef SETTINGSSCENE_H
#define SETTINGSSCENE_H

#include <QGraphicsScene>
#include <QLabel>
#include <QPushButton>

class BackgroundItem;

// TODO
class SettingsScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit SettingsScene(QObject *parent = 0);

    BackgroundItem *mBkg;
    QLabel *mSettingsLb;
    QPushButton *mBackBtn;

    QPushButton *mSoundBtn;
    QLabel *mSoundLb;

    QLabel *mKeysLb;
    QLabel *mLeftLb;
    QLabel *mRightLb;
    QLabel *mJumpLb;
    QPushButton *mLeftBtn;
    QPushButton *mRightBtn;
    QPushButton *mJumpBtn;


    void setSoundIcon();
    void resetButtons();

public slots:
    void changeLeftKey();
    void changeRightKey();
    void changeJumpKey();

signals:
    void leftKeyChanged(int);
    void rightKeyChanged(int);
    void jumpKeyChanged(int);

protected:
    void keyPressEvent(QKeyEvent *event);

};

#endif // SETTINGSSCENE_H
