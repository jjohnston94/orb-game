#include "aiorb.h"
#include <QTimer>
#include <QGraphicsScene>
#include <iostream>
#include <QList>
#include <QPixmap>
#include <typeinfo>
#include "playerorb.h"
#include "game.h"

extern Game * game; // this allows AIOrb to access the global "game" variable declared in the main cpp file
static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

AIOrb::AIOrb()
{

}

AIOrb::AIOrb(qreal radius, int x, int y)
{
    imageSource = ":/images/resources/pinkFood2.png";
    setRadius(radius); // set the pixmap image and then scale it to the radius
    actualRadius = radius;
    setAcceleration(.5);
    setMaxVelocity(5);
    setPos(x, y);
    aiOn = true;

}

void AIOrb::grow()
{
    // checks if there is anything in the grow queue. if so, increase the size by the next amount in the queue
    if (growQueue.size() > 0)
    {
        setRadius(radius + growQueue.front());
        growQueue.pop();
    }
}

void AIOrb::growBy(qreal amount)
{
    for (int j=0; j<5; j++)
        growQueue.push(amount/5);
}

void AIOrb::shrinkBy(qreal amount)
{
    for (int j=0; j<5; j++)
        growQueue.push(-amount/5);
}

void AIOrb::move()
{
    if (aiOn)
    {
        setMaxVelocity(-0.1*radius+9); //Set the max velocity to be a function of the size.
        qreal range = 50*maxVelocity; //The distance that an orb can see around itself is a function of its max velocity
        qreal xVMag = 0, yVMag = 0;

        //Check for a range around an orb, and then run from or chase the orbs in that range
        QList<QGraphicsItem *> closeOrb = scene()->items(QPolygonF()
                                                         << mapToScene(range+2*radius, range+2*radius)
                                                         << mapToScene(-range, range+2*radius)
                                                         << mapToScene(-range, -range)
                                                         << mapToScene(range+2*radius, -range));

        foreach(QGraphicsItem *item, closeOrb){
            Orb * current = (Orb*)item;
            if (item == this)
                continue;
            QLineF lineToOrb(QPointF(radius, radius), mapFromItem(item, current->getRadius(),current->getRadius())); //The distance between the midpoints
            qreal distance = lineToOrb.length() - current->getRadius(); //The distance to the border of the orb
            qreal angle = ::acos(lineToOrb.dx() / lineToOrb.length()); //The angle of the orb
            if (lineToOrb.dy() > 0)
                angle = TwoPi - angle;
            qreal orbEffect = range - distance; //The value for the effect that the current orb will have on this orb
            if (orbEffect < 0) orbEffect = 0;
            if (radius <= current->getRadius()){ //This orb is smaller than the current orb, run away.
                xVMag -= cos(angle)*(orbEffect);
                yVMag += sin(angle)*(orbEffect);
            } else if (radius > current ->getRadius()){ //This orb is larger, chase.
                xVMag += cos(angle)*(orbEffect);
                yVMag -= sin(angle)*(orbEffect);
            }
        }

        //Scale xVMag and yVMag to the max velocity
        xVel = xVMag * maxVelocity / sqrt((double) xVMag*xVMag+yVMag*yVMag+0.0001);
        yVel = yVMag * maxVelocity / sqrt((double) xVMag*xVMag+yVMag*yVMag+0.0001);
        //If the orb is running into a wall, redirect it to a side instead
        if (x() + xVel < 0 || x()+2*radius + xVel > scene()->width()){
            xVMag = 0;
            xVel = xVMag * maxVelocity / sqrt((double) xVMag*xVMag+yVMag*yVMag+0.0001);
            yVel = yVMag * maxVelocity / sqrt((double) xVMag*xVMag+yVMag*yVMag+0.0001);
        }
        if (y() + yVel < 0 || y()+2*radius+yVel > scene()->height()){
            yVMag = 0;
            xVel = xVMag * maxVelocity / sqrt((double) xVMag*xVMag+yVMag*yVMag+0.0001);
            yVel = yVMag * maxVelocity / sqrt((double) xVMag*xVMag+yVMag*yVMag+0.0001);
        }

        setPos(x() + xVel, y() + yVel);
    }

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
