#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->laserStateComboBox->addItem("LASER_OFF");
    ui->laserStateComboBox->addItem("LASER_ON");

    direction = NONE;

    calibrationState = OFF;

    disableSliderUpdate = false;


    laserpointer.registerObserver(this);
    laserpointer.registerObserver(ui->openGLWidget);
    laserpointer.registerObserver(&uartInterface);
    laserpointer.notifyObservers();

    //Update Status Bar
    if(uartInterface.isConnected())
    {
        statusBar()->showMessage("uart interface: connected");
    }
    else
    {
        statusBar()->showMessage("uart interface: NOT connected");
    }

    timer.start(10, this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateObserver(QRectF minMaxAngles, QPointF angles, LaserMode laserMode, std::vector<QPointF> calibrationAngles)
{
    disableSliderUpdate = true;

    //Update the UI Elements
    ui->xAxisSlider->setMinimum(minMaxAngles.left());
    ui->xAxisSlider->setMaximum(minMaxAngles.right());
    ui->xAxisSlider->setSliderPosition(angles.x());
    ui->xAxisLabel->setText(QString::number(angles.x(), 'f', 1) + " °");


    ui->yAxisSlider->setMinimum(minMaxAngles.top());
    ui->yAxisSlider->setMaximum(minMaxAngles.bottom());
    ui->yAxisSlider->setSliderPosition(angles.y());
    ui->yAxisLabel->setText(QString::number(angles.y(), 'f', 1) + " °");

    ui->laserStateComboBox->setCurrentIndex(laserMode);

    disableSliderUpdate = false;
}

void MainWindow::on_laserStateComboBox_currentIndexChanged(int index)
{
    laserpointer.setLaserMode(static_cast<LaserMode>(index));
}

void MainWindow::on_xAxisSlider_valueChanged(int value)
{
    if(disableSliderUpdate)
        return;

    laserpointer.setAngleX(value);
}

void MainWindow::on_yAxisSlider_valueChanged(int value)
{
    if(disableSliderUpdate)
        return;

    laserpointer.setAngleY(value);
}


void MainWindow::on_pushButton_3_clicked()
{
    laserpointer.setAngles(QPointF(0,0));
}


void MainWindow::on_pushButton_4_pressed()
{
    laserpointer.setAngleX(laserpointer.getAngles().x() - 0.1);

    //Start Timer
    counter = 0;
    direction = LEFT;
}


void MainWindow::on_pushButton_4_released()
{
    counter = 0;
    direction = NONE;
}

void MainWindow::timerEvent(QTimerEvent *)
{
    counter++;
    if(counter >= 20)
    {
        if(direction == LEFT)
        {
            laserpointer.setAngleX(laserpointer.getAngles().x() - 0.1f);
        }
        else if(direction == RIGHT)
        {
            laserpointer.setAngleX(laserpointer.getAngles().x() + 0.1f);
        }
        else if(direction == UP)
        {
            laserpointer.setAngleY(laserpointer.getAngles().y() + 0.1f);
        }
        else if(direction == DOWN)
        {
            laserpointer.setAngleY(laserpointer.getAngles().y() - 0.1f);
        }

    }
}


void MainWindow::on_pushButton_pressed()
{
    laserpointer.setAngleY(laserpointer.getAngles().y() + 0.1);

    //Start Timer
    counter = 0;
    direction = UP;
}

void MainWindow::on_pushButton_released()
{
    counter = 0;
    direction = NONE;
}

void MainWindow::on_pushButton_5_pressed()
{
    laserpointer.setAngleX(laserpointer.getAngles().x() + 0.1);

    //Start Timer
    counter = 0;
    direction = RIGHT;
}

void MainWindow::on_pushButton_5_released()
{
    counter = 0;
    direction = NONE;
}

void MainWindow::on_pushButton_2_pressed()
{
    laserpointer.setAngleY(laserpointer.getAngles().y() - 0.1);

    //Start Timer
    counter = 0;
    direction = DOWN;
}

void MainWindow::on_pushButton_2_released()
{
    counter = 0;
    direction = NONE;
}

void MainWindow::on_calibrationButton_clicked()
{
    switch(calibrationState)
    {
        case OFF:
        {
            //Start the calibration
            ui->calibrationText->setText("Calibrate TOP LEFT position:");
            ui->calibrationButton->setText("Save position");

            calibrationState = LEFTTOP;
            break;
        }
        case LEFTTOP:
        {
            calibrationBuffer[0] = laserpointer.getAngles();

            ui->calibrationText->setText("Calibrate TOP RIGHT position:");
            ui->calibrationButton->setText("Save position");

            calibrationState = RIGHTTOP;
            break;
        }
        case RIGHTTOP:
        {
            calibrationBuffer[1] = laserpointer.getAngles();

            ui->calibrationText->setText("Calibrate BOTTOM RIGHT position:");
            ui->calibrationButton->setText("Save position");

            calibrationState = RIGHTBOTTOM;
            break;
        }
        case RIGHTBOTTOM:
        {
            calibrationBuffer[2] = laserpointer.getAngles();

            ui->calibrationText->setText("Calibrate BOTTOM LEFT position:");
            ui->calibrationButton->setText("Save position");

            calibrationState = LEFTBOTTOM;
            break;
        }
        case LEFTBOTTOM:
        {
            calibrationBuffer[3] = laserpointer.getAngles();
            laserpointer.setCalibrationAngles(calibrationBuffer);

            ui->calibrationText->setText("Finished calibration:");
            ui->calibrationButton->setText("Start new calibration");

            calibrationState = OFF;
            break;
        }
    }


}
