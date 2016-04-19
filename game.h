#ifndef GAME_H
#define GAME_H

#include <QGraphicsScene>
#include "playerorb.h"
#include "aiorb.h"
#include "feederOrb.h"
#include <QGraphicsView>
#include <QList>
#include <QObject>

class Game : public QObject
{
    Q_OBJECT
public:
    Game();
    void show();
    int getScale();
    int WINDOW_WIDTH;
    int WINDOW_HEIGHT;
public slots:
    void gameLoop();
private:
    QTimer * timer;
    QList<AIOrb* > * aiList;
    QList<FeederOrb*> * feederList;
    QList<QGraphicsItem*> itemViewList;
    PlayerOrb * player;
    QGraphicsScene * scene;
    QGraphicsView * view;

    int SCENE_WIDTH;
    int SCENE_HEIGHT;
    int scale;
    int lastScale;

    void deleteAI(Orb * orb);
    void changeScale();
    void spawnAI();
    void moveCollideOrbs();
    void spawnFeeders();
};

#endif // GAME_H
