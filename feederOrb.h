#ifndef FEEDERORB_H
#define FEEDERORB_H

#include "orb.h"

class FeederOrb : public Orb
{
public:
    FeederOrb();
    FeederOrb(qreal radius, int x, int y);
};
#endif // FEEDERORB_H
