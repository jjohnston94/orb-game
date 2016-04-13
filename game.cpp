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
    // Get the specific geometry of the user's screen to get the window size
    QRect rec = QApplication::desktop()->screenGeometry(); // get screen geometry for width and height
    WINDOW_HEIGHT = rec.height();
    WINDOW_WIDTH = rec.width();

    // Total size of the game environment (on and off screen)
    VIEW_WIDTH = 3000;
    VIEW_HEIGHT = 5000;

    // Initialize the scene, which holds QGraphicsItems
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0, VIEW_WIDTH, VIEW_HEIGHT);

    // Initialize list of AIOrbs so we can manipulate all of them
    aiList = new QList<AIOrb *>();

    // Initialize the player, add them to the scene, and allow them to be focused on
    player = new PlayerOrb(); // the player
    scene->addItem(player);
    player->setFlag(QGraphicsItem::ItemIsFocusable); // Focus allows the object to take in keypresses

    // Initialize the view and set it to display the scene
    view = new QGraphicsView(scene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // get rid of scroll bars
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedSize(WINDOW_WIDTH,WINDOW_HEIGHT);

    //  A nice gradient background
    QLinearGradient gradient(QPointF(WINDOW_WIDTH,0), QPointF(0,2000));
    gradient.setColorAt(0,Qt::blue);
    gradient.setColorAt(1,Qt::darkMagenta);
    scene->setBackgroundBrush(gradient);

    // Single timer that calls gameLoop which controls the movements of the other objects
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(gameLoop()));;

    // Timer called every 20 milliseconds
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
            AIOrb* newAI = new AIOrb(39, qrand() % VIEW_WIDTH, (qrand() % 500) + (player->y()+1000));
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
    player->setFocus(); // to fix the clicking issue - could be considered sloppy but it works
    view->centerOn(player);

    spawnAI();

    // Only call "move()" for AIOrbs currently on the screen/view
    QList<QGraphicsItem*> itemList = view->items(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);
    for (int i = 0; i < itemList.size(); i++)
    {
        AIOrb * aiorb = (AIOrb*)itemList[i];
        if (typeid(*(itemList[i])) == typeid(AIOrb))
            aiorb->move();
    }


}

