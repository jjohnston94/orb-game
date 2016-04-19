
#include <QApplication>
#include "game.h"
#include "startscreen.h"
#include <iostream>\

Game * game;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // necessary for any qt program

    StartScreen start;
    start.show();

    return a.exec();
}
