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
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;
public slots:
    void gameLoop();
    void spawnAI();
private:
    QTimer * timer;
    QList<AIOrb* > * aiList;
    PlayerOrb * player;
    QGraphicsScene * scene;
    QGraphicsView * view;

    int VIEW_WIDTH;
    int VIEW_HEIGHT;
};

#endif // GAME_H
