//
// Created by k7668 on 01.03.2024.
//

#pragma once

#include "glm/glm.hpp"

struct PointLight {
    PointLight() = default;

    PointLight(const glm::vec3 &pos, const glm::vec3 &color, float intensity, float radius)
            : position_in_ws(pos),
              color(color), atn(intensity, radius, 0.0f) {}

    alignas(16) glm::vec3 position_in_ws;
    alignas(16) glm::vec3 position_in_vs;
    alignas(16) glm::vec3 color;
    alignas(16) glm::vec3 atn;
};
