#version 420


layout(location=0) out vec4 vFragColor;
in vec4 outColor;
layout(std140, binding = 0) uniform Modifier {
    float strength;
    vec3  color;
} ;


void main() {
    vFragColor.a = outColor.a;
    vFragColor.rgb = outColor.rgb*color*strength;
}
