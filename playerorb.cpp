#include "playerorb.h"
#include <QKeyEvent> // for key press events
#include <QTimer> //for timers to loop the move() method
#include <iostream> // purely for help finding bugs by printing stuff out
#include <QGraphicsScene> // so this->scene() can be used
#include <QList> // for list of colliding objects
#include "aiorb.h" // currently not needed
#include <typeinfo> // currently not needed (but good if we want to see if a colliding object is a specific type)

PlayerOrb::PlayerOrb()
{
    // these should probabl be paramaterized at some point
    radius = 20;
    setRect(0,0,getRadius()*2,getRadius()*2);
    setAcceleration(.5);
    setMaxVelocity(5);

    QTimer * timer = new QTimer(); // timer
    connect(timer, SIGNAL(timeout()), this, SLOT(move())); // connect timer to object and slot method
    timer->start(20); //call slotted method (move()) every 20 milliseconds
}

void PlayerOrb::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A)
        keyDirection[0] = true;
    else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D)
        keyDirection[1] = true;
    else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W)
        keyDirection[2] = true;
    else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_S)
        keyDirection[3] = true;
}

void PlayerOrb::keyReleaseEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A)
        keyDirection[0] = false;
    else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D)
        keyDirection[1] = false;
    else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W)
        keyDirection[2] = false;
    else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_S)
        keyDirection[3] = false;
}

void PlayerOrb::move()
{
    // resize orb in case radius changed due to eating
    setRect(0,0,radius*2,radius*2);
    // Check collisions
    QList<QGraphicsItem *> collisions = collidingItems(); // collidingItems(); provides a QList of QGraphicsItem * that this item is colliding with
    for (int i = 0; i < collisions.size(); i++)
    {
        qreal orbSize = collisions[i]->boundingRect().width()/2;// I cant just use getRadius() because the object is still a GraphicsItem, not an AIOrb
        radius += orbSize;
        scene()->removeItem(collisions[i]);
        delete collisions[i];
    }

    // Slow down or speed up
    for (int i = 0; i < 4; i++)
    {
        if (!keyDirection[i]) //if key is not being pressed
            dirVelocity[i] -= .2; //decrease speed
        else
            dirVelocity[i] += getAcceleration(); //increase speed
    }
    verifyVelocities();

    // Set the x and y velocities based on right/left up/down velocities
    xVel = dirVelocity[1] - dirVelocity[0]; // -max to max velocity
    yVel = dirVelocity[3] - dirVelocity[2];

    // Move in a direction
    setPos(x() + xVel, y() + yVel);

    // Keep player inside window
    if (x() < 0)
        setPos(0, y());
    else if (x() + 2*radius > scene()->width())
        setPos(scene()->width() - 2*radius, y());
    if (y() < 0)
        setPos(x(), 0);
    else if (y() + 2*radius > scene()->height())
        setPos(x(), scene()->height() - 2*radius);

}


