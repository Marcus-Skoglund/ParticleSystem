#include <example/randomsystem.h>

namespace example {

RandomSystem::RandomSystem(size_t numParticles)
    : position(numParticles)
    , size(numParticles)
    , color(numParticles)
    , lifetime(numParticles)
    , gen{[]() {
        std::random_device rd;  // Will be used to obtain a seed for the random number engine
        std::mt19937 gen;       // Standard mersenne_twister_engine
        return gen;
    }()}
    , positionDist{-1.0f, 1.0f}  // Position between (-1.0, 1,0) = Screen extent
    , sizeDist{1.0f, 10.0f}      // Radius between (1.0, 10.0)
    , colorDist{0.0f, 1.0f}      // Color between (0.0, 1.0) per channel
    , lifetimeDist{0.5f, 2.5f}   // Lifetime between (0.5, 2.5) seconds
    , jitterDist{-1.0f, 1.0f}    // Jitter between (-1.0, 1.0) seconds
    , prevTime{0.0} {
    std::ranges::generate(position, [&]() { return randPosition(); });
    std::ranges::generate(size, [&]() { return randSize(); });
    std::ranges::generate(color, [&]() { return randColor(); });
    std::ranges::generate(lifetime, [&]() { return randLifetime(); });
}

void RandomSystem::update(double time, float speed) {
    const double dt = time - prevTime;
    prevTime = time;

    // Create some global smooth rocking motion
    const glm::vec2 vel = 0.2f * glm::vec2{static_cast<float>(std::cos(time)),
                                           -static_cast<float>(std::abs(std::sin(time)))};

    // Simulation dt may differ from actual dt based on the simulation speed
    const float simDt = static_cast<float>(dt) * speed;

    for (size_t i = 0; i < position.size(); ++i) {
        // Apply per particle jitter
        position[i] += (vel + randJitter()) * simDt;
        color[i].a = std::min(color[i].a, lifetime[i]);  // Modify alpha based on lifetime
        lifetime[i] -= simDt;

        // Check lifetime and reset if needed
        if (lifetime[i] < 0.0f) {
            position[i] = randPosition();
            color[i] = randColor();
            size[i] = randSize();
            lifetime[i] = randLifetime();
        }
    }
}

}  // namespace example
