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

    QPushButton *mBackBtn;
    QPushButton *mSoundBtn;
    QPushButton *mLeftBtn;
    QPushButton *mRightBtn;
    QPushButton *mJumpBtn;

    void setSoundIcon();
    void resetButtons();

private:
    BackgroundItem *mBkg;
    QLabel *mSettingsLb;
    QLabel *mSoundLb;
    QLabel *mKeysLb;
    QLabel *mLeftLb;
    QLabel *mRightLb;
    QLabel *mJumpLb;

protected:
    void keyPressEvent(QKeyEvent *event);

public slots:
    void changeLeftKey();
    void changeRightKey();
    void changeJumpKey();

signals:
    void leftKeyChanged(int);
    void rightKeyChanged(int);
    void jumpKeyChanged(int);
};

#endif // SETTINGSSCENE_H
