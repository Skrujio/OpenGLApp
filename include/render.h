#ifndef RENDER_H
#define RENDER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <game.h>

namespace render {
    struct WindowSettings {
        int width;
        int height;
        const char* name;
    };

    struct State {
        glm::vec4 clear_color {1.0f, 1.0f, 1.0f, 1.0f};
        glm::uint32 clear_flags {};
    };

    bool init_glfw();
    bool init_glad();

    GLFWwindow* create_window(WindowSettings settings);
    GLFWwindow* init(WindowSettings settings);

    void render(GLFWwindow* window, render::State render_state, double render_interpolation, game::State game_state);
}
#endif