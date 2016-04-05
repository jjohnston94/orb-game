#include "orb.h"

Orb::Orb()
{

}

void Orb::setRadius(qreal radius)
{
    this->radius = radius;
    setPixmap(QPixmap(imageSource).scaled((int) radius*2,(int) radius*2)); // change size of image since it depends on radius
}

qreal Orb::getRadius()
{
    return radius;
}

void Orb::setAcceleration(qreal acc)
{
    this->acceleration = acc;
}

qreal Orb::getAcceleration()
{
    return acceleration;
}

void Orb::setMaxVelocity(qreal max)
{
    this->maxVelocity = max;
}

qreal Orb::getMaxVelocity()
{
    return maxVelocity;
}

void Orb::setDirVelocity(int ind, qreal val)
{
    dirVelocity[ind] = val;
}

qreal Orb::getDirVelocity(int ind)
{
    return dirVelocity[ind];
}

void Orb::setXVel(qreal xVel)
{
    this->xVel = xVel;
}

qreal Orb::getXVel()
{
    return xVel;
}

void Orb::setYVel(qreal yVel)
{
    this->yVel = yVel;
}

qreal Orb::getYVel()
{
    return yVel;
}

// Makes sure each directional velocity is greater than 0 and less than 'maxVelocity'
void Orb::verifyVelocities()
{
    for (int i = 0; i < 4; i++)
    {
        if (dirVelocity[i] > maxVelocity)
            dirVelocity[i] = maxVelocity;
        else if (dirVelocity[i] < 0)
            dirVelocity[i] = 0;

        if (xVel > maxVelocity * 2) // maxvelocity *2 so that colliding ai orbs actually change speed?
            xVel = maxVelocity * 2;
        else if (xVel < -maxVelocity * 2)
            xVel = -maxVelocity * 2;
        if (yVel > maxVelocity * 2)
            yVel = maxVelocity * 2;
        else if (xVel < -maxVelocity * 2)
            yVel = -maxVelocity * 2;
    }
}


