#ifndef UARTINTERFACE_H
#define UARTINTERFACE_H

#include "observer.h"

class UARTInterface : public Observer
{
public:
    UARTInterface();
    ~UARTInterface();

    void updateObserver(QRectF minMaxAngles, QPointF angles, LaserMode laserMode);


};

#endif // UARTINTERFACE_H
