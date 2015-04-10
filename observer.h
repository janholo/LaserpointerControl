#ifndef OBSERVER_H
#define OBSERVER_H

#include <QRectF>
#include <QPointF>
#include <vector>

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

    virtual void updateObserver(QRectF minMaxAngles, QPointF angles, LaserMode laserMode, std::vector<QPointF> calibrationAngles) = 0;
};

#endif // OBSERVER_H
