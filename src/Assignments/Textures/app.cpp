//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>
#include <Engine/Mesh.h>
#include <Engine/ColorMaterial.h>

#include "Application/utils.h"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
//#include "glm/gtx/string_cast.hpp"

#define STB_IMAGE_IMPLEMENTATION  1

#include "3rdParty/stb/stb_image.h"

using namespace xe;
using namespace std;

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    glViewport(0,0,w,h);
    camera_->set_aspect((float) w / h);
}

GLuint create_texture(const char *filename) {
    GLint width, height, channels;

    stbi_set_flip_vertically_on_load(true);
    auto img = stbi_load(filename, &width, &height, &channels, 0);
    if (!img) {
        cerr << "Could not read image from file " << filename << endl;
        return 0;
    }
    GLenum format;
    if (channels == 3)
        format = GL_RGB;
    else if (channels == 4) {
        format = GL_RGBA;
    }
    GLuint result_texture;
    glGenTextures(1, &result_texture);
    glBindTexture(GL_TEXTURE_2D, result_texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, format, GL_UNSIGNED_BYTE, img);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0u);
    return result_texture;
}


void SimpleShapeApplication::init() {
    ColorMaterial::init();
    // ponieważ nie korzystamy już tutaj z poprzedniego programu zdecydowałem się usunąć jego kompilacje na tym etapie

    set_camera(new Camera);

    set_controler(new CameraControler(camera()));

    glm::mat4 PVM(1.0f);

    int w, h;
    tie(w, h) = frame_buffer_size();
    camera_->perspective(glm::pi<float>()/4.0, (float)w/h, 0.1f, 100.0f);
    camera_->look_at(glm::vec3(0.3f, 0.3f, -2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    PVM = camera_->projection() * camera_->view();


    glGenBuffers(1, &u_pvm_buffer_);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_pvm_buffer_);

    glBufferData(GL_UNIFORM_BUFFER, 32 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM);

    auto texture_file_name = std::string(ROOT_DIR) + "/Models/multicolor.png";
    GLuint texture = create_texture(texture_file_name.c_str());

    Mesh *pyramid = new Mesh;

    // A vector containing the x,y,z vertex coordinates for the triangle.
    vector<GLushort> vectorBuffer = {
            0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17
    };

    pyramid->allocate_index_buffer(vectorBuffer.size() * sizeof(GLushort), GL_STATIC_DRAW);
    pyramid->load_indices(0, vectorBuffer.size() * sizeof(GLushort), vectorBuffer.data());

    vector<GLfloat> vertices = {
            0.5f, 0.5f, 0.0f,   0.5f, 0.809f,
            -0.5f, -0.5f, 0.0f, 0.5f, 0.191f,
            0.5f, -0.5f, 0.0f,  0.809f, 0.5f,

            0.5f, 0.5f, 0.0f,  0.5f, 0.809f,
            -0.5f, 0.5f, 0.0f,  0.191f, 0.5f,
            -0.5f, -0.5f, 0.0f, 0.5f, 0.191f,

            0.5f, 0.5f, 0.0f,   0.5f, 0.809f,
            0.0f, 0.0f, -1.0f,  1.0f, 1.0f,
            0.5f, -0.5f, 0.0f,  0.809f, 0.5f,

            0.0f, 0.0f, -1.0f,  0.0f, 1.0f,
            0.5f, 0.5f, 0.0f,   0.5f, 0.809f,
            -0.5f, 0.5f, 0.0f,  0.191f, 0.5f,

            0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
            -0.5f, 0.5f, 0.0f,  0.191f, 0.5f,
            -0.5f, -0.5f, 0.0f, 0.5f, 0.191f,

            0.0f, 0.0f, -1.0f,  1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.5f, 0.191f,
            0.5f, -0.5f, 0.0f,  0.809f, 0.5f
            };

    pyramid->allocate_vertex_buffer(vertices.size() * sizeof(GLfloat), GL_STATIC_DRAW);
    pyramid->load_vertices(0, vertices.size() * sizeof(GLfloat), vertices.data());
    pyramid->vertex_attrib_pointer(0, 3, GL_FLOAT, 5 * sizeof(GLfloat), 0);
    pyramid->vertex_attrib_pointer(1, 2, GL_FLOAT, 5 * sizeof(GLfloat), 3 * sizeof(GLfloat));

    pyramid->add_submesh(0, 6, new ColorMaterial(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f), texture, 1));
    pyramid->add_submesh(6, 9, new ColorMaterial(glm::vec4(0.0f, 5.0f, 5.0f, 1.0f), texture, 2));
    pyramid->add_submesh(9, 12, new ColorMaterial(glm::vec4(0.0f, 7.0f, 7.0f, 1.0f), texture, 3));
    pyramid->add_submesh(12, 15, new ColorMaterial(glm::vec4(0.0f, 1.0f, 1.0f, 1.0f), texture, 4));
    pyramid->add_submesh(15, 18, new ColorMaterial(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f), texture, 5));
    add_submesh(pyramid);


    // Setting the background color of the rendering window,
    // I suggest not to use white or black for better debuging.
    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);

    // This setups an OpenGL vieport of the size of the whole rendering window.
    auto[_w, _h] = frame_buffer_size();
    glViewport(0, 0, _w, _h);


}

//This functions is called every frame and does the actual rendering.
void SimpleShapeApplication::frame() {
    // Binding the VAO will setup all the required vertex buffers.
    auto PVM = camera_->projection() * camera_->view();
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_pvm_buffer_);
    glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
    glEnable(GL_DEPTH_TEST);
//    glEnable(GL_CULL_FACE); // niestety psuło program

    for (auto m: meshes_)
        m->draw();

}
