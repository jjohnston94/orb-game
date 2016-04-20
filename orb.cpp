#include "orb.h"
/*
 * The Orb class is the basis for all the objects that are
 * represented in the game scene. It is inherited by
 * PlayerOrb, AIOrb, and FeederOrb. If any other orbal objects
 * were implemented into the game, they would also
 * inherit Orb.
 *
 * Orb contains methods and variables primarily useful for
 * pseudo-realistic physics velocity and acceleration.
 * It allows the player to move around smoothly, and
 * the AI to move around smoothly.
 *
 * It was the first most important class.
 */

// No constuctor really needed for Orb since the base is never used in our case
Orb::Orb()
{

}

// Both sets the radius, and re-sets the pixmap to the proper scaled size
void Orb::setRadius(qreal radius)
{
    this->radius = radius;

    // Change size of pixmap since it depends on radius
    setPixmap(QPixmap(imageSource).scaled((int) radius*2,(int) radius*2));
}

// Gets the radius
qreal Orb::getRadius()
{
    return radius;
}

// Sets the actual radius
void Orb::setActualRadius(qreal actradius)
{
    this->actualRadius = actradius;
}

// Gets the actual radius
qreal Orb::getActualRadius()
{
    return actualRadius;
}

// Sets the acceleration
void Orb::setAcceleration(qreal acc)
{
    this->acceleration = acc;
}

// Gets the acceleration
qreal Orb::getAcceleration()
{
    return acceleration;
}

// Sets the max velocity
void Orb::setMaxVelocity(qreal max)
{
    this->maxVelocity = max;
}

// Gets the max velocity
qreal Orb::getMaxVelocity()
{
    return maxVelocity;
}

// Sets the dir velocity depending on the val
void Orb::setDirVelocity(int ind, qreal val)
{
    dirVelocity[ind] = val;
}

// Gets the directional velocities
qreal Orb::getDirVelocity(int ind)
{
    return dirVelocity[ind];
}

// Sets the x velocity
void Orb::setXVel(qreal xVel)
{
    this->xVel = xVel;
}

// Gets the x velocity
qreal Orb::getXVel()
{
    return xVel;
}

// Sets the y velocity
void Orb::setYVel(qreal yVel)
{
    this->yVel = yVel;
}

// Gets the y velocity
qreal Orb::getYVel()
{
    return yVel;
}

// Makes sure each directional velocity is greater than 0 and less than 'maxVelocity'
void Orb::verifyVelocities()
{
    // For every directional velocity
    for (int i = 0; i < 4; i++)
    {
        // If the current directional velocity is greater than the maximum velocity
        if (dirVelocity[i] > maxVelocity)
        {
            // Floor it to the maximum velocity
            dirVelocity[i] = maxVelocity;
        }
        // If the current direcitonal vel is less than 0
        else if (dirVelocity[i] < 0)
        {
            // Set it to 0
            dirVelocity[i] = 0;
        }

        // Make sure the x and y velocities are not too large or too small
        if (xVel > maxVelocity)
        {
            xVel = maxVelocity;
        }
        else if (xVel < -maxVelocity )
        {
            xVel = -maxVelocity;
        }
        if (yVel > maxVelocity)
        {
            yVel = maxVelocity;
        }
        else if (xVel < -maxVelocity)
        {
            yVel = -maxVelocity;
        }
    }
}


