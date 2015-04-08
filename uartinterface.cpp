#include "uartinterface.h"

UARTInterface::UARTInterface()
{
    connected = false;

    serialPort.setPortName("COM5");

    serialPort.setBaudRate(QSerialPort::Baud19200);
    serialPort.setDataBits(QSerialPort::Data8);
    serialPort.setParity(QSerialPort::NoParity);
    serialPort.setStopBits(QSerialPort::OneStop);
    serialPort.setFlowControl(QSerialPort::NoFlowControl);

    if(serialPort.open(QSerialPort::ReadWrite))
    {
        connected = true;
    }


}

UARTInterface::~UARTInterface()
{
    serialPort.close();
}

void UARTInterface::updateObserver(QRectF minMaxAngles, QPointF angles, LaserMode laserMode)
{
    if(connected == false)
    {
        return;
    }

    //Invert the angles
    angles = -angles;

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

    int sendCount = serialPort.write(msg, 5);

    if(sendCount != 5)
    {
        QMessageBox messageBox;
        messageBox.critical(0,"Error","Data was not completely send!");
        messageBox.setFixedSize(500,200);
    }

}
