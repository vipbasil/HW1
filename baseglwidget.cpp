#include "baseglwidget.h"

BaseGLWidget::BaseGLWidget(QWidget *parent):QOpenGLWidget(parent){

}

void BaseGLWidget::initializeGL(){

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

    //shaders link
    QOpenGLShader vShader(QOpenGLShader::Vertex);
    vShader.compileSourceFile(":/files/Shaders/vShader.glsl");


    QOpenGLShader fShader(QOpenGLShader::Fragment);
    fShader.compileSourceFile(":/files/Shaders/fShader.glsl");

    shader_program.addShader(&vShader);
    shader_program.addShader(&fShader);

    if(!shader_program.link()){
        qWarning("Can't link the shader program");
    }

    vertexAttr = shader_program.attributeLocation("vertexAttr");
    colorAttr = shader_program.attributeLocation("colorAttr");
    matrixUniform = shader_program.uniformLocation("projection");
    modelViewMatrix = shader_program.uniformLocation("modelView");

    pen = new Draw(&shader_program, vertexAttr, colorAttr, modelViewMatrix);


}

void BaseGLWidget::paintGL(){


    if(!shader_program.bind())
        return;

    //translate the camera away
    QMatrix4x4 matrix;
    matrix.ortho( -2.0f, 2.0f, -2.0f, 2.0f, 2.0f, -2.0f);

    shader_program.setUniformValue(matrixUniform, matrix);

    //draw a triangle as example

    std::vector<float> vertices;
    std::vector<float> onecolor;
    std::vector<float> allcolors;

    vertices.resize(9);
    onecolor.resize(3);
    allcolors.resize(9);

    vertices[0] = 0.0f;
    vertices[1] = 1.0f;
    vertices[2] = 0.0f;

    vertices[3] = 1.0f;
    vertices[4] = 0.0f;
    vertices[5] = 0.0f;

    vertices[6] = -1.0f;
    vertices[7] = 0.0f;
    vertices[8] = 0.0f;

    allcolors[0] = 1.0f;
    allcolors[1] = 0.0f;
    allcolors[2] = 0.0f;

    allcolors[3] = 0.0f;
    allcolors[4] = 1.0f;
    allcolors[5] = 0.0f;

    allcolors[6] = 0.0f;
    allcolors[7] = 0.0f;
    allcolors[8] = 1.0f;

    onecolor[0] = 1.0f;
    onecolor[1] = 1.0f;
    onecolor[2] = 0.5f;

    pen->addCircle(vertices, onecolor, 1.0f, 50);
    pen->addBasicTriangle(vertices, allcolors);

    onecolor[0] = 0.5f;
    onecolor[1] = 1.0f;
    onecolor[2] = 0.5f;

    vertices[0] = -1.f;
    vertices[1] = -1.f;
    vertices[2] = 0.0f;

    vertices[3] = 1.0f;
    vertices[4] = 1.0f;
    vertices[5] = 0.0f;

    pen->addLine(vertices, onecolor, 0.5f);

    vertices[0] = -1.f;
    vertices[1] = 1.f;
    vertices[2] = 0.0f;

    vertices[3] = 1.0f;
    vertices[4] = -1.0f;
    vertices[5] = 0.0f;

    pen->addLine(vertices, onecolor, 0.5f);



    pen->Paint();
    //release the  binded shader program
    shader_program.release();

}

void BaseGLWidget::resizeGL(int width, int height){
    glViewport(0, 0, width, height);
    //translate the camera away
    QMatrix4x4 matrix;
    matrix.ortho( -2.0f, 2.0f, -2.0f, 2.0f, 2.0f, -2.0f);
    matrix.translate(0.0f, 0.0f, 1.0f);
    matrix.scale(2.0f / width, 2.0f / height, 0);
    //this->geometry.width();

    shader_program.setUniformValue(matrixUniform, matrix);
    pen->setHeight(height);
    pen->setWidth(width);

}

