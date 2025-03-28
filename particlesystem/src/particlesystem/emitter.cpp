#include <particlesystem/particle.h>
#include <particlesystem/emitter.h>
#include <rendering/window.h>

bool Emitter::tick(double dt) {
    time += dt;
    if (time > delay) {
        time = 0;
        return true;
    }
    return false;
}

void Directional::addParticle(std::vector<Particle>& particles, float dt) {
    delay = 0.25;

    Particle particle;
    particle.position = position;
    particle.lifetime = 6;

    // emitter
    Particle Emit;
    Emit.position = position;
    Emit.radius = 20;
    Emit.color = {1.0f, 0.0f, 0.0f, 1.0f};
    Emit.velocity = {0.0, 0.0};
    particles.push_back(Emit);


    if (tick(dt)) {

        particle.color = {0.4, 0.9, 0.4, 1};

        particle.velocity.y = float(sin(angle));
        particle.velocity.x = float(cos(angle));

        particle.radius = float(10);

        particles.push_back(particle);
    }
}

void Uniform::addParticle(std::vector<Particle>& particles, float dt) {
    delay = 0.5;
    const int numParticles = amount;
    const float angleStep = 2 * 3.14 / numParticles;
    Particle particle;
    particle.position = position;
    particle.lifetime = 5;
    particle.color = {0.9, 0.1, 1, 1};

    // emitter
    Particle Emit;
    Emit.position = position;
    Emit.radius = 20;
    Emit.color = {0.0f, 0.0f, 1.0f, 1.0f};
    Emit.velocity = {0.0, 0.0};
    particles.push_back(Emit);

    if (tick(dt)) {
        for (int i = 0; i < numParticles; i++) {
            particle.velocity.y = sin(angleStep * i);
            particle.velocity.x = cos(angleStep * i);
            particle.radius = float(10);
            particles.push_back(particle);
        }
    }
}

void Explosion::addParticle(std::vector<Particle>& particles, float dt) {

    if (hasExploded) {
        return;
    }

    Particle particle;
    particle.position = position;
    particle.lifetime = 5;

    for (int i = 0; i < amount; i++) {
        particle.color = {rand() / (float)(RAND_MAX), rand() / (float)(RAND_MAX),
                          rand() / (float)(RAND_MAX), 1};

        particle.velocity.y = float(cos(rand()));
        particle.velocity.x = float(sin(rand()));
        particle.radius = float(10);
        particles.push_back(particle);
    }
    // Set the flag to true after emitting the explosion
    hasExploded = true;
}