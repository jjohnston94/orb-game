#ifndef GAME_H
#define GAME_H

#include <QGraphicsScene>
#include "playerorb.h"
#include "aiorb.h"
#include "feederOrb.h"
#include <QGraphicsView>
#include <QList>
#include <QObject>
#include <QMediaPlayer>
#include <QMediaPlaylist>


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
    bool won;

    int orbsEaten;
    int deaths;

    void deleteAI(Orb * orb);
    void changeScale();
    void spawnAI();
    void moveCollideOrbs();
    void collidePlayer();
    void spawnFeeders();
    void cullBadOrbs();
    void win();
    QMediaPlaylist * playlist;
    QMediaPlayer * music;
};

#endif // GAME_H
