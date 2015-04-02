#include "uartinterface.h"

UARTInterface::UARTInterface()
{

}

UARTInterface::~UARTInterface()
{

}

void UARTInterface::updateObserver(QRectF minMaxAngles, QPointF angles, LaserMode laserMode)
{
    //Send the correct values over the UART Interface
    unsigned int x = 1000 + ((angles.x()-minMaxAngles.left())/(minMaxAngles.width())*1000.0f);

    unsigned int y = 1000 + ((angles.y()-minMaxAngles.top())/(minMaxAngles.height())*1000.0f);


    char msg[6] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    msg[0] = y >> 8;
    msg[1] = y & 0xFF;
    msg[2] = x >> 8;
    msg[3] = x & 0xFF;
    if(laserMode == LASER_ON)
        msg[4] = 0x01;
    else if(laserMode == LASER_OFF)
        msg[4] = 0x02;

    pGame->sendSerialMessage(msg);


}
