#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QBasicTimer>
#include "laserpointer.h"
#include "observer.h"
#include "uartinterface.h"

enum Direction
{
    NONE,
    LEFT,
    RIGHT,
    UP,
    DOWN,
};

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

protected:
    void timerEvent(QTimerEvent *e) Q_DECL_OVERRIDE;

private slots:

    void on_laserStateComboBox_currentIndexChanged(int index);

    void on_xAxisSlider_valueChanged(int value);

    void on_yAxisSlider_valueChanged(int value);


    void on_pushButton_3_clicked();

    void on_pushButton_4_pressed();

    void on_pushButton_4_released();

    void on_pushButton_pressed();

    void on_pushButton_released();

    void on_pushButton_5_pressed();

    void on_pushButton_5_released();

    void on_pushButton_2_pressed();

    void on_pushButton_2_released();

private:
    Ui::MainWindow *ui;

    QBasicTimer timer;
    Direction direction;
    int counter;

    bool disableSliderUpdate;

    Laserpointer laserpointer;
    UARTInterface uartInterface;

};

#endif // MAINWINDOW_H
