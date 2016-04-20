#include "playerorb.h"
#include <QKeyEvent> // for key press events
#include <QTimer> //for timers to loop the move() method
#include <iostream> // purely for help finding bugs by printing stuff out
#include <QGraphicsScene> // so this->scene() can be used
#include <QList> // for list of colliding objects
#include "aiorb.h"
#include "game.h"
#include <typeinfo> // used to make sure we dont decrease the player when decreasing ai orbs
#include <QRectF>
#include <QPixmap>
#include <QGraphicsView>
#include <math.h>

extern Game * game; // this allows playerOrb to access the global "game" variable declared in the main cpp file

PlayerOrb::PlayerOrb()
{
    // these should probably be paramaterized at some point

    imageSource = ":/images/resources/mc1.png";
    setRadius(40);
    actualRadius = radius;
    setPos(1500, 1000);
    setAcceleration(2);
    setMaxVelocity(10);

}

void PlayerOrb::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A)
    {
        keyDirection[0] = true;
    }
    else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D)
    {
        keyDirection[1] = true;
    }
    else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W)
    {
        keyDirection[2] = true;
    }
    else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_S)
    {
        keyDirection[3] = true;
    }
}

void PlayerOrb::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A)
    {
        keyDirection[0] = false;
    }
    else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D)
    {
        keyDirection[1] = false;
    }
    else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W)
    {
        keyDirection[2] = false;
    }
    else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_S)
    {
        keyDirection[3] = false;
    }

    if (event->key() == Qt::Key_F)
    {
        QGraphicsView * view = scene()->views()[0];
        if (view->isFullScreen())
        {
            view->showNormal();
        }
        else
        {
            view->showFullScreen();
        }
    }
    else if (event->key() == Qt::Key_Escape)
    {
        QGraphicsView * view = scene()->views()[0];
        view->showNormal();
    }


}

void PlayerOrb::grow()
{
    // checks if there is anything in the grow queue. if so, increase the size by the next amount in the queue
    if (growQueue.size() > 0)
    {
        setRadius(radius + growQueue.front());
        growQueue.pop();
    }
}

void PlayerOrb::growBy(qreal amount)
{
    for (int j=0; j<5; j++)
    {
        growQueue.push(amount/5);
    }
}

void PlayerOrb::shrinkBy(qreal amount)
{
    for (int j=0; j<5; j++)
    {
        growQueue.push(-amount/5);
    }
}

void PlayerOrb::correctImage()
{
    qreal inc = 581.25/6;
    if (actualRadius > inc*5)
    {
        imageSource = ":/images/resources/mc6.png";
    }
    else if (actualRadius > inc*4)
    {
        imageSource = ":/images/resources/mc5.png";
    }
    else if (actualRadius > inc*3)
    {
        imageSource = ":/images/resources/mc4.png";
    }
    else if (actualRadius > inc*2)
    {
        imageSource = ":/images/resources/mc3.png";
    }
    else if (actualRadius > inc)
    {
        imageSource = ":/images/resources/mc2.png";
    }
    else
    {
        imageSource = ":/images/resources/mc1.png";
    }
}

void PlayerOrb::move()
{
    // MOVEMENT
    // Slow down or speed up
    for (int i = 0; i < 4; i++)
    {
        if (!keyDirection[i]) //if key is not being pressed
        {
            dirVelocity[i] -= getAcceleration()/5; //decrease speed
        }
        else
        {
            dirVelocity[i] += getAcceleration(); //increase speed
        }
    }
    verifyVelocities();

    // Set the x and y velocities based on right/left up/down velocities
    xVel = dirVelocity[1] - dirVelocity[0]; // range of -max to max velocity
    yVel = dirVelocity[3] - dirVelocity[2];

    // Move in a direction
    setPos(x() + xVel, y() + yVel);

    // Grow the player if there is something in the growQueue
    grow();

    // Keep player inside window
    if (x() < 0)
    {
        setPos(0, y());
    }
    else if (x() + 2*radius > scene()->width())
    {
        setPos(scene()->width() - 2*radius, y());
    }
    if (y() < 0)
    {
        setPos(x(), 0);
    }
    else if (y() + 2*radius > scene()->height())
    {
        setPos(x(), scene()->height() - 2*radius);
    }

    //Keep the player from going to a level it shouldn't
    if (radius < 80 && y() >= game->getScale() * 4000)
    {
        setPos(x(), (game->getScale() * 4000) - 1);
        yVel = 0;
    }
    else if (radius > 320 && y() <= (game->getScale( )- 1) * 4000)
    {
        setPos(x(), ((game->getScale() - 1) * 4000) + 1);
        yVel = 0;
    }
}

void PlayerOrb::stop(){
    xVel = 0;
    yVel = 0;
    for (int i = 0; i < 4; i++)
    {
        dirVelocity[i] = 0;
    }
}
