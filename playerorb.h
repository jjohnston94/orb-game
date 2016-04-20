#ifndef PLAYERORB_H
#define PLAYERORB_H

#include <queue>
#include "orb.h"
class PlayerOrb : public Orb
{
public:
    PlayerOrb();
    void keyPressEvent(QKeyEvent * event);
    void keyReleaseEvent(QKeyEvent *event);
    void move();
    void stop();
    void growBy(qreal amount);
    void shrinkBy(qreal amount);
    void correctImage();
private:
    bool keyDirection[4]; // 0L 1R 2U 3D
    qreal b;
    std::queue<qreal> growQueue;

    void grow();
};

#endif // PLAYERORB_H
