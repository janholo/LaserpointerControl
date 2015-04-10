#ifndef LASERPOINTER_H
#define LASERPOINTER_H

#include <algorithm>
#include <vector>
#include <iterator>

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
    LaserMode getLaserMode(){return laserMode;}

    void setCalibrationAngles(QPointF *calibrationAngles);
private:
    QPointF angles;
    QRectF minMaxAngles;
    LaserMode laserMode;
    std::vector<QPointF> calibrationAngles;  //Topleft, topright, bottomright, bottomleft

};

#endif // LASERPOINTER_H
