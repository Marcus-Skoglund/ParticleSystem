#pragma once

#include <glm/vec2.hpp>
#include <glm/vec4.hpp>

#include <vector>
#include <random>
#include <span>

namespace example {

struct RandomSystem {

    /**
     * @brief Construct a new Random System object with 'numParticles' particles
     * The created system will have random positions between -1.0 and 1.0.
     * Particle sized will be in the range of 1.0 to 10.0. and
     * colors will be random with component values in the range of 0.0 and 1.0, with an
     * alpha value of 0.5.
     *
     * The particles will have a randomly assigned lifetime in the range of 0.5 to 2.5 time units.
     *
     * @param numParticles The number of particles in the system
     */
    RandomSystem(size_t numParticles);

    /**
     * Update the system state to a new time with a given speed.
     * The positions will be updated using a simple rocking motion.
     * The particle alpha will be decreased as the particle lifetime nears it end.
     * Once a particle reaches it's end of life, it will be respawned into a new particle.
     * @param time new time of the system
     * @param speed scaling factor for the particle velocities
     */
    void update(double time, float speed);

    std::span<const glm::vec2> getPosition() const { return position; }
    std::span<const float> getSize() const { return size; }
    std::span<const glm::vec4> getColor() const { return color; }

private:
    std::vector<glm::vec2> position;
    std::vector<float> size;
    std::vector<glm::vec4> color;
    std::vector<float> lifetime;

    glm::vec2 randPosition() { return {positionDist(gen), positionDist(gen)}; };
    float randSize() { return sizeDist(gen); };
    glm::vec4 randColor() { return {colorDist(gen), colorDist(gen), colorDist(gen), 0.5f}; };
    float randLifetime() { return lifetimeDist(gen); };
    glm::vec2 randJitter() { return {positionDist(gen), positionDist(gen)}; };

    std::mt19937 gen;
    std::uniform_real_distribution<float> positionDist;
    std::uniform_real_distribution<float> sizeDist;
    std::uniform_real_distribution<float> colorDist;
    std::uniform_real_distribution<float> lifetimeDist;
    std::uniform_real_distribution<float> jitterDist;
    
    double prevTime;
};

}  // namespace example
