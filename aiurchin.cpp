#include <QTimer>
#include <QGraphicsScene>
#include <iostream>
#include <QList>
#include <QPixmap>
#include <typeinfo>
#include "playerorb.h"


AIUrchin::AIUrchin()
{
  
}

AIUrchin::AIUrchin(qreal radius)
{
  */create urchin here*/
  this->radius = radius -.5; // .5 is being added to radius elsewhere in the program and I dont know why
    setPixmap(QPixmap(":/images/resources/black.png").scaled(radius*2,radius*2)); // set the pixmap image and then scale it to the radius

    do { // this is supposed to prevent aiorbs from being placed on top of each other but it doesn't seem to work
        setPos(qrand() % 750, qrand() % 550);
    } while (collidingItems().size() > 0);
}

void AIUrchin::shrink()
{
  */method for shrinking player or aiorbs if hit */
}
