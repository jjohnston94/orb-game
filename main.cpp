
#include <QApplication>
#include <QGraphicsScene>
#include "playerorb.h"
#include "aiorb.h"
#include <QGraphicsView>
#include <QList>
#include <iostream>
#include <QTimer>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv); // necessary for any qt program

    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;

    QGraphicsScene *scene = new QGraphicsScene(); // this holds graphicsitems and stuff
    scene->setSceneRect(0,0, WINDOW_WIDTH, WINDOW_HEIGHT);

    PlayerOrb *player = new PlayerOrb(); // the player

    for (int i = 0; i < 5; i++) // add ai orbs to scene
        scene->addItem(new AIOrb());
    scene->addItem(player); // add player to scene

    player->setFlag(QGraphicsItem::ItemIsFocusable); // these allow the player to take in key presses
    player->setFocus(); // Note: only one thing can be focused on, so other objects cant take key presses afaik

    QGraphicsView *view = new QGraphicsView(scene); // the view actually displays the scene
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // get rid of scroll bars
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    view->show();
    view->setFixedSize(WINDOW_WIDTH,WINDOW_HEIGHT); // this prevents the view from growing if things leave the screen

    return a.exec();
}
