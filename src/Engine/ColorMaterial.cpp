//
// Created by k7668 on 01.03.2024.
//

#include "ColorMaterial.h"

namespace xe {

    GLuint ColorMaterial::color_uniform_buffer_ = 0u;
    GLuint ColorMaterial::shader_ = 0u;
    GLint  ColorMaterial::uniform_map_Kd_location_ = 0;

    void ColorMaterial::bind() {
        glUseProgram(program());
        int use_map_Kd = 0;
        if (texture_ > 0){
            glUniform1i(uniform_map_Kd_location_, texture_unit_);
            glActiveTexture(GL_TEXTURE0 + texture_unit_);
            glBindTexture(GL_TEXTURE_2D, texture_);
            use_map_Kd = 1;
        }

        glBindBufferBase(GL_UNIFORM_BUFFER, 0, color_uniform_buffer_);
        glBindBuffer(GL_UNIFORM_BUFFER, color_uniform_buffer_);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &color_[0]);
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(GLint), &use_map_Kd);
        glBindBuffer(GL_UNIFORM_BUFFER, 0u);

    }

    void ColorMaterial::unbind() {
        glBindTexture(GL_TEXTURE_2D, 0u);
        glBindBuffer(GL_UNIFORM_BUFFER, 0u);
    }


    void ColorMaterial::init() {


        auto program = xe::utils::create_program(
                {{GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/color_vs.glsl"},
                 {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/color_fs.glsl"}});
        if (!program) {
            std::cerr << "Invalid program" << std::endl;
            exit(-1);
        }

        shader_ = program;
        glUseProgram(shader_);

        glGenBuffers(1, &color_uniform_buffer_);

        glBindBuffer(GL_UNIFORM_BUFFER, color_uniform_buffer_);
        glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), nullptr, GL_STATIC_DRAW);
        glBindBuffer(GL_UNIFORM_BUFFER, 0u);

        uniform_map_Kd_location_ = glGetUniformLocation(shader_, "map_Kd");
        if (uniform_map_Kd_location_ == -1) {
            std::cerr << "Cannot get uniform location: " << "map_Kd" << std::endl;
        }


#if __APPLE__
        auto u_modifiers_index = glGetUniformBlockIndex(program, "Color");
        if (u_modifiers_index == -1) {
            std::cerr << "Cannot find Color uniform block in program" << std::endl;
        } else {
            glUniformBlockBinding(program, u_modifiers_index, 0);
        }
#endif

#if __APPLE__
        auto u_transformations_index = glGetUniformBlockIndex(program, "Transformations");
        if (u_transformations_index == -1) {
            std::cerr << "Cannot find Transformations uniform block in program" << std::endl;
        } else {
            glUniformBlockBinding(program, u_transformations_index, 1);
        }
#endif


    }

    GLuint ColorMaterial::getTexture() const {
        return texture_;
    }

    void ColorMaterial::setTexture(GLuint texture) {
        texture_ = texture;
    }

    GLuint ColorMaterial::getTextureUnit() const {
        return texture_unit_;
    }

    void ColorMaterial::setTextureUnit(GLuint textureUnit) {
        texture_unit_ = textureUnit;
    }
}