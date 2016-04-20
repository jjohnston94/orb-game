#include "aiorb.h"
#include <QTimer>
#include <QGraphicsScene>
#include <iostream>
#include <QList>
#include <QPixmap>
#include <typeinfo>
#include "playerorb.h"
#include "game.h"

/*
 * AIOrb class extends the Orb class and uses the different images
 * to display the "enemies". Their movement is dependent upon any nearby
 * orbs. If they see a bigger orb in their sight radius, they run away.
 * If they see a smaller orb in their sight radius, they run away.
 * Their collisions with other orbs are handled in the "game.cpp" file.
 * Their faces also change based on whether they are chasing, running, or
 * staying still.
 */

extern Game * game; // this allows AIOrb to access the global "game" variable declared in the main cpp file
static const double Pi = 3.14159265358979323846264338327950288419717;
static double TwoPi = 2.0 * Pi;

AIOrb::AIOrb()
{

}

AIOrb::AIOrb(qreal radius, int x, int y)
{
    // Array of the image base names so they can have random faces
    QString sourceTypes[3] = {"orange", "purpleMask", "oreo"};
    baseImgName = sourceTypes[qrand() % 3];
    imageSource = ":/images/resources/" + baseImgName + "Neutral.png";

    // Scale for their size when they are spawned
    int thisScale = (y / 4000) + 1;
    setRadius((radius - 100*(thisScale - 1))/pow(2,(game->getScale() - thisScale))); // set the pixmap image and then scale it to the radius
    actualRadius = radius;

    // Set acceleration, maxvelocity, and starting position
    setAcceleration(.5);
    setMaxVelocity(5);
    setPos(x, y);

    // Variable to turn ai off when not needed
    aiOn = true;

}

// Grow if there is anything in the grow queue. Allows smooth growth.
void AIOrb::grow()
{
    // checks if there is anything in the grow queue. if so, increase the size by the next amount in the queue
    if (growQueue.size() > 0)
    {
        setRadius(radius + growQueue.front());
        growQueue.pop();
    }
}

// Add a given radius amount to the growQueue
void AIOrb::growBy(qreal amount)
{
    for (int j=0; j<5; j++)
        growQueue.push(amount/5);
}

// Add a negative amount to the growQueue
void AIOrb::shrinkBy(qreal amount)
{

    for (int j=0; j<5; j++)
        growQueue.push(-amount/5);
}

// AI for the movement chasing/running away from orbs
void AIOrb::move()
{
    if (aiOn)
    {
        //Set the max velocity to be a function of the size.
        setMaxVelocity(-0.1*radius+9);
        if (maxVelocity < 2) maxVelocity = 2;
        //The distance that an orb can see around itself is a function of its max velocity
        qreal range = 50*maxVelocity;
        if (range < 200) range = 200;
        qreal xVMag = 0, yVMag = 0;
        qreal chaseEffect = 0;

        //Check for a range around an orb, and then run from or chase the orbs in that range
        QList<QGraphicsItem *> closeOrb = scene()->items(QPolygonF()
                                                         << mapToScene(range+2*radius, range+2*radius)
                                                         << mapToScene(-range, range+2*radius)
                                                         << mapToScene(-range, -range)
                                                         << mapToScene(range+2*radius, -range));

        // For each item in the radius around this orb
        foreach(QGraphicsItem *item, closeOrb)
        {
            // Cast it as an Orb reference
            Orb * current = (Orb*)item;

            // If it's not a feeder orb
            if (typeid(*(current)) != typeid(FeederOrb))
            {
                // Skip the code if current is this orb
                if (item == this)
                    continue;

                // The distance between the midpoints
                QLineF lineToOrb(QPointF(radius, radius), mapFromItem(item, current->getRadius(),current->getRadius()));

                // The distance to the border of the orb
                qreal distance = lineToOrb.length() - current->getRadius() - radius;

                // The angle of the orb
                qreal angle = ::acos(lineToOrb.dx() / lineToOrb.length());

                // Do angle stuff
                if (lineToOrb.dy() > 0)
                {
                    angle = TwoPi - angle;
                }

                // The value for the effect that the current orb will have on this orb
                qreal orbEffect = range - distance;
                if (orbEffect < 0)
                {
                    orbEffect = 0;
                }

                // This orb is smaller than the current orb, run away.
                if (radius <= current->getRadius())
                {
                    xVMag -= cos(angle)*(orbEffect);
                    yVMag += sin(angle)*(orbEffect);
                    chaseEffect -= orbEffect;
                }

                //This orb is larger, chase.
                else if (radius > current->getRadius())
                {
                    xVMag += cos(angle)*(orbEffect);
                    yVMag -= sin(angle)*(orbEffect);
                    chaseEffect += orbEffect;
                }
            }
        }

        //Change Orb Face
        if (chaseEffect < 0)
        {
            imageSource = ":/images/resources/" + baseImgName + "Running.png";
        }
        else if (chaseEffect == 0)
        {
            imageSource = ":/images/resources/" + baseImgName + "Neutral.png";
        }
        else
        {
            imageSource = ":/images/resources/" + baseImgName + "Chasing.png";
        }

        // Set the radius again so that the image is correctly assigned
        setRadius(radius);

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

        // Set the actual position
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
