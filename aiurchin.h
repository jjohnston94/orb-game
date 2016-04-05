#ifndef AIURCHIN_H
#define AIURCHIN_H

#include "orb.h"
class AIUrchin : public Orb
{
    Q_OBJECT 
public:
    AIUrchin();
    AIUrchin(qreal radius);
public slots:
    void move();
};

#endif
