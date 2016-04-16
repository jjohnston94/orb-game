
#include <QApplication>
#include "game.h"
#include "startscreen.h"
#include <iostream>\

Game * game;
startScreen * startscreen;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // necessary for any qt program

    startscreen = new startScreen();
    startscreen->show();

    game = new Game();
    game->show();

    return a.exec();
}
