#include <engine.h>

#include <input_handler.h>
#include <game.h>
#include <render.h>

namespace engine {

    void run(render::WindowSettings window_settings) {
        GLFWwindow* window = render::init(window_settings);
        if (!window) {
            return;
        }
        render::State render_state {};

        // engine::State state {}

        const double delta_time = 1.0 / 60.0;
        double previous_timestamp = glfwGetTime();
        double accumulator {};

        while(true) {
            float current_timestamp = glfwGetTime();
            float elapsed_time = current_timestamp - previous_timestamp;
            previous_timestamp = current_timestamp;
            
            accumulator += elapsed_time;
            
            process_input();
            input::State input = input::process();

            game::State state {};
            while(accumulator >= delta_time) {
                state = game::update(delta_time, input);
                accumulator -= delta_time;
            }

            render::render(window, render_state, accumulator / delta_time, state);
            glfwPollEvents();
        }
    }
}