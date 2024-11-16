#version 410

layout(location=0) in  vec4 a_vertex_position;
layout(location=1) in vec4 b_vertex_color;
out vec4 outColor;

void main() {
    gl_Position = a_vertex_position;
    outColor = b_vertex_color;
}
