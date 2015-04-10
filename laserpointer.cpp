#include "laserpointer.h"

Laserpointer::Laserpointer()
{
    angles = QPointF(0,0);

    minMaxAngles = QRectF(QPointF(-45, -45), QPointF(45,45));

    calibrationAngles.clear();
    calibrationAngles.push_back(QPointF(0,0));
    calibrationAngles.push_back(QPointF(0,0));
    calibrationAngles.push_back(QPointF(0,0));
    calibrationAngles.push_back(QPointF(0,0));

    laserMode = LASER_OFF;
}

Laserpointer::~Laserpointer()
{

}

void Laserpointer::setAngles(QPointF angles)
{
    setAngleX(angles.x());
    setAngleY(angles.y());
}

void Laserpointer::setAngleX(qreal x)
{
    if(this->angles.x() != x)
    {
        angles.setX(qMax(minMaxAngles.left(),qMin(minMaxAngles.right(),x)));
        notifyObservers();
    }
}

void Laserpointer::setAngleY(qreal y)
{
    if(this->angles.y() != y)
    {
        angles.setY(qMax(minMaxAngles.top(),qMin(minMaxAngles.bottom(),y)));
        notifyObservers();
    }

}

void Laserpointer::setMinMaxAngles(QRectF minMaxAngles)
{
    if(this->minMaxAngles != minMaxAngles)
    {
        this->minMaxAngles = minMaxAngles;
        notifyObservers();
    }
}

void Laserpointer::setLaserMode(LaserMode laserMode)
{
    if(laserMode != this->laserMode)
    {
        this->laserMode = laserMode;
        notifyObservers();
    }
}

void Laserpointer::notifyObservers()
{
    for(Observer *o : observerList)
    {
        o->updateObserver(minMaxAngles, angles, laserMode, calibrationAngles);
    }
}

void Laserpointer::setCalibrationAngles(QPointF *calibrationAngles)
{
    this->calibrationAngles.clear();
    std::copy(calibrationAngles, calibrationAngles+4, std::back_inserter(this->calibrationAngles));
    notifyObservers();


}

