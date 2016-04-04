#ifndef PLAYERORB_H
#define PLAYERORB_H
#include <queue>
#include "orb.h"
class PlayerOrb : public Orb
{
    Q_OBJECT
public:
    float growFactor = 2.0; // determines the amount the orb grows each time another orb is eaten
    std::queue<int> growQueue;
    PlayerOrb();
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent *event);
public slots:
    void move();
    void grow(/*float growFactor*/);
private:
    bool keyDirection[4];
    qreal b;
};

#endif // PLAYERORB_H
