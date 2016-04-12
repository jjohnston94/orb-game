#include "game.h"
#include <iostream>
#include <QTimer>
#include <QList>
#include <QRect>
#include <QApplication>
#include <QDesktopWidget>
#include <typeinfo>

Game::Game()
{

    QRect rec = QApplication::desktop()->screenGeometry(); // get screen geometry for width and height
    WINDOW_HEIGHT = rec.height();
    WINDOW_WIDTH = rec.width();

    VIEW_WIDTH = 3000;
    VIEW_HEIGHT = 5000;

    scene = new QGraphicsScene(); // this holds graphicsitems and stuff
    scene->setSceneRect(0,0, VIEW_WIDTH, VIEW_HEIGHT);


    aiList = new QList<AIOrb *>(); // List of ai orbs so we can manipulate them later
    /*for (int i = 0; i < 25; i++) // add ai orbs to scene
    {
        aiList->append(new AIOrb(qrand() % 50, qrand() % VIEW_WIDTH, qrand() % VIEW_HEIGHT));
        scene->addItem(aiList->at(i));
        while (aiList->at(i)->collidingItems().size() > 0)       //Prevents orbs from being placed on each other
            aiList->at(i)->setPos(qrand() % 3000, qrand() % 10000);
    }*/

    player = new PlayerOrb(); // the player
    scene->addItem(player);
    player->setFlag(QGraphicsItem::ItemIsFocusable); // these allow the player to take in key presses
    player->setFocus(); // Note: only one thing can be focused on, so other objects cant take key presses afaik

    view = new QGraphicsView(scene); // the view actually displays the scene
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // get rid of scroll bars
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedSize(WINDOW_WIDTH,WINDOW_HEIGHT);

    //  A nice gradient background
    QLinearGradient gradient(QPointF(WINDOW_WIDTH,0), QPointF(0,2000));
    gradient.setColorAt(0,Qt::blue);
    gradient.setColorAt(1,Qt::darkMagenta);

    scene->setBackgroundBrush(gradient);

    // Single timer that calls gameLoop, and move for the player and all AI
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(gameLoop()));;

    timer->start(20);

}

void Game::show()
{
    view->showFullScreen();
}

void Game::spawnAI()
{
    int max = 50;
    if (aiList->size() < max)
    {
        if (player->y() < 3500)
        {
            AIOrb* newAI = new AIOrb(20, qrand() % VIEW_WIDTH, (qrand() % 500) + (player->y()+1000));
            aiList->append(newAI);
            scene->addItem(newAI);
        }
        if (player->y() > 1500)
        {
            AIOrb* newAI2 = new AIOrb(30, qrand() % VIEW_WIDTH, (player->y()-1000) - (qrand() % 500));
            aiList->append(newAI2);
            scene->addItem(newAI2);
        }

    }

}

void Game::gameLoop()
{
    player->move();
    player->grow();
    player->setFocus(); // to fix the clicking issue - could be considered sloppy but it works
    view->centerOn(player);
    spawnAI();

    QList<QGraphicsItem*> itemList = view->items(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
    for (int i = 0; i < itemList.size(); i++)
    {
        AIOrb * aiorb = (AIOrb*)itemList[i];
        if (typeid(*(itemList[i])) == typeid(AIOrb))
            aiorb->move();
    }


}

