#ifndef AIORB_H
#define AIORB_H

#include "orb.h"
class AIOrb : public Orb
{
    Q_OBJECT // this macro or whatever is necessary for slots and signals? (still not sure how those work)
public:
    AIOrb();
    AIOrb(qreal radius);
    void test();
public slots:
    void move();
};

#endif // AIORB_H
