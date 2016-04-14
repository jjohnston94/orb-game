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
    void shrinkBy(qreal amount);
    void grow();
private:
    bool aiOn;
    std::queue<qreal> growQueue;
};

#endif // AIORB_H
