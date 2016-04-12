#ifndef PLAYERORB_H
#define PLAYERORB_H

#include <queue>
#include "orb.h"
class PlayerOrb : public Orb
{
    Q_OBJECT
public:
    PlayerOrb();
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent *event);
public slots:
    void move();
    void grow();
private:
    bool keyDirection[4];
    qreal b;
    // I made these private
    double growFactor; // determines the amount the orb grows each time another orb is eaten
    std::queue<int> growQueue;
};

#endif // PLAYERORB_H
