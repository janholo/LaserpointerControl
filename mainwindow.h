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


    void on_xAxisSlider_sliderMoved(int position);

    void on_yAxisSlider_sliderMoved(int position);

    void on_laserStateComboBox_currentIndexChanged(int index);

private:
    Ui::MainWindow *ui;

    Laserpointer laserpointer;

};

#endif // MAINWINDOW_H
