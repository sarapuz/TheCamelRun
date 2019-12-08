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

};

#endif // SETTINGSSCENE_H
