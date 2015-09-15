#include "draw.h"

Draw::Draw(QOpenGLShaderProgram *program, int LvertexAttr, int LcolorAttr, int LmodelViewMatrix):
shader_program(program),
vertexAttr(LvertexAttr),
colorAttr(LcolorAttr),
modelViewMatrix(LmodelViewMatrix)
{
    countAttr = 0;
}

void Draw::addTriangle(std::vector<float> points, std::vector<float> color)
{
    ++countAttr;
    std::vector<float> allcolor;
    allcolor.resize(points.size());
    QMatrix4x4 matrix;
    matrix.setToIdentity();

    transformations.push_back(matrix);
    vertices.push_back(points);

    unsigned int i = 0;
    while (i < points.size()){
        allcolor[i + 0] = color[0];
        allcolor[i + 1] = color[1];
        allcolor[i + 2] = color[2];

        i += 3;
    }
    colors.push_back(allcolor);

}
void Draw::addBasicTriangle(std::vector<float> points, std::vector<float> color)
{
    ++countAttr;
    QMatrix4x4 matrix;
    matrix.setToIdentity();
    transformations.push_back(matrix);
    vertices.push_back(points);
    colors.push_back(color);

}

void Draw::addCircle(std::vector<float> point, std::vector<float> color, int radius,unsigned int tessellation)
{
    //the array length = number of triangles(tessellation) times number of vertices in each triangle, times the number of coordinates x,y,z =3
    unsigned int array_length = tessellation * 3 * 3;

    //we prepare affine transformation matrix  RT
    QMatrix4x4 matrix;
    matrix.setToIdentity();
    matrix.translate(point[0], point[1], point[2]);

    transformations.push_back(matrix);

    //prepare colors array
    std::vector<float> allcolor;
    allcolor.resize(array_length);

    unsigned int i = 0;
    while (i < array_length){
        allcolor[i + 0] = color[0];
        allcolor[i + 1] = color[1];
        allcolor[i + 2] = color[2];
        i += 3;
    }
    colors.push_back(allcolor);

    std::vector<float> local_vertices;
    local_vertices.resize(array_length);

    i = 0;
    float ex_alpha = 0.0f;
    while (i <= tessellation){
        float alpha = 2 * M_PI / tessellation * i;
        if (i == 0){
            ex_alpha = alpha;
            ++i;
            continue;
        }
        //creating points for the the triangle
        unsigned int j = (i - 1) * 9;
        local_vertices[j + 0] = 0.0f;
        local_vertices[j + 1] = 0.0f;
        local_vertices[j + 2] = 0.0f;

        local_vertices[j + 3] = radius * cos(alpha);
        local_vertices[j + 4] = radius * sin(alpha);
        local_vertices[j + 5] = 0.0f;

        local_vertices[j + 6] = radius * cos(ex_alpha);
        local_vertices[j + 7] = radius * sin(ex_alpha);
        local_vertices[j + 8] = 0.0f;
        ex_alpha = alpha;
        ++i;
    }
    vertices.push_back(local_vertices);
}

void Draw::addLine(std::vector<float> points, std::vector<float> color, float width){

    // your code here for a line with different width
    // Please use TRS matrix for transformations
    // Points mus have 2 entries 3 elemaents each The begin point and the end point
    //1. Draw a rectangle(2 triangles ) with given width and length
    //2. Rotate the rectangle (matrix of rotation then translate it)
}

void Draw::deleteObject(int index)
{
    vertices.erase(vertices.begin() + index);
}

float Draw::dist(std::vector<float> points)
{
    //this function return the distance between two points
    return sqrt(powl(points[0] - points[3], 2) + powl(points[1] - points[4], 2) + powl(points[2] - points[5], 2));
}

float Draw::angle(std::vector<float> points)
{
    //this function return the angle of a vector between two points
    return atan2(points[4] - points[1], points[3] - points[0]);
}

void Draw::Paint(int index)
{
    if( index == -1)
        for (unsigned  int i = 0; i < vertices.size(); i++)   {

            shader_program->setAttributeArray(vertexAttr, vertices[i].data(), 3);
            shader_program->setAttributeArray(colorAttr, colors[i].data(), 3);
            shader_program->setUniformValue(modelViewMatrix,transformations[i]);

            shader_program->enableAttributeArray(vertexAttr);
            shader_program->enableAttributeArray(colorAttr);

            glDrawArrays(GL_TRIANGLES, 0, vertices[i].size() / 3);

            shader_program->disableAttributeArray(vertexAttr);
            shader_program->disableAttributeArray(colorAttr);

        }


}

void Draw::setHeight(unsigned int h)
{
    Height  = h;
}

void Draw::setWidth(unsigned int w)
{
    Width = w;
}

