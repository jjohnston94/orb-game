#include "aiorb.h"
#include <QTimer>
#include <QGraphicsScene>
#include <iostream>
#include <QList>
#include <QPixmap>
#include <typeinfo>
#include "playerorb.h"

AIOrb::AIOrb()
{

}

AIOrb::AIOrb(qreal radius)
{
    this->radius = radius -.5; // .5 is being added to radius elsewhere in the program and I dont know why
    setPixmap(QPixmap(":/images/resources/black.png").scaled(radius*2,radius*2)); // set the pixmap image and then scale it to the radius
    setAcceleration(.5);
    setMaxVelocity(5);
    do { // this is supposed to prevent aiorbs from being placed on top of each other but it doesn't seem to work
        setPos(qrand() % 750, qrand() % 550);
    } while (collidingItems().size() > 0);

    QTimer * timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(20);
}

void AIOrb::move()
{ 
    // the movement stuff below is just completely temporary and doesn't really do much, still need actual AI and collision prevention

    /*for (int i = 0; i < 4; i++)
    {
        dirVelocity[i] = qrand() % 50; // random jerky movement
    }

    xVel = dirVelocity[1] - dirVelocity[0];
    yVel = dirVelocity[3] - dirVelocity[2];


    setPos(x() + xVel, y() + yVel);

    // Prevent intersection with other orbs
    QList<QGraphicsItem *> collisions = collidingItems();
    for (int i = 0; i < collisions.size(); i++)
    {
        for (int j = 0; j < 4; j++)
        {
            Orb * current = (Orb*)collisions[i];
            qreal centerDist = sqrt((double) 2*radius*radius);
        }
    }*/

    // Keep orb inside window
    if (x() < 0)
        setPos(0, y());
    else if (x() + 2*radius > scene()->width())
        setPos(scene()->width() - 2*radius, y());
    if (y() < 0)
        setPos(x(), 0);
    else if (y() + 2*radius > scene()->height())
        setPos(x(), scene()->height() - 2*radius);

}

AIUrchin::AIUrchin()
{
    
}

AIUrchin::AiUrchin(qreal radius)        //where do i set radius const?
{
    this->radius = radius -.5; // .5 is being added to radius elsewhere in the program and I dont know why
    setPixmap(QPixmap(":/images/resources/black.png").scaled(radius*2,radius*2)); // set the pixmap image and then scale it to the radius
    setAcceleration(0);     //^^^ add urchin png
    setMaxVelocity(0);
    do { // this is supposed to prevent aiorbs from being placed on top of each other but it doesn't seem to work
        setPos(qrand() % 750, qrand() % 550);
    } while (collidingItems().size() > 0);

    QTimer * timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));      // do i need to worry about this for urchin?
    timer->start(20);

}
