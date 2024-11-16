#version 420

layout(location=0) in  vec4 a_vertex_position;
layout(location=1) in vec4 b_vertex_color;
out vec4 outColor;
layout(std140, binding=1) uniform Transformations {
    vec2 scale;
    vec2 translation;
    mat2 rotation;
};

void main() {
    gl_Position.xy = rotation*(scale*a_vertex_position.xy)+translation;
    gl_Position.zw = a_vertex_position.zw;
    outColor = b_vertex_color;
}
