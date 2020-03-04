#include "Header/gamemanager.h"
#include "Header/floor.h"
#include "Header/cactus.h"

#include <QDebug>
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GameManager gm;
    return a.exec();
}
