#ifndef ORB_H
#define ORB_H

#include <QGraphicsEllipseItem>
#include <QObject>

class Orb: public QObject, public QGraphicsEllipseItem // QGraphicsEllipseItem inherits QGraphicsItem
        // we inherit QObject here so that both playerorb and aiorb inherit it (it allows signals and slots)
{
public:
    Orb();
    void setRadius(qreal radius);
    qreal getRadius();
    void setAcceleration(qreal acc);
    qreal getAcceleration();
    void setMaxVelocity(qreal max);
    qreal getMaxVelocity();
protected:
    void verifyVelocities();
    qreal acceleration;
    qreal maxVelocity;
    qreal radius;
    qreal dirVelocity[4];
    qreal xVel;
    qreal yVel;

};

#endif // ORB_H
