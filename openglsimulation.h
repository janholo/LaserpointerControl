#ifndef OPENGLSIMULATION_H
#define OPENGLSIMULATION_H

#include <QOpenGLWidget>

class OpenGLSimulation : public QOpenGLWidget
{
    Q_OBJECT
public:
    explicit OpenGLSimulation(QWidget *parent = 0);
    ~OpenGLSimulation();

    void initializeGL() Q_DECL_OVERRIDE;
    void resizeGL(int w, int h) Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;

signals:

public slots:

};

#endif // OPENGLSIMULATION_H
