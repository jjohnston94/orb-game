#ifndef GAME_H
#define GAME_H

#include <QGraphicsScene>
#include "playerorb.h"
#include "aiorb.h"
#include <QGraphicsView>
#include <QList>

class Game
{
public:
    Game();
    void show();
private:
    PlayerOrb * player;
    QGraphicsScene * scene;
    QGraphicsView * view;
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;
};

#endif // GAME_H
