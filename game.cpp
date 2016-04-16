#include "game.h"
#include <iostream>
#include <QTimer>
#include <QList>
#include <QRect>
#include <QApplication>
#include <QDesktopWidget>
#include <typeinfo>
#include "feederOrb.h"

Game::Game()
{
    // Get the specific geometry of the user's screen to get the window size
    QRect rec = QApplication::desktop()->screenGeometry(); // get screen geometry for width and height
    WINDOW_HEIGHT = rec.height();
    WINDOW_WIDTH = rec.width();

    // Total size of the game environment (on and off screen)
    SCENE_WIDTH = 3000;
    SCENE_HEIGHT = 20000;

    // Initialize the scene, which holds QGraphicsItems
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0, SCENE_WIDTH, SCENE_HEIGHT);

    // Initialize list of AIOrbs so we can manipulate all of them
    aiList = new QList<AIOrb *>();

    // Scale
    scale = 1;
    lastScale = 1;

    // Initialize the player, add them to the scene, and allow them to be focused on
    player = new PlayerOrb(); // the player
    scene->addItem(player);
    player->setFlag(QGraphicsItem::ItemIsFocusable); // Focus allows the object to take in keypresses
    player->setFocus();

    // Initialize the view and set it to display the scene
    view = new QGraphicsView(scene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff); // get rid of scroll bars
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setFixedSize(WINDOW_WIDTH,WINDOW_HEIGHT);

    //  A nice gradient background
    QRadialGradient gradient(QPointF(SCENE_WIDTH/2, SCENE_HEIGHT/2), 75);
    gradient.setColorAt(0,Qt::blue);
    gradient.setColorAt(1,Qt::black);
    gradient.setSpread(QGradient::ReflectSpread);
    scene->setBackgroundBrush(gradient);
    //scene->setBackgroundBrush(QPixmap(":/images/resources/bg.png"));

    // Single timer that calls gameLoop which controls the movements of the other objects
    timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(gameLoop()));;

}

void Game::show()
{
    view->showFullScreen();
    // Timer called every 20 milliseconds
    timer->start(20);
}

// Spawn the AI (currently only below)
void Game::spawnAI()
{
    int max = 50;
    if (aiList->size() < max)
    {
        if (player->y() > 0 && player->y() < SCENE_HEIGHT-WINDOW_HEIGHT)
        {
            AIOrb* newAI = new AIOrb(10 + qrand()%(int)player->getRadius()-10, qrand() % SCENE_WIDTH, (qrand() % 1000) + (player->y()+WINDOW_HEIGHT));
            aiList->append(newAI);
            scene->addItem(newAI);
        }
        if (player->y() < SCENE_HEIGHT && player->y() > WINDOW_HEIGHT && false)
        {
            AIOrb* newAI2 = new AIOrb(10 + qrand()%(int)player->getRadius()-20, qrand() % SCENE_WIDTH, (player->y()-WINDOW_HEIGHT) - (qrand() % 1000));
            aiList->append(newAI2);
            scene->addItem(newAI2);
        }
    }
}

// Changes the size of the player and all AI based on scale
void Game::changeScale()
{
    if (scale < lastScale)
    {
        player->growBy( qAbs((player->getActualRadius() / scale ) - player->getRadius()));

        for (int i = 0; i < aiList->size(); i++)
        {
            AIOrb * current = aiList->at(i);
            current->growBy( qAbs((current->getActualRadius() / scale) - current->getRadius()));
        }
    }
    else
    {
        player->shrinkBy( qAbs((player->getActualRadius() / scale ) - player->getRadius()));

        for (int i = 0; i < aiList->size(); i++)
        {
            AIOrb * current = aiList->at(i);
            current->shrinkBy( qAbs((current->getActualRadius() / scale) - current->getRadius()));
        }
    }

}

void Game::deleteAI(AIOrb * orb)
{
    scene->removeItem((QGraphicsItem*) orb);
    aiList->removeAt(aiList->indexOf(orb));
    delete orb;
}

void Game::gameLoop()
{
    player->move();

    // Check collisions with the player
    QList<QGraphicsItem *> collisions = player->collidingItems();

    // For every item the player is colliding with
    for (int i = 0; i < collisions.size(); i++)
    {
        // Cast the item as an Orb
        Orb * current = (Orb*)collisions[i]; // cast the colliding objects as Orb references so their member functions can be accessed
        qreal aiRadius = current->getRadius();
        qreal pRadius = player->getRadius();

        // If the player is bigger than the other (AI)
        if (pRadius > aiRadius)
        {
            // Remove the item from the scene and aiList and delete it
            deleteAI((AIOrb*)current);

            // Add the area of the eaten orb to the player but don't let the player grow bigger than 300 from eating
            if (player->getRadius() < 300)
            {
                qreal radiusDiff = sqrt( (double) (pRadius*pRadius + aiRadius*aiRadius) ) - pRadius;
                player->setActualRadius(player->getActualRadius() + radiusDiff);
                player->growBy(radiusDiff);
            }
        }
    }

    // Re-set the focus to the player to make sure it can still move
    player->setFocus();

    // Center the view on the player so that it follows the player around
    view->centerOn(player);

    // Spawn AIOrbs if applicable
    spawnAI();

    // Move AI if in view and check their collisions
    moveCollideAI();

    // Make sure all of the AI grow/shrink regardless of whether they are in the view
    for (int i = 0; i < aiList->size(); i++)
        aiList->at(i)->grow();

    // If an AI gets too small, delete it
    for (int i = 0; i < aiList->size(); i++)
    {
        if (aiList->at(i)->getRadius() < 10)
            deleteAI(aiList->at(i));
    }

    // Make the scale of everything change at height intervals
    int divisor = SCENE_HEIGHT / 5;
    if (lastScale != scale)
    {
        changeScale();
        lastScale = scale;
    }
    scale = ((int) (player->y()/divisor)) + 1;

}

void Game::moveCollideAI()
{
    // Get a list of all the items seen in the view
    itemViewList = view->items(0,0,WINDOW_WIDTH,WINDOW_HEIGHT);

    // For every item seen in the view
    for (int i = 0; i < itemViewList.size(); i++)
    {
        // Cast each item in the list as an Orb
        Orb * currentOrb = (Orb*)itemViewList[i];

        // If the item is an AIOrb
        if (typeid(*(currentOrb)) == typeid(AIOrb))
        {
            // Cast it as an AIOrb, and allow it to move
            AIOrb * aiOrb = (AIOrb*)currentOrb;
            aiOrb->move();

            // Get a list of collisions with the current AIOrb
            QList<QGraphicsItem*> collisions = aiOrb->collidingItems();
            qreal thisRadius = aiOrb->getRadius();

            // For every item the current AIOrb is colliding with
            for (int i = 0; i < collisions.size(); i++)
            {
                // Cast it as an Orb
                Orb * collided = (Orb*)collisions[i];
                qreal oradius = collided->getRadius();

                // If the current AIOrb is bigger than the other object (and the other object is an AIOrb)
                if (thisRadius > oradius && typeid(*(collided)) == typeid(AIOrb))
                {
                    // Remove the item from the scene and aiList and delete it
                    itemViewList.removeAt(itemViewList.indexOf(collisions[i]));
                    deleteAI((AIOrb*)collided);

                    // Then increase the size of the current AIOrb
                    if (aiOrb->getRadius() < player->getRadius() + 20)
                    {
                        qreal radiusDiff = (sqrt((double) (thisRadius*thisRadius + oradius*oradius))) - thisRadius;
                        aiOrb->growBy(radiusDiff);
                        aiOrb->setActualRadius(aiOrb->getActualRadius() + radiusDiff);
                    }

                }

                // If the current AIOrb is bigger than the player
                else if (thisRadius > oradius && typeid(*(collided)) == typeid(PlayerOrb))
                {/*
                    if (player->getRadius() > 15)
                    {
                        player->shrinkBy(1);
                        player->setActualRadius(player->getActualRadius()-1);
                    }*/
                }
            }
        }
    }
}

