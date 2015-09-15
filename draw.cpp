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

    //matrix.scale(0.5f, 0.6f);
    //matrix.translate(1, 1, 1);
    //matrix.rotate(10.f,0, 0, 1);
    //matrix.translate(-1, -1, -1);

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

    float line_long = dist(points);
    float alpha = angle(points);
    std::vector<float> local_vertices;
    local_vertices.resize(18);

    //we prepare affine transformation matrix  RT
    QMatrix4x4 matrix;
    matrix.setToIdentity();
    matrix.translate(points[0], points[1], points[2]);
    matrix.rotate(alpha * 180 / M_PI, 0, 0, 1);

    transformations.push_back(matrix);

    //creating points for the first triangle
    local_vertices[0] = 0;
    local_vertices[1] = -width / 2.0f;
    local_vertices[2] = 0.0f;

    local_vertices[3] = line_long ;
    local_vertices[4] = -width / 2.0f;
    local_vertices[5] = 0.0f;

    local_vertices[6] = 0.0f;
    local_vertices[7] = width / 2.0f;
    local_vertices[8] = 0.0f;

    //creating points for the second triangle
    local_vertices[9] = line_long;
    local_vertices[10] = width / 2.0f;
    local_vertices[11] = 0.0f;

    local_vertices[12] = line_long;
    local_vertices[13] = -width / 2.0f;
    local_vertices[14] = 0.0f;

    local_vertices[15] = 0.0f;
    local_vertices[16] = width / 2.0f;
    local_vertices[17] = 0.0f;

    vertices.push_back(local_vertices);

    //prepare colors array
    std::vector<float> allcolor;
    allcolor.resize(18);
    unsigned int i = 0;
    while (i < local_vertices.size()){
        allcolor[i + 0] = color[0];
        allcolor[i + 1] = color[1];
        allcolor[i + 2] = color[2];

        i += 3;
    }
    colors.push_back(allcolor);

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

