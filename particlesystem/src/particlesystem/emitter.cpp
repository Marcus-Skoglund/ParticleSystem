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

void SpinningUniform::addParticle(std::vector<Particle>& particles, float dt) {
    delay = 0.1;
    const int numParticles = amount;
    const float angleStep = 2 * 3.1415f / numParticles;
    const float rotationSpeed = (speed * dt);
    static float totalRotation = 0.0f;  // Accumulated rotation over time
    totalRotation += rotationSpeed;     // Update rotation

    // emitter
    Particle Emit;
    Emit.position = position;
    Emit.radius = 20;

    // Make the emitter's color or shape rotate (example: pulsing radius + spinning hue)
    Emit.radius = 20.0f + 5.0f * sin(totalRotation * 2.0f);  // Pulsing effect
    Emit.color = {
        0.5f + 0.5f * cos(totalRotation),  // R (varies with rotation)
        0.5f + 0.5f * sin(totalRotation),  // G (varies with rotation)
        1.0f,                              // B (fixed)
        1.0f                               // A (opaque)
    };
    Emit.velocity = {0.0f, 0.0f};
    particles.push_back(Emit);

    if (tick(dt)) {
        Particle particle;
        particle.position = position;
        particle.lifetime = 5;
        particle.color = {0.9, 0.1, 1, 1};
        particle.radius = float(10);
        for (int i = 0; i < numParticles; i++) {
            // Calculate angle with rotation applied
            float newangle = angleStep * i + totalRotation;

            particle.velocity.y = sin(newangle);
            particle.velocity.x = cos(newangle);
            
            particles.push_back(particle);
        }
    }
}

void Explosion::addParticle(std::vector<Particle>& particles, float dt) {

    dt;

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