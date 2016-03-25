#include "aiorb.h"
#include <QTimer>
#include <QGraphicsScene>
#include <iostream>
AIOrb::AIOrb()
{
    radius = 20;
    setRect(0,0,getRadius()*2,getRadius()*2);
    setAcceleration(.5);
    setMaxVelocity(5);
    setPos(qrand() % 750, qrand() % 550);

    QTimer * timer = new QTimer();
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
    timer->start(20);
}

void AIOrb::move()
{
    for (int i = 0; i < 4; i++)
    {
        dirVelocity[i] = qrand() % 50; // random jerky movement
    }
    verifyVelocities();
    xVel = dirVelocity[1] - dirVelocity[0];
    yVel = dirVelocity[3] - dirVelocity[2];

    setPos(x() + xVel, y() + yVel);

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
