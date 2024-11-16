//
// Created by pbialas on 05.08.2020.
//

#pragma once

#include <vector>

#include "Application/application.h"
#include "Application/utils.h"
#include "glm/gtc/matrix_transform.hpp"

#include <Engine/Mesh.h>
#include "Engine/lights.h"


#include "glad/gl.h"

#include "camera.h"
#include "camera_controler.h"

using namespace xe;

class SimpleShapeApplication : public xe::Application
{
public:
    SimpleShapeApplication(int width, int height, std::string title, bool debug) : Application(width, height, title, debug) {}

    void init() override;

    void frame() override;

    void framebuffer_resize_callback(int w, int h) override;

    void set_camera(Camera *camera) { camera_ = camera; }
    Camera *camera() { return camera_; }
    ~SimpleShapeApplication() {
        if (camera_) {
            delete camera_;
        }
    }

    void set_controler(CameraControler *controler) { controler_ = controler; }

    void scroll_callback(double xoffset, double yoffset) override {
        Application::scroll_callback(xoffset, yoffset);
        camera()->zoom(yoffset / 30.0f);
    }

    void mouse_button_callback(int button, int action, int mods) override {
        Application::mouse_button_callback(button, action, mods);

        if (controler_) {
            double x, y;
            glfwGetCursorPos(window_, &x, &y);

            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
                controler_->LMB_pressed(x, y);

            if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
                controler_->LMB_released(x, y);
        }

    }

    void cursor_position_callback(double x, double y) override {
        Application::cursor_position_callback(x, y);
        if (controler_) {
            controler_->mouse_moved(x, y);
        }
    }

    void add_submesh(Mesh *mesh) {
        meshes_.push_back(mesh);
    }

    void add_light(const PointLight &p_light) {
        p_lights_.push_back(p_light);
    }

    void add_ambient(glm::vec3 ambient) {
        ambient_ = ambient;
    }

private:
    GLuint vao_;

    GLuint u_pvm_buffer_;

    Camera *camera_;

    CameraControler *controler_;

    std::vector<Mesh*> meshes_;

    glm::vec3 ambient_;
    std::vector<PointLight> p_lights_;

    GLuint Matrix_bufor;
    GLuint Lights_bufor;

};