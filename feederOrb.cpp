#include "feederorb.h"
#include <QPixmap>
#include <QString>
#include <QGraphicsScene>

/*
 * Feeder Orbs are small harmless orbs that
 * the player can eat at the very beginning to
 * gain size up to a maximum. This can be used as
 * a crutch for the player so they can gain some size
 * more easily than chasing down other orbs
 */

FeederOrb::FeederOrb()
{

}

FeederOrb::FeederOrb(qreal radius, int x, int y)
{
    // Different images for the feeder orbs so there can be variability
    QString sources[4] = {":/images/resources/greenFood.png",
                         ":/images/resources/blueFood.png",
                         ":/images/resources/pinkFood.png",
                         ":/images/resources/yellowFood.png"};

    // Random image source
    imageSource = sources[qrand() % 4];
    setRadius(radius);
    maxVelocity = 5;
    setPos(x,y);
}

void FeederOrb::move()
{
    // Give the orbs semirandom wiggly movement
    for (int i = 0; i < 4; i++)
        dirVelocity[i] = qrand() % 5;

    xVel = dirVelocity[1] - dirVelocity[0]; // range of -max to max velocity
    yVel = dirVelocity[3] - dirVelocity[2];

    // Move in a direction
    setPos(x() + xVel, y() + yVel);

    // Keep orb inside window
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

}
