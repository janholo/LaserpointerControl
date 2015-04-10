#ifndef UARTINTERFACE_H
#define UARTINTERFACE_H

#include "observer.h"
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include <QMessageBox>

class UARTInterface : public Observer
{
public:
    UARTInterface();
    ~UARTInterface();

    void updateObserver(QRectF minMaxAngles, QPointF angles, LaserMode laserMode, std::vector<QPointF> calibrationAngles);

    bool isConnected(){return connected;}
private:
    QSerialPort serialPort;

    bool connected;

};

#endif // UARTINTERFACE_H
