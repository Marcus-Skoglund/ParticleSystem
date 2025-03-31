#include <rendering/window.h>
#include <particlesystem/particle.h>

//#include <tracy/Tracy.hpp>
#pragma once
class Effect {
public:
      virtual void move(std::vector<Particle>& particles) = 0;
};

class GravityWell : public Effect {

public:
    GravityWell(glm::vec2 position, int gravity, float range)
    : Effect(), position(position), gravity(gravity), range(range) {}

    void move(std::vector<Particle>& particles) override;
    glm::vec2 position;   // position of the effect
    int gravity;         // power of the gravity
    float range;        // range of the gravity
};

class Wind : public Effect {
public:
    Wind(glm::vec2 position, float angle, float power) : Effect(), position(position), angle(angle), power(power) {}

    void move(std::vector<Particle>& particles) override;
    glm::vec2 position;  // position of the effect
    float angle;        // direction of the effect (rad)
    float power;       // power of the wind
};
