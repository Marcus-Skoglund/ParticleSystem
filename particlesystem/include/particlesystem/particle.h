#pragma once
#include <rendering/window.h>
#include <cmath>
#include <cstdlib>
#include <vector>
#include <iostream>

class Particle {
public:
    glm::vec2 position;
    glm::vec2 velocity;
    glm::vec4 color;
    float lifetime;
    float radius;

    Particle(glm::vec2 pos = {0, 0}, glm::vec2 vel = {0, 0}, glm::vec4 col = {0, 0, 0, 0},
             float life = 0, float rad = 0)
        : position(pos), velocity(vel), color(col), lifetime(life), radius(rad) {}

    ~Particle() {}
};