#ifndef UARTINTERFACE_H
#define UARTINTERFACE_H

#include "observer.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class UARTInterface : public Observer
{
public:
    UARTInterface();
    ~UARTInterface();

    void updateObserver(QRectF minMaxAngles, QPointF angles, LaserMode laserMode);

private:
    QSerialPort serialPort;

};

#endif // UARTINTERFACE_H
