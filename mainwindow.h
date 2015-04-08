#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "laserpointer.h"
#include "observer.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow, public Observer
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void updateObserver(QRectF minMaxAngles, QPointF angles, LaserMode laserMode);

private slots:

    void on_laserStateComboBox_currentIndexChanged(int index);

    void on_xAxisSlider_valueChanged(int value);

    void on_yAxisSlider_valueChanged(int value);


    void on_pushButton_3_clicked();

    void on_pushButton_4_pressed();

    void on_pushButton_4_released();

private:
    Ui::MainWindow *ui;

    Laserpointer laserpointer;

};

#endif // MAINWINDOW_H
