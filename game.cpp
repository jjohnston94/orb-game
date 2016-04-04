#include "game.h"
#include <iostream>
using namespace std;

Game::Game()
{
    WINDOW_HEIGHT = 600;
    WINDOW_WIDTH = 800;

    scene = new QGraphicsScene(); // this holds graphicsitems and stuff
    scene->setSceneRect(0,0, WINDOW_WIDTH, WINDOW_HEIGHT);

    player = new PlayerOrb(); // the player
    for (int i = 0; i < 10; i++) // add ai orbs to scene
    {
        AIOrb *newOrb = new AIOrb (qrand() % 30);
        scene->addItem(newOrb);
        while (newOrb->collidingItems().size() > 0){        //Prevents orbs from being placed on each other
            newOrb->setPos(qrand() % 750, qrand() % 550);
        }
    }
    scene->addItem(player); // add player to scene

    player->setFlag(QGraphicsItem::ItemIsFocusable); // these allow the player to take in key presses
    player->setFocus(); // Note: only one thing can be focused on, so other objects cant take key presses

    view = new QGraphicsView(scene); // the view actually displays the scene
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // get rid of scroll bars
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedSize(WINDOW_WIDTH,WINDOW_HEIGHT);

}

void Game::show()
{
    view->show();

}

