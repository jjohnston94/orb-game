#ifndef GAME_H
#define GAME_H

#include <QGraphicsScene>
#include "playerorb.h"
#include "aiorb.h"
#include <QGraphicsView>
#include <QList>
#include <QObject>

class Game : public QObject
{
    Q_OBJECT
public:
    Game();
    void show();
public slots:
    void gameLoop();
private:
    PlayerOrb * player;
    QGraphicsScene * scene;
    QGraphicsView * view;
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;
};

#endif // GAME_H
