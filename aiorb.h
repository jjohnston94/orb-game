#ifndef AIORB_H
#define AIORB_H

#include <queue>
#include "orb.h"

class AIOrb : public Orb
{
public:
    AIOrb();
    AIOrb(qreal radius, int x, int y);
    void move();
    void growBy(qreal amount);
private:
    bool aiOn;
    std::queue<qreal> growQueue;
    void grow();
};

#endif // AIORB_H
