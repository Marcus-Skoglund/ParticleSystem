#include <rendering/window.h>
#include <example/randomsystem.h>
#include <particlesystem/particlesystem.h>

#include <fmt/format.h>

#include <cmath>
#include <vector>
#include <random>
#include <algorithm>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>

#include <glm/gtc/constants.hpp> // For glm::pi
#include <glm/gtc/type_ptr.hpp>  // For glm::radians

//#include <tracy/Tracy.hpp>

/*
 * This application represents the "Client" the uses your particle system
 */
//int main() try {
//    rendering::Window window("Particle System v0.0.2 pre-release alpha", 850, 850);
//
//    // Creates an example system with a 1000 particles.
//    // This should be replaced with your new particle system
//    const size_t numParticles = 1000;
//    example::RandomSystem system{numParticles};
//
//    float speed = 1.0f;
//    bool running = true;
//
//    while (running) {
//        window.beginFrame();
//
//        // Update the example system.
//        // This should be replaced with a call to the new particle system
//        system.update(window.time(), speed);
//
//        // Clear screen with color
//        window.clear({0, 0, 0, 1});
//
//        // Draw particles
//        // This should also be adjusted to work with the new particle system
//        window.drawPoints(system.getPosition(), system.getSize(), system.getColor());
//
//        // Example user interface, extend for your needs.
//        // Might add controls for effects and emitters etc.
//        {
//            window.beginGuiWindow("UI");
//            window.text("I'm text!");
//            window.sliderFloat("Speed", speed, 0.001f, 10.0f);
//            if (window.button("Close application")) {
//                running = false;
//            }
//            window.endGuiWindow();
//        }
//
//        window.endFrame();
//        running = running && !window.shouldClose();
//    }
//
//    return EXIT_SUCCESS;
//} catch (const std::exception& e) {
//    fmt::print("{}", e.what());
//    return EXIT_FAILURE;
//}



// Simple random functions to get started
// Random float (0,1)
float rnd() { return rand() / static_cast<float>(RAND_MAX); }

// Random float (-1,1)
float srnd() { return rnd() * 2.0f - 1.0f; }

int main(int, char**) try {
    rendering::Window window("Particle System v0.0.1 pre-release alpha", 2000, 1300);

    Particlesystem ps;

    std::vector<glm::vec2> positions;
    std::vector<float> radiuses;
    std::vector<glm::vec4> colors;

    double prevTime = 0.0;
    bool running = true;

    while (running) {
        window.beginFrame();

        const double t = window.time();
        float dt = t - prevTime;
        prevTime = t;

        // Clear screen with color
        window.clear({0, 0, 0, 1});

        {
            //ZoneScopedN("Update particles");

            ps.update(dt);

            for (size_t i = 0; i < ps.particles.size(); i++) {
                positions.push_back(ps.particles[i].position);
                radiuses.push_back(ps.particles[i].radius);
                colors.push_back(ps.particles[i].color);
            }

            window.drawPoints(positions, radiuses, colors);

            /*    for (size_t i = 0; i < ps.particles.size(); i++) {
                    window.drawPoint(ps.particles[i].position, ps.particles[i].radius,
                                     ps.particles[i].color);
                }*/

            positions.clear();
            radiuses.clear();
            colors.clear();
        }

        // UI
        {
            {
                window.beginGuiWindow("Particle System");

                if (window.button("Close application")) {
                    running = false;
                }

                // ------------ EMITTERS ------------

                // Directional
                static float directionalPosX = 0.9f;      // Default X position
                static float directionalPosY = 0.0f;      // Default Y position
                static float directionalAngleDegrees = 180.0f;  // Default angle (180 degrees in radians)
                static float directionalAngle;

                // Slider for X position
                if (window.sliderFloat("Position X", directionalPosX, -1.0f, 1.0f)) {
                    // X position was updated
                }

                // Slider for Y position
                if (window.sliderFloat("Position Y", directionalPosY, -1.0f, 1.0f)) {
                    // Y position was updated
                }

                // Slider for direction in degrees
                if (window.sliderFloat("Direction (Degrees)", directionalAngleDegrees, 0.0f,
                                       360.0f)) {
                    // Convert degrees to radians for the emitter
                    directionalAngle = glm::radians(directionalAngleDegrees);
                }

                // Button to create a new directional emitter
                if (window.button("Create Directional Emitter")) {
                    // Create a glm::vec2 for the position
                    glm::vec2 directionalPosition = {directionalPosX, directionalPosY};

                    // Create a new directional emitter with the current position and angle
                    ps.emitters.push_back(new Directional(directionalPosition, directionalAngle));
                }

                // Uniform
                static float uniformPosX = -0.3f;    // Default X position
                static float uniformPosY = 0.4f;     // Default Y position
                static int amount = 10;              // Default particle amount

                // Slider for X position
                if (window.sliderFloat("Position X", uniformPosX, -1.0f, 1.0f)) {
                    // X position was updated
                }

                // Slider for Y position
                if (window.sliderFloat("Position Y", uniformPosY, -1.0f, 1.0f)) {
                    // Y position was updated
                }

                // Slider for amount position
                if (window.sliderInt("Particle amount", amount, 0, 40)) {
                    // Amount was updated
                }

                // Button to create a new uniform emitter
                if (window.button("Create Uniform Emitter")) {
                    // Create a glm::vec2 for the position
                    glm::vec2 uniformPosition = { uniformPosX, uniformPosY };

                    // Create a new uniform emitter with the current position and amount
                    ps.emitters.push_back(new Uniform(uniformPosition, amount));
                }

                // Explosion
                static float explosionPosX = 0.0f;  // Default X position
                static float explosionPosY = 0.0f;   // Default Y position
                static int amount_ex = 20;            // Default particle amount

                // Slider for X position
                if (window.sliderFloat("Position X", explosionPosX, -1.0f, 1.0f)) {
                    // X position was updated
                }

                // Slider for Y position
                if (window.sliderFloat("Position Y", explosionPosY, -1.0f, 1.0f)) {
                    // Y position was updated
                }

                // Slider for amount position
                if (window.sliderInt("Particle amount", amount_ex, 0, 100)) {
                    // Amount was updated
                }

                // Button to create a new explosion emitter
                if (window.button("Create Explosion Emitter")) {
                    // Create a glm::vec2 for the position
                    glm::vec2 explosionPosition = { explosionPosX, explosionPosY };

                    // Create a new explosion emitter with the current position and amount
                    ps.emitters.push_back(new Explosion(explosionPosition, amount_ex));
                }

                // ------------ EFFECTS ------------

                // Gravity effect
                static float gravityPosX = -0.3f;  // Default X position
                static float gravityPosY = -0.3f;  // Default Y position
                static int power_gravity = 3;      // power of the gravity
                static float gravity_range = 0.5f;  // the range of the gravity

                // Slider for X position
                if (window.sliderFloat("Position X", gravityPosX, -1.0f, 1.0f)) {
                    // X position was updated
                }

                // Slider for Y position
                if (window.sliderFloat("Position Y", gravityPosY, -1.0f, 1.0f)) {
                    // Y position was updated
                }

                // Slider for power
                if (window.sliderInt("Gravity power", power_gravity, 0, 100)) {
                    // power was updated
                }

                 // Slider for gravity range
                if (window.sliderFloat("Gravity range", gravity_range, 0.1f, 2.0f)) {
                    // Gravity range was updated
                }

                // Button to create a new gravity effect
                if (window.button("Create Gravity Effect")) {
                    // Create a glm::vec2 for the position
                    glm::vec2 gravityPosition = {gravityPosX, gravityPosY};

                    // Create a new directional emitter with the current position and angle
                    ps.effects.push_back(new GravityWell(gravityPosition, power_gravity, gravity_range));
                }

                // Wind effect
                static float windPosX = 0.0f;  // Default X position
                static float windPosY = 0.0f; // Default Y position
                static int power = 5;        // power of the wind
                static float directionalAngleDegrees_wind = 0.0f;  // Default angle (180 degrees in radians)
                static float directionalAngle_wind;

                // Slider for X position
                if (window.sliderFloat("Position X", windPosX, -1.0f, 1.0f)) {
                    // X position was updated
                }

                // Slider for Y position
                if (window.sliderFloat("Position Y", windPosY, -1.0f, 1.0f)) {
                    // Y position was updated
                }

                // Slider for power
                if (window.sliderInt("Wind power", power, 0, 20)) {
                    // power was updated
                }

                // Slider for direction in degrees
                if (window.sliderFloat("Direction (Degrees)", directionalAngleDegrees_wind, 0.0f,
                    360.0f)) {
                    // Convert degrees to radians for the emitter
                    directionalAngle_wind = glm::radians(directionalAngleDegrees_wind);
                }

                // Button to create a new wind effect
                if (window.button("Create Wind Effect")) {
                    // Create a glm::vec2 for the position
                    glm::vec2 windPosition = { windPosX, windPosY };

                    // Create a new directional emitter with the current position and angle
                    ps.effects.push_back(new Wind(windPosition, directionalAngle_wind, power));
                }

                window.endGuiWindow();
            }

            {
                window.beginGuiWindow("Active Emitters and Effects");

                 // Display active emitters
                window.text("Active Emitters:");
                size_t displayIndex = 0;  // Separate counter for displayed index

                for (size_t i = 0; i < ps.emitters.size(); i++) {
                    // Skip explosion emitters (do not show them in the list)
                    if (dynamic_cast<Explosion*>(ps.emitters[i])) {
                        continue;  // Skip this emitter without incrementing displayIndex
                    }

                    std::string emitterLabel =
                        "Emitter " + std::to_string(displayIndex);  // Use displayIndex
                    if (dynamic_cast<Directional*>(ps.emitters[i])) {
                        emitterLabel += " (Directional)";
                    } else if (dynamic_cast<Uniform*>(ps.emitters[i])) {
                        emitterLabel += " (Uniform)";
                    }

                    // Display emitter details
                    window.text(emitterLabel);

                    // Add a delete button for the emitter
                    if (window.button("Delete Emitter " + std::to_string(displayIndex))) {
                        delete ps.emitters[i];                       // Free memory
                        ps.emitters.erase(ps.emitters.begin() + i);  // Remove from the vector
                        i--;  // Adjust the index after deletion
                    }

                    displayIndex++;  // Increment the displayed index only for non-Explosion
                                     // emitters
                }
                

                // Display active effects
                window.text("Active Effects:");
                for (size_t i = 0; i < ps.effects.size(); i++) {
                    std::string effectLabel = "Effect " + std::to_string(i);
                    if (dynamic_cast<GravityWell*>(ps.effects[i])) {
                        effectLabel += " (Gravity Well)";
                    } else if (dynamic_cast<Wind*>(ps.effects[i])) {
                        effectLabel += " (Wind)";
                    }

                    // Display effect details
                    window.text(effectLabel);

                    // Add a delete button for the effect
                    if (window.button("Delete Effect " + std::to_string(i))) {
                        delete ps.effects[i];                      // Free memory
                        ps.effects.erase(ps.effects.begin() + i);  // Remove from the vector
                        i--;  // Adjust the index after deletion
                    }
                }

                window.endGuiWindow();
            }
        }

        window.endFrame();
        running = running && !window.shouldClose();
    }

    return EXIT_SUCCESS;
} catch (const std::exception& e) {
    fmt::print("{}", e.what());
    return EXIT_FAILURE;
}