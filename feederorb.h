#ifndef FEEDERORB_H
#define FEEDERORB_H

#include "orb.h"
class FeederOrb : public Orb
{
    Q_OBJECT // this macro or whatever is necessary for slots and signals? (still not sure how those work)
public:
    FeederOrb();
    FeederOrb(qreal radius, int x, int y);
};
#endif // FEEDERORB_H
