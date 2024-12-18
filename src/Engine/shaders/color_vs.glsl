#version 430

layout(location=0) in vec4 a_vertex_position;
layout(location=1) in vec2 a_vertex_texcoords;

layout(std140, binding=1) uniform Transformations {
    mat4 PVM;
};

out vec2 vertex_texcoords;

void main() {
    vertex_texcoords = a_vertex_texcoords;
    gl_Position =  PVM * a_vertex_position;
}
