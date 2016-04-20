#include "playerorb.h"
#include <QKeyEvent>
#include <QTimer>
#include <iostream>
#include <QGraphicsScene>
#include <QList>
#include "aiorb.h"
#include "game.h"
#include <typeinfo>
#include <QRectF>
#include <QPixmap>
#include <QGraphicsView>
#include <math.h>

// This allows playerOrb to access the global "game" variable declared in the main cpp file
extern Game * game;

PlayerOrb::PlayerOrb()
{
    //The initial image used for the player
    imageSource = ":/images/resources/mc1.png";
    //The starting radius is 40
    setRadius(40);

    /*
     * The difference between actualRadius and radius
     * is that actualRadius reflects the actual radius
     * of the orb object itself, regardless of how
     * zoomed out the view is of the game. actualradius
     * changes whenever the player eats something or is
     * reset. radius reflects what the radius of the orb
     * is on the actual game screen, and depends on the
     * actualRadius and on what level the player is
     * in.
     */
    actualRadius = radius;
    //Set the initial position to 1500 x and 1000 y
    setPos(1500, 1000);
    //The acceleration is set to 2
    setAcceleration(2);
    /*
     * The max velocity is 10, this allows the player
     * to be faster than most other orbs, but still
     * makes the game challenging.
     */
    setMaxVelocity(10);

    //Make sure that the player starts with no direction
    for (int i = 0; i < 4; i++)
    {
        keyDirection[i] = false;
    }

}

//Check to see if any keys are pressed
void PlayerOrb::keyPressEvent(QKeyEvent *event)
{
    //If left arrow key or 'A' are pressed, change the first value
    //of keyDirection true to signify the orb is moving left.
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A)
    {
        keyDirection[0] = true;
    }
    //If right arrow key or 'D' are pressed, change the second value
    //of keyDirection true to signify the orb is moving right.
    else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D)
    {
        keyDirection[1] = true;
    }
    //If up arrow key or 'W' are pressed, change the third value
    //of keyDirection true to signify the orb is moving up.
    else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W)
    {
        keyDirection[2] = true;
    }
    //If down arrow key or 'S' are pressed, change the fourth value
    //of keyDirection true to signify the orb is moving down.
    else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_S)
    {
        keyDirection[3] = true;
    }
}

//Check to see if any keys have been released
void PlayerOrb::keyReleaseEvent(QKeyEvent *event)
{
    //If left arrow key or 'A' are released, change the first value
    //of keyDirection to false to signify the orb should stop moving left.
    if (event->key() == Qt::Key_Left || event->key() == Qt::Key_A)
    {
        keyDirection[0] = false;
    }
    //If right arrow key or 'D' are released, change the second value
    //of keyDirection to false to signify the orb should stop moving right.
    else if (event->key() == Qt::Key_Right || event->key() == Qt::Key_D)
    {
        keyDirection[1] = false;
    }
    //If up arrow key or 'W' are released, change the third value
    //of keyDirection to false to signify the orb should stop moving up.
    else if (event->key() == Qt::Key_Up || event->key() == Qt::Key_W)
    {
        keyDirection[2] = false;
    }
    //If down arrow key or 'S' are released, change the fourth value
    //of keyDirection to false to signify the orb should stop moving down.
    else if (event->key() == Qt::Key_Down || event->key() == Qt::Key_S)
    {
        keyDirection[3] = false;
    }

    //If key 'F' is pressed, toggle fullscreen
    if (event->key() == Qt::Key_F)
    {
        //Set view to point to the first view in scene
        //(the only view in scene)
        QGraphicsView * view = scene()->views()[0];
        //Switch to normal (windowed) if the view is fullscreen
        if (view->isFullScreen())
        {
            view->showNormal();
        }
        //Otherwise (the view is normal), switch to fullscreen
        else
        {
            view->showFullScreen();
        }
    }
    //If key 'ESC' is pressed, exit fullscreen
    else if (event->key() == Qt::Key_Escape)
    {
        //Set view to point to the first view in scene
        //(the only view in scene)
        QGraphicsView * view = scene()->views()[0];
        //Set the view to normal (windowed)
        view->showNormal();
    }
}

//Grow the player orb at a gradual rate
void PlayerOrb::grow()
{
    // Checks if there is anything in the grow queue. If so,
    // increase the size by the next amount in the queue
    if (growQueue.size() > 0)
    {
        //Grow the orb by adding the next value in the queue
        setRadius(radius + growQueue.front());
        //Pop the value from the queue.
        growQueue.pop();
    }
}

//Set up values to go into the grow queue if the player is growing
void PlayerOrb::growBy(qreal amount)
{
    //Split up the growing into 5 stages, so the growth is more gradual
    for (int j=0; j<5; j++)
    {
        //Push amount/5 into the queue 5 times
        growQueue.push(amount/5);
    }
}

//Set up values to go into the grow queue if the player is shrinking
void PlayerOrb::shrinkBy(qreal amount)
{
    //Split up the shrinking into 5 stages, so the shrinking is more gradual
    for (int j=0; j<5; j++)
    {
        //Push -amount/5 into the queue 5 times
        growQueue.push(-amount/5);
    }
}

//Change the player's image to correspond to its size
void PlayerOrb::correctImage()
{
    //This value comes from the largest size the orb could be divided by 6.
    qreal inc = 96.875;
    //Use the last image
    if (actualRadius > inc*5)
    {
        imageSource = ":/images/resources/mc6.png";
    }
    //Use the fifth image
    else if (actualRadius > inc*4)
    {
        imageSource = ":/images/resources/mc5.png";
    }
    //Use the fourth image
    else if (actualRadius > inc*3)
    {
        imageSource = ":/images/resources/mc4.png";
    }
    //Use the third image
    else if (actualRadius > inc*2)
    {
        imageSource = ":/images/resources/mc3.png";
    }
    //Use the second image
    else if (actualRadius > inc)
    {
        imageSource = ":/images/resources/mc2.png";
    }
    //Use the first image
    else
    {
        imageSource = ":/images/resources/mc1.png";
    }
}

//Move the player orb
void PlayerOrb::move()
{
    // MOVEMENT
    // Slow down or speed up
    for (int i = 0; i < 4; i++)
    {
        //if no direction is specified
        if (!keyDirection[i])
        {
            //decrease speed
            dirVelocity[i] -= getAcceleration()/5;
        }
        //if any direction is specified
        else
        {
            //increase speed
            dirVelocity[i] += getAcceleration();
        }
    }

    //Make sure the velocites do not become negative
    //or greater than the max velocity
    verifyVelocities();

    // Set the x and y velocities based on right/left up/down velocities
    // range of -max to max velocity
    xVel = dirVelocity[1] - dirVelocity[0];
    yVel = dirVelocity[3] - dirVelocity[2];

    // Move in a direction
    setPos(x() + xVel, y() + yVel);

    // Grow the player if there is something in the growQueue
    grow();

    // Keep player inside window
    //The player position is to the left of the game border
    if (x() < 0)
    {
        //Set the player position to the left game border
        setPos(0, y());
    }
    //The player position is to the right of the game border
    else if (x() + 2*radius > scene()->width())
    {
        //Set the player position to the right game border
        setPos(scene()->width() - 2*radius, y());
    }
    //The player position is above the game border
    if (y() < 0)
    {
        //Set the player position to the top game border
        setPos(x(), 0);
    }
    //The player position is below the game border
    else if (y() + 2*radius > scene()->height())
    {
        //Set the player position to the bottom game border
        setPos(x(), scene()->height() - 2*radius);
    }

    //Keep the player from going to a level it shouldn't
    //If the radius (the one represented on the screen) is
    //less than 80, don't allow it in the level below
    if (radius < 80 && y() >= game->getScale() * 4000)
    {
        setPos(x(), (game->getScale() * 4000) - 1);
        //Set the y velocity to 0
        yVel = 0;
    }
    //If the radius is greater than 320, don't allow it
    //in the level above (or else it would scale to be too big)
    else if (radius > 320 && y() <= (game->getScale( )- 1) * 4000)
    {
        setPos(x(), ((game->getScale() - 1) * 4000) + 1);
        //Set the y velocity to 0
        yVel = 0;
    }
}

// Full stop for the player (to reset velocities if necessary)
void PlayerOrb::stop()
{
    xVel = 0;
    yVel = 0;
    //Reset all the directional velocities
    for (int i = 0; i < 4; i++)
    {
        dirVelocity[i] = 0;
    }
}
