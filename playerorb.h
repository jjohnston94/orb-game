#ifndef PLAYERORB_H
#define PLAYERORB_H

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
private:
    bool keyDirection[4];
    qreal b;
};

#endif // PLAYERORB_H
