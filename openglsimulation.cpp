#include "openglsimulation.h"

OpenGLSimulation::OpenGLSimulation(QWidget *parent)
    :QOpenGLWidget(parent)
{
    rotationAngle = 0.0;
    nickAngle = 0.0;

    servoSize = QVector3D(2.0, 3.7, 4.0);
    servoSpindle = QVector3D(1.0, 3.7, -3.0);
    servoSpindleSize = QVector3D(0.5, 1, 0.5);

    servo2Contact = QVector3D(0.0,-2.5,3.25);
    laserSize = QVector3D(1.25,1.25,4.0);

    cameraDistance = 15.0;

    actualAngle = QPointF(0,0);
    actualAngleSpeed = QPointF(180,180);
    targetAngle = QPointF(0,0);


    laserMode = LASER_OFF;
}

OpenGLSimulation::~OpenGLSimulation()
{
    // Make sure the context is current when deleting the texture
    // and the buffers.
    makeCurrent();
    delete servoTexture;
    delete grassTexture;
    delete laserTexture;
    delete cubeGeometry;
    delete planeGeometry;
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

    glEnable(GL_POINT_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);

    cubeGeometry = new GeometryEngine(CUBE);
    planeGeometry = new GeometryEngine(PLANE);

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
    servoTexture = new QOpenGLTexture(QImage(":images/images/servo.png"));

    // Set nearest filtering mode for texture minification
    servoTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);

    // Set bilinear filtering mode for texture magnification
    servoTexture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);

    // Wrap texture coordinates by repeating
    // f.ex. texture coordinate (1.1, 1.2) is same as (0.1, 0.2)
    servoTexture->setWrapMode(QOpenGLTexture::Repeat);


    grassTexture = new QOpenGLTexture(QImage(":images/images/grass.png"));
    grassTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    grassTexture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
    grassTexture->setWrapMode(QOpenGLTexture::Repeat);

    laserTexture = new QOpenGLTexture(QImage(":images/images/laser.png"));
    laserTexture->setMinificationFilter(QOpenGLTexture::LinearMipMapLinear);
    laserTexture->setMagnificationFilter(QOpenGLTexture::LinearMipMapLinear);
    laserTexture->setWrapMode(QOpenGLTexture::Repeat);
}

void OpenGLSimulation::resizeGL(int w, int h)
{
    // Calculate aspect ratio
    qreal aspect = qreal(w) / qreal(h ? h : 1);

    // Set near plane to 3.0, far plane to 7.0, field of view 45 degrees
    const qreal zNear = 1.0, zFar = 1000.0, fov = 45.0;

    // Reset projection
    projectionMat.setToIdentity();

    // Set perspective projection
    projectionMat.perspective(fov, aspect, zNear, zFar);
}

void OpenGLSimulation::recalcCameraMat()
{
    cameraMat.setToIdentity();
    cameraMat.translate(0,0,-cameraDistance);
    cameraMat.rotate(QQuaternion::fromAxisAndAngle(QVector3D(1,0,0),nickAngle));
    cameraMat.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,1,0),rotationAngle));
    cameraMat.translate(0,-5,2.5);
}

void OpenGLSimulation::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    QMatrix4x4 matrix;

    grassTexture->bind();

    //Draw grass at the ground
    matrix.setToIdentity();
    matrix.scale(1000.0, 1.0, 1000.0);

    paintPlane(matrix);

    servoTexture->bind();

    // Base Servo
    matrix.setToIdentity();
    matrix.translate(servoSize.x()/2.0, servoSize.y()/2.0, -servoSize.z()/2);
    matrix.scale(servoSize);

    paintCube(matrix);

    // Spindle of Base Servo
    matrix.setToIdentity();
    matrix.translate(0.0, servoSpindleSize.y()/2.0, 0.0);
    matrix.translate(servoSpindle);
    matrix.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,-1,0), actualAngle.x()));
    matrix.scale(servoSpindleSize);

    paintCube(matrix);

    // Second Servo
    matrix.setToIdentity();
    matrix.translate(0.0, servoSpindleSize.y(), 0.0);
    matrix.translate(servoSpindle);

    matrix.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,-1,0), actualAngle.x()));
    matrix.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,0,-1), -90));

    matrix.translate(servo2Contact);
    matrix.translate(servoSize.x()/2.0, servoSize.y()/2.0, -servoSize.z()/2);
    matrix.scale(servoSize);

    paintCube(matrix);

    // Second Servo Spindle
    matrix.setToIdentity();

    matrix.translate(0.0, servoSpindleSize.y(), 0.0);
    matrix.translate(servoSpindle);

    matrix.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,-1,0), actualAngle.x()));
    matrix.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,0,-1), -90));


    matrix.translate(servo2Contact);


    matrix.translate(0.0, servoSpindleSize.y()/2.0, 0.0);
    matrix.translate(servoSpindle);
    matrix.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,-1,0), actualAngle.y()));
    matrix.scale(servoSpindleSize);

    paintCube(matrix);

    // Laser
    matrix.setToIdentity();

    matrix.translate(0.0, servoSpindleSize.y(), 0.0);
    matrix.translate(servoSpindle);

    matrix.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,-1,0), actualAngle.x()));
    matrix.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,0,-1), -90));


    matrix.translate(servo2Contact);

    matrix.translate(0.0, laserSize.y()/2.0, 0.0);
    matrix.translate(0.0, servoSpindleSize.y(), 0.0);
    matrix.translate(servoSpindle);

    matrix.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,-1,0), actualAngle.y()));
    matrix.scale(laserSize);

    paintCube(matrix);

    if(laserMode == LASER_ON)
    {
        laserTexture->bind();

        // Red Laser Beam
        matrix.setToIdentity();

        matrix.translate(0.0, servoSpindleSize.y(), 0.0);
        matrix.translate(servoSpindle);

        matrix.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,-1,0), actualAngle.x()));
        matrix.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,0,-1), -90));


        matrix.translate(servo2Contact);

        matrix.translate(0.0, laserSize.y()/2.0, 0.0);
        matrix.translate(0.0, servoSpindleSize.y(), 0.0);
        matrix.translate(servoSpindle);

        matrix.rotate(QQuaternion::fromAxisAndAngle(QVector3D(0,-1,0), actualAngle.y()));
        matrix.translate(0,0,-500);
        matrix.scale(0.1,0.1,1000);

        paintCube(matrix);

        servoTexture->bind();
    }
}

void OpenGLSimulation::paintCube(QMatrix4x4 worldMat)
{
    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projectionMat * cameraMat * worldMat);

    // Use texture unit 0 which contains cube.png
    program.setUniformValue("texture", 0);

    // Draw cube geometry
    cubeGeometry->drawGeometry(&program);
}

void OpenGLSimulation::paintPlane(QMatrix4x4 worldMat)
{
    // Set modelview-projection matrix
    program.setUniformValue("mvp_matrix", projectionMat * cameraMat * worldMat);

    // Use texture unit 0 which contains cube.png
    program.setUniformValue("texture", 0);

    // Draw cube geometry
    planeGeometry->drawGeometry(&program);
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

void OpenGLSimulation::wheelEvent(QWheelEvent *e)
{

    cameraDistance -= e->angleDelta().y()/100;

    if(cameraDistance < 2.0)
        cameraDistance = 2.0;

    recalcCameraMat();


}

void OpenGLSimulation::timerEvent(QTimerEvent *)
{
    //Recalculate the Position of the Servo
    if(targetAngle.x() < actualAngle.x())
    {
        actualAngle.setX(actualAngle.x() - actualAngleSpeed.x()*12/1000);
        actualAngle.setX(qMax(actualAngle.x(), targetAngle.x()));
    }
    else if(targetAngle.x() > actualAngle.x())
    {
        actualAngle.setX(actualAngle.x() + actualAngleSpeed.x()*12/1000);
        actualAngle.setX(qMin(actualAngle.x(), targetAngle.x()));
    }

    if(targetAngle.y() < actualAngle.y())
    {
        actualAngle.setY(actualAngle.y() - actualAngleSpeed.y()*12/1000);
        actualAngle.setY(qMax(actualAngle.y(), targetAngle.y()));
    }
    else if(targetAngle.y() > actualAngle.y())
    {
        actualAngle.setY(actualAngle.y() + actualAngleSpeed.y()*12/1000);
        actualAngle.setY(qMin(actualAngle.y(), targetAngle.y()));
    }

    // Request an update
    update();
}

void OpenGLSimulation::updateObserver(QRectF minMaxAngles, QPointF angles, LaserMode laserMode)
{
    targetAngle = angles;
    this->laserMode = laserMode;
}
