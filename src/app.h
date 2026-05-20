#pragma once

#include <SDL3/SDL.h>
#include <vector>
#include "pendulum.h"

constexpr int SCREEN_WIDTH  = 800;
constexpr int SCREEN_HEIGHT = 600;
constexpr int MAX_TRAIL_POINTS = 40000;

struct AppContext {
    SDL_Window*   window   = nullptr;
    SDL_Renderer* renderer = nullptr;
    std::vector<Pendulum> pendulums;
    bool   running     = true;
    Uint64 lastTime    = 0;
    double accumulator = 0.0;
    float  cx = SCREEN_WIDTH  / 2.0f;
    float  cy = SCREEN_HEIGHT / 2.5f;
};
