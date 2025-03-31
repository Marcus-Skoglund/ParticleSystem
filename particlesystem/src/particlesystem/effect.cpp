#include <particlesystem/effect.h>
#include <cmath>
#include <glm/geometric.hpp> // For glm::normalize
#include <glm/glm.hpp>       // For general GLM functionality

void GravityWell::move(std::vector<Particle>& particles) {

    for (size_t i = 0; i < particles.size(); i++) {

        // Calculate the distance between the gravity well and the particle
        glm::vec2 diff = position - particles[i].position;

        // Calculate the squared distance (avoiding sqrt for performance)
        float distanceSquared = diff.x * diff.x + diff.y * diff.y;

        if (distanceSquared < range * range) {
            // Avoid division by zero by ensuring a minimum distance
            const float minDistance = 0.01f;
            if (distanceSquared < minDistance * minDistance) {
                distanceSquared = minDistance * minDistance;
            }

            // Calculate the gravitational force (inverse square law)
            float force = gravity / distanceSquared;

            // Normalize the direction vector
            //glm::vec2 forceDirection = glm::normalize(diff);

            // Apply the gravitational force to the particle's velocity
            particles[i].velocity.x += diff.x * force * 0.0001f;   // multiplyer
            particles[i].velocity.y += diff.y * force * 0.0001f;  // multiplyer
        }
    }
}

void Wind::move(std::vector<Particle>& particles) {
    // Calculate the wind force direction based on the angle
    glm::vec2 windDirection = {cos(angle), sin(angle)};
    
    // Scale the wind force by the power
    glm::vec2 windForce = windDirection * (power * 0.001f);  // multiplier

    for (size_t i = 0; i < particles.size(); i++) {
        // Calculate the distance between the wind's and the particle's position
        float diffX = particles[i].position.x - position.x;
        float diffY = particles[i].position.y - position.y;

        // Check if the particle is within a certain range of the wind's position
        float distanceSquared = diffX * diffX + diffY * diffY;
        float range = 0.5f;  // the range of the wind

        if (distanceSquared < range * range) {
            // Apply the wind force to the particle's velocity
            particles[i].velocity += windForce;
        }
    }
}
