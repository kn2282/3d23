//
// Created by k7668 on 01.03.2024.
//

#pragma once

#include <glad/gl.h>
#include <glm/glm.hpp>

#include "Application/utils.h"
#include "Engine/Material.h"

namespace xe {

    class ColorMaterial : public Material {
    public:
        ColorMaterial(const glm::vec4 color, GLuint texture, GLuint texture_unit) : color_(color), texture_(texture),
                                                                                    texture_unit_(texture_unit) {}

        ColorMaterial(const glm::vec4 color) : color_(color) {}

        void bind();

        void unbind();

        static void init();

        static GLuint program() { return shader_; }

        GLuint getTexture() const;

        void setTexture(GLuint texture);

        GLuint getTextureUnit() const;

        void setTextureUnit(GLuint textureUnit);

    private:
        static GLuint shader_;
        static GLuint color_uniform_buffer_;

        static GLint uniform_map_Kd_location_;
        GLuint texture_;
        GLuint texture_unit_;

        glm::vec4 color_;

    };
}