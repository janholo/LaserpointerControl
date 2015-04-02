#include "openglsimulation.h"

OpenGLSimulation::OpenGLSimulation(QWidget *parent)
    :QOpenGLWidget(parent)
{
    rotationAngle = 0.0;
    nickAngle = 0.0;

    servoSize = QVector3D(1.0, 2.0, 3.0);
    servoSpindle = QVector3D(0.5, 2.0, -2.0);
    servoSpindleSize = QVector3D(0.5, 0.5, 0.5);

    firstLaserAngle = 0.0;
    secondLaserAngle = 0.0;
}

OpenGLSimulation::~OpenGLSimulation()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete texture;
    delete geometries;
    doneCurrent();
}

void OpenGLSimulation::initializeGL()
{
    initializeOpenGLFunctions();

    glClearColor(0.4, 0.4, 0.4, 1);

    initShaders();
    initTextures();

    // Enable depth buffer
    glEnable(GL_DEPTH_TEST);

    // Enable back face culling
    glEnable(GL_CULL_FACE);

    geometries = new GeometryEngine;

    recalcCameraMat();

    // Use QBasicTimer because its faster than QTimer
    timer.start(12, this);
}

void OpenGLSimulation::initShaders()
{
    // Compile vertex shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shader/shader/vshader.glsl"))
        close();

    // Compile fragment shader
    if (!program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shader/shader/fshader.glsl"))
        close();

    // Link shader pipeline
    if (!program.link())
        close();

    // Bind shader pipeline for use
    if (!program.bind())
        close();
}

void OpenGLSimulation::initTextures()
{
    // Load cube.png image
    texture = new QOpenGLTexture(QImage(":images/images/servo.png"));

    // Set nearest filtering mode for texture minification
    texture->setMinificationFilter(QOpenGLTexture::Nearest);

    // Set bilinear filtering mode for texture magnification
    texture->setMagnificationFilter(QOpenGLTexture::Linear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    texture->setWrapMode(QOpenGLTexture::Repeat);
}

void OpenGLSimulation::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 3.0, zFar = 100.0, fov = 45.0;

    // Reset projection
    projectionMat.setToIdentity();

    // Set perspective projection
    projectionMat.perspective(fov, aspect, zNear, zFar);
}

void OpenGLSimulation::recalcCameraMat()
{
    cameraMat.setToIdentity();
    cameraMat.translate(0,0,-15);
    cameraMat.rotate(QQuaternion::fromAxisAndAngle(QVector3D(1,0,0),nickAngle));
    cameraMat.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,1,0),rotationAngle));

    firstLaserAngle = nickAngle;
    secondLaserAngle = rotationAngle;
}

void OpenGLSimulation::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    texture->bind();

    //Draw plane at the ground
    QMatrix4x4 matrix;
    matrix.scale(0.1,0.1,0.1);

    paintCube(matrix);

    // Plane
//    matrix.setToIdentity();
//    matrix.translate(0.0, -0.5, 0.0);
//    matrix.scale(20.0, 1.0, 20.0);

//    paintCube(matrix);

    // Base Servo
    matrix.setToIdentity();
    matrix.translate(servoSize.x()/2.0, servoSize.y()/2.0, -servoSize.z()/2);
    matrix.scale(servoSize);

    paintCube(matrix);

    // Spindle of Base Servo
    matrix.setToIdentity();
    matrix.translate(0.0, servoSpindleSize.y()/2.0, 0.0);
    matrix.translate(servoSpindle);
    matrix.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,1,0), firstLaserAngle));
    matrix.scale(servoSpindleSize);

    paintCube(matrix);

    // Second Servo
    matrix.setToIdentity();
    matrix.translate(0.0, servoSpindleSize.y(), 0.0);
    matrix.translate(servoSpindle);
    matrix.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,1,0), firstLaserAngle));
    matrix.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,0,-1), -90));
    matrix.translate(servoSize.x()/2.0, 0.0, 0.0);
    matrix.scale(servoSize);

    paintCube(matrix);

    // Second Servo Spindle
    matrix.setToIdentity();
    matrix.translate(0.0, servoSpindleSize.y(), 0.0);
    matrix.translate(servoSpindle);
    matrix.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,1,0), firstLaserAngle));

    matrix.translate(-servoSize.x()/2.0, 0.0, 0.0);
    matrix.scale(servoSpindleSize);

    paintCube(matrix);

}

void OpenGLSimulation::paintCube(QMatrix4x4 worldMat)
{
    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projectionMat * cameraMat * worldMat);

    // Use texture unit 0 which contains cube.png
    program.setUniformValue("texture", 0);

    // Draw cube geometry
    geometries->drawCubeGeometry(&program);
}

void OpenGLSimulation::mousePressEvent(QMouseEvent *e)
{

}

void OpenGLSimulation::mouseReleaseEvent(QMouseEvent *e)
{

}

void OpenGLSimulation::mouseMoveEvent(QMouseEvent *e)
{
    //Check if Mouse Button is still pressed
    if(e->buttons() & Qt::RightButton)
    {
        double diffX = e->globalPos().x() - lastPos.x();

        rotationAngle += 180.0*diffX/width();

        if(rotationAngle >= 360)
            rotationAngle-= 360;


        double diffY = e->globalPos().y() - lastPos.y();

        nickAngle += 180.0*diffY/width();

        if(nickAngle > 60)
            nickAngle = 60;

        if(nickAngle < -60)
            nickAngle = -60;

        recalcCameraMat();
    }

    lastPos = e->globalPos();
}

void OpenGLSimulation::timerEvent(QTimerEvent *)
{


    // Request an update
    update();
}


