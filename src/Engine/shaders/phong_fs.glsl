#version 430

#define MAX_POINT_LIGHTS 24

struct PointLight {
    vec3 position_in_ws;
    vec3 position_in_vs;
    vec3 color;
    vec3 atn;
};

layout(location=0) out vec4 vFragColor;

layout(std140, binding = 0) uniform Material {
    vec4 Kd;
    vec3 Ks;
    vec4 Ka;
    bool use_map_Kd;
    float Ns;
} modifier;

layout(std140, binding=3) uniform Light {
    PointLight light[MAX_POINT_LIGHTS];
    int n_lights;
} p_light;

uniform sampler2D map_Kd;

//uniform vec3 ambient_light;
layout(std140, binding=4) uniform ambient {
   vec3 ambient_light;
};

in vec4 vertex_position;
in vec3 vertex_normal;
in vec2 vertex_texcoords;
in vec3 vertex_coords_in_viewspace;
in vec3 vertex_normal_in_viewspace;

void main() {
    float Ns = modifier.Ns;
    vec3 Ks = modifier.Ks;
    vec4 Kd = modifier.Kd;

    if (modifier.use_map_Kd) {
        Kd *= texture(map_Kd, vertex_texcoords);
    }

    vec3 normal = normalize(vec3(0, 0, 1)); // had to set it manually, there was some issues while receiving "vertex_normal"
    vec3 diffusionLight;
    vec3 specularLight;

    for(int i = 0; i < p_light.n_lights; ++i)
    {
        vec3 incomingLightDirection = normalize(p_light.light[i].position_in_ws - vertex_position.xyz);

        // Specular light
        vec3 reflectDirection = reflect(-incomingLightDirection, normal);
        vec3 viewDirection = -normalize(vertex_coords_in_viewspace);
        float specular = pow(max(dot(viewDirection, reflectDirection), 0.0f), Ns);
        specularLight += specular * Ks;

        // Diffuse light
        float diffusion = max(dot(normal, incomingLightDirection), 0.0f);
        diffusionLight += diffusion * p_light.light[i].color;


    }

    vFragColor.a = Kd.a;
    vFragColor.rgb = Kd.rgb * (diffusionLight + ambient_light + specularLight);
}