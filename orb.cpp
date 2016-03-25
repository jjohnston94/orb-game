#include "orb.h"

Orb::Orb()
{

}

void Orb::setRadius(qreal radius)
{
    this->radius = radius;
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

// Makes sure each directional velocity is greater than 0 and less than 'maxVelocity'
void Orb::verifyVelocities()
{
    for (int i = 0; i < 4; i++)
    {
        if (dirVelocity[i] > maxVelocity)
            dirVelocity[i] = maxVelocity;
        else if (dirVelocity[i] < 0)
            dirVelocity[i] = 0;
    }
}
