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

class OpenGLSimulation : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    explicit OpenGLSimulation(QWidget *parent = 0);
    ~OpenGLSimulation();

    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

private:
    void initShaders();
    void initTextures();

private:
    QBasicTimer timer;

    QOpenGLShaderProgram program;
    QOpenGLTexture *texture;

    QMatrix4x4 projectionMat;

signals:

public slots:

};

#endif // OPENGLSIMULATION_H
