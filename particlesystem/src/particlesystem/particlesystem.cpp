#include <particlesystem/particlesystem.h>
#include <rendering/window.h> // Include the Window class for drawing



//// return a random value for "width" and "direction"
//float randomValue(float direction, float width) { 
//	return direction + width * rand() / static_cast<float>(RAND_MAX);
//}
//
//// Update postion for particle based on acceleration and time (dt)
//void Particle::updatePosition(const double dt) { 
//	this->Velocity += this->acceleration * (float)dt; 
//	this->position += this->velocity * (float)dt;
//}
//
//// create a uniformed distibution particle
//Particle Uniform::createParticle() { 
//	Particle myParticle(this->position);
//    return myParticle;
//}
//
//// Creates a particle with directional distribution
//Particle Directional::createParticle() {
//    Particle myParticle(this->position, this->direction, this->width);
//    return myParticle;
//}


void Particlesystem::update(float dt) {

    for (size_t i = 0; i < particles.size(); i++) {
        particles[i].lifetime -= dt;
        particles[i].position += particles[i].velocity * dt;

        if (particles[i].lifetime <= 0) {
            particles.erase(particles.begin() + int(i));
        }
    }

    // Add new particles from emitters
    for (size_t i = 0; i < emitters.size(); i++) {
        emitters[i]->addParticle(particles, dt);
    }

    // Apply effects to particles
    for (size_t j = 0; j < effects.size(); j++) {
        effects[j]->move(particles);
    }
}
