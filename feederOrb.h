#ifndef FEEDERORB_H
#define FEEDERORB_H

#include "orb.h"

class FeederOrb : public Orb
{
public:
    FeederOrb();
    FeederOrb(qreal radius, int x, int y);
    void move();
};
#endif // FEEDERORB_H
