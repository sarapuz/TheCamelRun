#ifndef LOSINGSCENE_H
#define LOSINGSCENE_H

#include <QGraphicsScene>
#include <QLabel>
#include <QPushButton>

class BackgroundItem;


/*!
 * \brief The LosingScene class scene which is displayed upon losing.
 *
 * still TODO
 */
class LosingScene : public QGraphicsScene
{
    Q_OBJECT
public:
    LosingScene(QObject *parent = 0);

    BackgroundItem *mBkg;
    QLabel *mLosingLb;

protected:
    void keyPressEvent(QKeyEvent *event);

signals:
    void backToMenu();

};

#endif // LOSINGSCENE_H
