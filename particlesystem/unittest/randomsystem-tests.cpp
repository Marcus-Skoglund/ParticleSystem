#include <catch2/catch_all.hpp>
#include <glm/glm.hpp>

#include <example/randomsystem.h>

/* Unit tests using the catch2 framework
 * Homepage: https://github.com/catchorg/Catch2
 * Tutorial: https://github.com/catchorg/Catch2/blob/devel/docs/tutorial.md
 * Docs: https://github.com/catchorg/Catch2/blob/devel/docs/Readme.md
 */

TEST_CASE("Empty system", "[RandomSystem]") {

    example::RandomSystem empty{0};

    REQUIRE(empty.getPosition().size() == 0);
    REQUIRE(empty.getSize().size() == 0);
    REQUIRE(empty.getColor().size() == 0);
}

TEST_CASE("Initial system", "[RandomSystem]") {
    example::RandomSystem system{100};

    SECTION("There should be the same number of positions, sizes, and colors") {
        REQUIRE(system.getPosition().size() == 100);
        REQUIRE(system.getSize().size() == 100);
        REQUIRE(system.getColor().size() == 100);
    }

    SECTION("All positions should initially be within -1,1") {
        REQUIRE(std::ranges::all_of(system.getPosition(), [](auto& p) { return p.x >= -1.0f; }));
        REQUIRE(std::ranges::all_of(system.getPosition(), [](auto& p) { return p.x <= 1.0f; }));
        REQUIRE(std::ranges::all_of(system.getPosition(), [](auto& p) { return p.y >= -1.0f; }));
        REQUIRE(std::ranges::all_of(system.getPosition(), [](auto& p) { return p.y <= 1.0f; }));
    }

    SECTION("Color rgb components should be in the 0,1 range") {
        REQUIRE(std::ranges::all_of(system.getColor(), [](auto& c) { return c.r >= 0.0f; }));
        REQUIRE(std::ranges::all_of(system.getColor(), [](auto& c) { return c.r <= 1.0f; }));
        REQUIRE(std::ranges::all_of(system.getColor(), [](auto& c) { return c.g >= 0.0f; }));
        REQUIRE(std::ranges::all_of(system.getColor(), [](auto& c) { return c.g <= 1.0f; }));
        REQUIRE(std::ranges::all_of(system.getColor(), [](auto& c) { return c.b >= 0.0f; }));
        REQUIRE(std::ranges::all_of(system.getColor(), [](auto& c) { return c.b <= 1.0f; }));
    }

    // The same test but using matchers
    SECTION("Color rgb components should be in the 0,1 range") {
        using namespace Catch::Matchers;
        REQUIRE_THAT(system.getColor(), AllMatch(Predicate<glm::vec4>(
                                            [](const glm::vec4& color) {
                                                return color.r >= 0.0f && color.r <= 1.0f &&
                                                       color.g >= 0.0f && color.g <= 1.0f &&
                                                       color.b >= 0.0f && color.b <= 1.0f;
                                            },
                                            "Color rgb components should be in the 0,1 range")));
    }
}

// Tests using the GIVEN/WHEN/THEN pattern
SCENARIO("System updates", "[RandomSystem]") {
    GIVEN("A random system with 100 particles") {
        example::RandomSystem system{100};

        THEN("The initial size should be 100") {
            REQUIRE(system.getPosition().size() == 100);
            REQUIRE(system.getSize().size() == 100);
            REQUIRE(system.getColor().size() == 100);
        }

        WHEN("The system is updated less than the min lifetime") {
            std::vector<glm::vec2> initialPositions(system.getPosition().begin(),
                                                    system.getPosition().end());
            std::vector<float> initialSize(system.getSize().begin(), system.getSize().end());
            std::vector<glm::vec4> initialColor(system.getColor().begin(), system.getColor().end());

            system.update(0.1, 1.0);

            THEN("The system should have the same size, but different positions") {

                REQUIRE(system.getPosition().size() == 100);
                REQUIRE(system.getSize().size() == 100);
                REQUIRE(system.getColor().size() == 100);

                REQUIRE(!std::ranges::equal(system.getPosition(), initialPositions));
                REQUIRE(std::ranges::equal(system.getSize(), initialSize));

                // The rgb values should not change but alpha might
                constexpr auto rgb = [](auto& c) { return glm::vec3(c); };
                REQUIRE(std::ranges::equal(system.getColor(), initialColor, std::equal_to<>{}, rgb,
                                           rgb));
            }
        }

        WHEN("The system is updated more than the max lifetime") {
            std::vector<glm::vec2> initialPositions(system.getPosition().begin(),
                                                    system.getPosition().end());
            std::vector<float> initialSize(system.getSize().begin(), system.getSize().end());
            std::vector<glm::vec4> initialColor(system.getColor().begin(), system.getColor().end());

            system.update(5.0, 1.0);

            THEN(
                "The system should have the same size, but different positions, "
                "sizes and "
                "colors") {

                REQUIRE(system.getPosition().size() == 100);
                REQUIRE(system.getSize().size() == 100);
                REQUIRE(system.getColor().size() == 100);

                REQUIRE(!std::ranges::equal(system.getPosition(), initialPositions));
                REQUIRE(!std::ranges::equal(system.getSize(), initialSize));
                REQUIRE(!std::ranges::equal(system.getColor(), initialColor));
            }
        }

        WHEN("The system is copied") {
            auto copy = system;

            THEN("The copy should have the same state") {

                REQUIRE(copy.getPosition().size() == 100);
                REQUIRE(copy.getSize().size() == 100);
                REQUIRE(copy.getColor().size() == 100);

                REQUIRE(std::ranges::equal(system.getPosition(), copy.getPosition()));
                REQUIRE(std::ranges::equal(system.getSize(), copy.getSize()));
                REQUIRE(std::ranges::equal(system.getColor(), copy.getColor()));
            }
        }

        WHEN("The system is copied and updated") {
            auto copy = system;
            copy.update(0.1, 1.0);

            THEN(
                "The copy should have the same size, but different positions, "
                "(deep copy)") {

                REQUIRE(copy.getPosition().size() == 100);
                REQUIRE(copy.getSize().size() == 100);
                REQUIRE(copy.getColor().size() == 100);

                REQUIRE(!std::ranges::equal(system.getPosition(), copy.getPosition()));
                REQUIRE(std::ranges::equal(system.getSize(), copy.getSize()));

                // The rgb values should not change but alpha might
                constexpr auto rgb = [](auto& c) { return glm::vec3{c}; };
                REQUIRE(std::ranges::equal(system.getColor(), copy.getColor(), std::equal_to<>{},
                                           rgb, rgb));
            }
        }
    }
}

// An example benchmark using the catch system
// See https://github.com/catchorg/Catch2/blob/devel/docs/benchmarks.md
// Run using: ./unittest "[benchmark]"
TEST_CASE("Benchmark system update", "[.benchmark]") {
    {
        example::RandomSystem system1{100};
        BENCHMARK("100 particles") { return system1.update(0.1, 1.0); };
    }

    {
        example::RandomSystem system2{1'000};
        BENCHMARK("1'000 particles") { return system2.update(0.1, 1.0); };
    }
    {
        example::RandomSystem system3{10'000};
        BENCHMARK("10'000 particles") { return system3.update(0.1, 1.0); };
    }
    {
        example::RandomSystem system4{100'000};
        BENCHMARK("100'000 particles") { return system4.update(0.1, 1.0); };
    }
}
