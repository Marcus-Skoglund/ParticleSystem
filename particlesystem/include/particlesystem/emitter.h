#include <rendering/window.h>

class Emitter {
public:
    glm::vec2 velocity = {0, 0};
    double time;
    double delay;

    bool tick(double dt);

    virtual void addParticle(std::vector<Particle>& particles, float dt) = 0;
};

class Directional : public Emitter {

public:
    Directional(glm::vec2 position, float angle) : Emitter(), position(position), angle(angle) {}

    void addParticle(std::vector<Particle>& particles, float dt) override;
    glm::vec2 position; // position of the emitter
    float angle;       // direction of the emitter (rad) 
};

class Uniform : public Emitter {

public:
    Uniform(glm::vec2 position, int amount) : Emitter(), position(position), amount(amount) {}
    void addParticle(std::vector<Particle>& particles, float dt) override;
    glm::vec2 position;  // position of the emitter
    int amount;         // particle amount
    //~Uniform();
};

class SpinningUniform : public Emitter {

public:
    SpinningUniform(glm::vec2 position, int amount, float speed)
        : Emitter(), position(position), amount(amount), speed(speed) {}
    void addParticle(std::vector<Particle>& particles, float dt) override;
    glm::vec2 position;  // position of the emitter
    int amount;          // particle amount
    float speed;        // Spinning speed
};

class Explosion : public Emitter {

public:
    Explosion(glm::vec2 position, int amount): Emitter(), position(position), amount(amount), hasExploded(false) {}
    void addParticle(std::vector<Particle>& particles, float dt) override;
    glm::vec2 position;  // position of the emitter
    int amount;         // particle amount
    bool hasExploded;  // Flag to track if the explosion has occurred
};

