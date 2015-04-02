#ifndef OBSERVER_H
#define OBSERVER_H

#include <QRectF>
#include <QPointF>

enum LaserMode
{
    LASER_OFF,
    LASER_ON
};

class Observer
{
public:
    Observer();
    ~Observer();

    virtual void updateObserver(QRectF minMaxAngles, QPointF angles, LaserMode laserMode) = 0;
};

#endif // OBSERVER_H
