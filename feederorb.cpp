#include "feederorb.h"
#include <QPixmap>

FeederOrb::FeederOrb()
{

}

FeederOrb::FeederOrb(qreal radius, int x, int y)
{
    radius = 5;
    imageSource = ":/images/resources/black.png";
    setPixmap(QPixmap(imageSource).scaled(radius*2,radius*2));
    setPos(x,y);
}
