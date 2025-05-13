#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_string.hpp>

#include "MachineController.h"

using Catch::Matchers::Equals;

TEST_CASE("MachineController handles machine state changes", "[MachineController][State]") {
    MachineController mc("machineTest");

    SECTION("Initial state is stopped") {
        REQUIRE_FALSE(mc.getMachine()->isRunning());
    }

    SECTION("Handles start state") {
        mc.handleSensor("state", R"({"state":"start"})");
        REQUIRE(mc.getMachine()->isRunning());
    }

    SECTION("Handles stop state") {
        mc.handleSensor("state", R"({"state":"start"})");
        REQUIRE(mc.getMachine()->isRunning());

        mc.handleSensor("state", R"({"state":"stop"})");
        REQUIRE_FALSE(mc.getMachine()->isRunning());
    }

    SECTION("Ignores invalid state") {
        mc.handleSensor("state", R"({"state":"invalid"})");
        REQUIRE_FALSE(mc.getMachine()->isRunning());
    }

    SECTION("Handles malformed JSON gracefully") {
        REQUIRE_NOTHROW(mc.handleSensor("state", R"({bad_json)"));
    }
}
