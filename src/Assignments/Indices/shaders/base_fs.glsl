#version 410

layout(location=0) out vec4 vFragColor;
in vec4 outColor;

void main() {
    vFragColor = outColor;
}
