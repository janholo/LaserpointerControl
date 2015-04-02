#ifndef OPENGLSIMULATION_H
#define OPENGLSIMULATION_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QMatrix4x4>
#include <QQuaternion>
#include <QVector2D>
#include <QBasicTimer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
#include <QMouseEvent>

#include "geometryengine.h"

class OpenGLSimulation : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLSimulation(QWidget *parent = 0);
    ~OpenGLSimulation();

    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

protected:
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void timerEvent(QTimerEvent *e) Q_DECL_OVERRIDE;

private:
    void initShaders();
    void initTextures();

    void recalcCameraMat();
    void paintCube(QMatrix4x4 worldMat);

private:
    QBasicTimer timer;

    QOpenGLShaderProgram program;
    QOpenGLTexture *texture;

    GeometryEngine *geometries;

    QMatrix4x4 projectionMat;

    double rotationAngle;
    double nickAngle;

    QPoint lastPos;

    QMatrix4x4 cameraMat;

    QVector3D servoSize;
    QVector3D servoSpindle;
    QVector3D servoSpindleSize;
    QVector3D servo2Contact;
    QVector3D laserSize;

    double firstLaserAngle;
    double secondLaserAngle;

signals:

public slots:

};

#endif // OPENGLSIMULATION_H
