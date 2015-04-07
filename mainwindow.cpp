#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    ui->laserStateComboBox->addItem("LASER_OFF");
    ui->laserStateComboBox->addItem("LASER_ON");


    laserpointer.registerObserver(this);
    laserpointer.registerObserver(ui->openGLWidget);
    laserpointer.notifyObservers();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::updateObserver(QRectF minMaxAngles, QPointF angles, LaserMode laserMode)
{
    //Update the UI Elements
    ui->xAxisSlider->setMinimum(minMaxAngles.left());
    ui->xAxisSlider->setMaximum(minMaxAngles.right());
    ui->xAxisSlider->setSliderPosition(angles.x());
    ui->xAxisLabel->setText(QString::number(angles.x()) + " °");


    ui->yAxisSlider->setMinimum(minMaxAngles.top());
    ui->yAxisSlider->setMaximum(minMaxAngles.bottom());
    ui->yAxisSlider->setSliderPosition(angles.y());
    ui->yAxisLabel->setText(QString::number(angles.y()) + " °");

    ui->laserStateComboBox->setCurrentIndex(laserMode);
}

void MainWindow::on_laserStateComboBox_currentIndexChanged(int index)
{
    laserpointer.setLaserMode(static_cast<LaserMode>(index));
}

void MainWindow::on_xAxisSlider_valueChanged(int value)
{
    laserpointer.setAngleX(value);
}

void MainWindow::on_yAxisSlider_valueChanged(int value)
{
    laserpointer.setAngleY(value);
}
