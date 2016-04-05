#ifndef ORB_H
#define ORB_H

#include <QGraphicsPixmapItem>
#include <QObject>
#include <QPixmap>
#include <QString>

class Orb: public QObject, public QGraphicsPixmapItem // QGraphicsEllipseItem inherits QGraphicsItem
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

    void setDirVelocity(int ind, qreal val);
    qreal getDirVelocity(int ind);

    void setXVel(qreal xVel);
    qreal getXVel();

    void setYVel(qreal YVel);
    qreal getYVel();

protected:
    void verifyVelocities();
    QString imageSource;
    qreal acceleration;
    qreal maxVelocity;
    qreal radius;
    qreal dirVelocity[4];
    qreal xVel;
    qreal yVel;

};

#endif // ORB_H
