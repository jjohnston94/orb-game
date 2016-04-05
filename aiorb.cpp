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

AIOrb::AIOrb(qreal radius)
{
    this->radius = radius -.5; // .5 is being added to radius elsewhere in the program and I dont know why
    imageSource = ":/images/resources/black.png";
    setPixmap(QPixmap(imageSource).scaled(radius*2,radius*2)); // set the pixmap image and then scale it to the radius
    setAcceleration(.5);
    setMaxVelocity(5);
    setPos(qrand() % 1900*2, qrand() % 1060*2);

}

void AIOrb::move()
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
    /*
    //Keep away from the walls
    //Not sure if this is actually useful
    if (x() < range/4) {
        xVMag += cos(Pi/4)*(range - x())/4;
        yVMag -= sin(Pi/4)*(range - x())/4;
    }
    if (y() < range/4) {
        xVMag += cos(-Pi/4)*(range - y())/4;
        yVMag -= sin(-Pi/4)*(range - y())/4;
    }
    if (x()+ 2*radius > scene()->width() - range/4){
        xVMag += cos(-Pi/4)*(range - y())/4;
        yVMag += sin(-Pi/4)*(range - y())/4;
    }
    if (y()+ 2*radius > scene()->height() - range/4){
        xVMag += cos(Pi/4)*(range - y())/4;
        yVMag += sin(Pi/4)*(range - y())/4;
    }*/

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

    /*
    bool lAngleGood = false, rAngleGood = false;
    qreal lang = atan(yVel/xVel), rang = lang;
    while (true){
        if (lAngleGood){
            xVel += cos(lang);
            yVel += sin(lang);
            break;
        }
        if (x() + cos(lang) < 50 && y() + sin(lang) < 50 && sqrt(pow((x()+cos(lang)-50),2) + pow((y()+sin(lang)-50),2)) > 50){
            lang += Pi/36;
        } else lAngleGood;
    }*/

    setPos(x() + xVel, y() + yVel);


    QList<QGraphicsItem *> collisions = collidingItems();
    for (int i = 0; i < collisions.size(); i++) {
        Orb * current = (Orb*)collisions[i]; // cast the colliding objects as Orb references so their member functions can be accessed
        qreal oradius = current->getRadius();
        if (radius > oradius && typeid(*current) != typeid(PlayerOrb)) { // if the Orb is smaller
            setRadius(sqrt((double) (radius*radius + oradius*oradius)));
            delete collisions[i]; // delete the orb we just ate
        }
        else if (typeid(*current) == typeid(PlayerOrb))
        {
            current->setRadius(current->getRadius() -.1);
            if (current->getRadius() < 10)
                current->setRadius(10);
        }
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

    if (radius <= 4) // delete an orb if it gets too small
        delete this;
}
