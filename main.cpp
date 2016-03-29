
#include <QApplication>
#include <QGraphicsScene>
#include "playerorb.h"
#include "aiorb.h"
#include <QGraphicsView>
#include <QList>
#include <iostream>
#include <QTimer>
#include "game.h"

Game * game;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // necessary for any qt program

    game = new Game();
    game->show();

    return a.exec();
}
