
attribute highp vec4 vertexAttr;
uniform mediump mat4 projection;
uniform mediump mat4 modelView;
attribute lowp vec4 colorAttr;

varying lowp vec4 color;

void main() {
    gl_Position = projection * modelView * vertexAttr;
    color = colorAttr;
}
