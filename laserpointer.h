#ifndef LASERPOINTER_H
#define LASERPOINTER_H

#include <QtGlobal>
#include <QPointF>
#include <QRectF>

#include "observedsubject.h"

class Laserpointer : public ObservedSubject
{
public:
    Laserpointer();
    ~Laserpointer();

    void notifyObservers();

    QPointF getAngles(){return angles;}
    void setAngles(QPointF angles);
    void setAngleX(qreal x);
    void setAngleY(qreal y);
    void setMinMaxAngles(QRectF minMaxAngles);
    QRectF getMinMaxAngles(){return minMaxAngles;}
    void setLaserMode(LaserMode laserMode);
private:
    QPointF angles;
    QRectF minMaxAngles;
    LaserMode laserMode;

};

#endif // LASERPOINTER_H
