#include <catch2/catch_all.hpp>
#include <glm/glm.hpp>

#include <particlesystem/particlesystem.h>

/* Unit tests using the catch2 framework
 * Homepage: https://github.com/catchorg/Catch2
 * Tutorial: https://github.com/catchorg/Catch2/blob/devel/docs/tutorial.md
 * Docs: https://github.com/catchorg/Catch2/blob/devel/docs/Readme.md
 */

// lab 3 test case

TEST_CASE("Check if emitters spawns and removes particles correctly", "[ParticleSystem]") {
    Particlesystem ps;

    // Create emitters
    auto emitterD = new Directional({0, 0}, 50);
    auto emitterU = new Uniform({0.5, 0}, 20);
    auto emitterE = new Explosion({-0.5, 0}, 2);

    ps.emitters.push_back(emitterD);
    ps.emitters.push_back(emitterU);
    ps.emitters.push_back(emitterE);

    SECTION("Particles are removed when their lifetime expires") {
        // Run for 1 second to spawn particles
        ps.update(1.0f);
        REQUIRE(ps.particles.size() > 0);  // Ensure particles exist

        // Delete emitters (stop spawning new particles)
        ps.emitters.clear();  // Removes all emitters (but doesn't delete them)
        delete emitterD;
        delete emitterU;
        delete emitterE;

        // Fast-forward time (e.g., 7 seconds) to let all particles die
        ps.update(7.0f);

        // Check that no particles remain
        REQUIRE(ps.particles.size() == 0);
    }
}

// lab 3 new emitter test case
TEST_CASE("SpinningUniform emitter rotates bursts correctly", "[ParticleSystem]") {
    Particlesystem ps;
    const float rotationSpeed = 1.0f;  // 1 radian per second
    const float burstDelay = 0.1f;     // Match emitter's internal delay

    // Create emitter with known burst timing
    auto emitter = new SpinningUniform({0, 0}, 12, rotationSpeed);  // 12 particles = 30° apart
    ps.emitters.push_back(emitter);

    SECTION("Verify burst rotation") {
        // Capture first burst
        ps.update(burstDelay);
        REQUIRE(ps.particles.size() == 13);
        std::vector<float> burst1Angles;
        for (const auto& p : ps.particles) {
            burst1Angles.push_back(std::atan2(p.velocity.y, p.velocity.x));
        }
        ps.particles.clear();  // Remove first burst

        // Wait for second burst
        ps.update(burstDelay);
        REQUIRE(ps.particles.size() == 13);

        // Verify angular offset
        float expectedRotation = rotationSpeed * burstDelay;
        float angleTolerance = 0.05f;  // Tight tolerance

        
        for (size_t i = 1; i < ps.particles.size(); i++) {
            float burst2Angle = std::atan2(ps.particles[i].velocity.y, ps.particles[i].velocity.x);
            float actualRotation = burst2Angle - burst1Angles[i];

            // Normalize to -pi, pi
            actualRotation = std::fmod(actualRotation + 3.1415926f, 2.0f * 3.1415926f) - 3.1415926f;

            REQUIRE_THAT(actualRotation,
                         Catch::Matchers::WithinAbs(expectedRotation, angleTolerance));
        }
    }
}