#include "gamemanager.h"
#include "floor.h"
#include "cactus.h"

#include <QDebug>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameManager gm;
    return a.exec();
}
