#include "feederorb.h"
#include <QPixmap>

FeederOrb::FeederOrb()
{

}

FeederOrb::FeederOrb(qreal radius, int x, int y)
{
    imageSource = ":/images/resources/blueFood.png";
    setRadius(radius);
    setPos(x,y);
}
