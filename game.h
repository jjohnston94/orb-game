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
private:
    QTimer * timer;
    QList<AIOrb* > * aiList;
    QList<QGraphicsItem*> itemViewList;
    PlayerOrb * player;
    QGraphicsScene * scene;
    QGraphicsView * view;

    int SCENE_WIDTH;
    int SCENE_HEIGHT;
    int scale;
    int lastScale;

    void deleteAI(AIOrb * ai);
    void changeScale();
    void spawnAI();
    void moveCollideAI();
};

#endif // GAME_H
