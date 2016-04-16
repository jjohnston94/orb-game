#include "feederorb.h"
#include <QPixmap>
#include <QString>
#include <QGraphicsScene>

FeederOrb::FeederOrb()
{

}

FeederOrb::FeederOrb(qreal radius, int x, int y)
{
    QString sources[4] = {":/images/resources/greenFood.png",
                         ":/images/resources/blueFood.png",
                         ":/images/resources/pinkFood.png",
                         ":/images/resources/yellowFood.png"};
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
        setPos(0, y());
    else if (x() + 2*radius > scene()->width())
        setPos(scene()->width() - 2*radius, y());
    if (y() < 0)
        setPos(x(), 0);
    else if (y() + 2*radius > scene()->height())
        setPos(x(), scene()->height() - 2*radius);

}
