//
// Created by pbialas on 25.09.2020.
//

#include "app.h"

#include <iostream>
#include <vector>
#include <tuple>

#include "Application/utils.h"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

union Modifier{
    float strength;
    float color[3];
};


void SimpleShapeApplication::init() {
    // A utility function that reads the shader sources, compiles them and creates the program object
    // As everything in OpenGL we reference program by an integer "handle".
    auto program = xe::utils::create_program(
            {{GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
             {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"}});

    if (!program) {
        std::cerr << "Invalid program" << std::endl;
        exit(-1);
    }


    GLfloat strength = 0.5f;
    GLfloat color[3] = {1.0f, 1.0f, 1.0f};


    GLuint interface_block_buffer;
    glGenBuffers(1, &interface_block_buffer);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, interface_block_buffer);

    glBindBuffer(GL_UNIFORM_BUFFER, interface_block_buffer);
    glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_UNIFORM_BUFFER, 0,  sizeof(GLfloat), &strength);
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(GLfloat), 3 * sizeof(GLfloat), &color);


    float theta = 1.0*glm::pi<float>()/6.0f;
    auto cs = std::cos(theta);
    auto ss = std::sin(theta);
    glm::mat2 rot{cs,ss,-ss,cs};
    glm::vec2 trans{0.0,  -0.25};
    glm::vec2 scale{0.5, 0.5};


    GLuint transformation_uniform_handler;
    glGenBuffers(1, &transformation_uniform_handler);
    glBindBufferBase(GL_UNIFORM_BUFFER, 1, transformation_uniform_handler);

    glBufferData(GL_UNIFORM_BUFFER, 32 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW);
    glBufferSubData(GL_UNIFORM_BUFFER, 0 * sizeof(GLfloat), 2 * sizeof(GLfloat), &scale);
    glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(GLfloat), 2 * sizeof(GLfloat), &trans);
    glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(GLfloat), 2 * sizeof(GLfloat), &rot[0]);
    glBufferSubData(GL_UNIFORM_BUFFER, 8 * sizeof(GLfloat), 2 * sizeof(GLfloat), &rot[1]);





    // A vector containing the x,y,z vertex coordinates for the triangle.
    std::vector<GLushort> vectorBuffer = {
            0, 1, 2, 3, 4, 5, 3, 6, 4
    };

    GLuint vec_buffer_unit;
    glGenBuffers(1, &vec_buffer_unit);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vec_buffer_unit);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, vectorBuffer.size() * sizeof(GLushort), vectorBuffer.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    std::vector<GLfloat> vertices = {
            0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
            -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
            -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f};

    // Generating the buffer and loading the vertex data into it.
    GLuint v_buffer_handle;
    glGenBuffers(1, &v_buffer_handle);
    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // This setups a Vertex Array Object (VAO) that  encapsulates
    // the state of all vertex buffers needed for rendering
    glGenVertexArrays(1, &vao_);
    glBindVertexArray(vao_);

    glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vec_buffer_unit);

    // This indicates that the data for attribute 0 should be read from a vertex buffer.
    glEnableVertexAttribArray(0);

    // and this specifies how the data is layout in the buffer.
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(0));

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat)));

//    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBindVertexArray(0);
    //end of vao "recording"


    // Setting the background color of the rendering window,
    // I suggest not to use white or black for better debuging.
    glClearColor(0.81f, 0.81f, 0.8f, 1.0f);

    // This setups an OpenGL vieport of the size of the whole rendering window.
    auto[w, h] = frame_buffer_size();
    glViewport(0, 0, w, h);

    glUseProgram(program);
}

//This functions is called every frame and does the actual rendering.
void SimpleShapeApplication::frame() {
    // Binding the VAO will setup all the required vertex buffers.
    glBindVertexArray(vao_);
    glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_SHORT, nullptr);
    glBindVertexArray(0);
}
