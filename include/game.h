#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <input_handler.h>

namespace game {

    struct State {

    };

    State update(float delta_time, input::State input);
}

#endif