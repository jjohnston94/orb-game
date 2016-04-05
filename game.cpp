#include "game.h"
#include <iostream>
#include <QTimer>
#include <QList>

Game::Game()
{
    WINDOW_HEIGHT = 1080;
    WINDOW_WIDTH = 1920;

    scene = new QGraphicsScene(); // this holds graphicsitems and stuff
    scene->setSceneRect(0,0, WINDOW_WIDTH*2, WINDOW_HEIGHT*2);

    QList<AIOrb *> aiList = QList<AIOrb *>(); // List of ai orbs so we can manipulate them later
    for (int i = 0; i < 75; i++) // add ai orbs to scene
    {
        aiList.append(new AIOrb(qrand() % 50));
        scene->addItem(aiList[i]);
        while (aiList[i]->collidingItems().size() > 0)       //Prevents orbs from being placed on each other
            aiList[i]->setPos(qrand() % 750, qrand() % 550);
    }

    player = new PlayerOrb(); // the player
    scene->addItem(player);
    player->setFlag(QGraphicsItem::ItemIsFocusable); // these allow the player to take in key presses
    player->setFocus(); // Note: only one thing can be focused on, so other objects cant take key presses afaik

    view = new QGraphicsView(scene); // the view actually displays the scene
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // get rid of scroll bars
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedSize(WINDOW_WIDTH,WINDOW_HEIGHT);

    //  A nice gradient background
    QLinearGradient gradient(QPointF(WINDOW_WIDTH,0), QPointF(WINDOW_WIDTH,WINDOW_HEIGHT*2));
    gradient.setColorAt(0,Qt::blue);
    gradient.setColorAt(1,Qt::darkMagenta);

    scene->setBackgroundBrush(gradient);

    // Single timer that calls gameLoop, and move for the player and all AI
    QTimer * timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(gameLoop()));
    connect(timer, SIGNAL(timeout()), player, SLOT(move()));
    connect(timer, SIGNAL(timeout()), player, SLOT(grow()));
    for (int i = 0; i < aiList.size(); i++)
        connect(timer, SIGNAL(timeout()), aiList[i], SLOT(move()));

    timer->start(20);

}

void Game::show()
{
    view->showFullScreen();
}

void Game::gameLoop()
{
    player->setFocus(); // to fix the clicking issue - could be considered sloppy but it works
    view->centerOn(player);
}

