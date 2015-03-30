#include "openglsimulation.h"

OpenGLSimulation::OpenGLSimulation(QWidget *parent)
{

}

OpenGLSimulation::~OpenGLSimulation()
{

}

void OpenGLSimulation::initializeGL()
{
    glClearColor(0.4,0.4,0.4,1);
}

void OpenGLSimulation::resizeGL(int w, int h)
{

}

void OpenGLSimulation::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

