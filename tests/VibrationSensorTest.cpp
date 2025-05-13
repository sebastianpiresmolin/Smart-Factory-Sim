#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "MachineController.h"

using Catch::Matchers::WithinAbs;

TEST_CASE("Vibration sensor behavior", "[VibrationSensor]") {
    MachineController mc("machine1");

    // Ensure the machine is running before testing sensor behavior
    mc.handleSensor("state", R"({"state": "start"})");

    SECTION("Normal vibration level") {
        mc.handleSensor("vibration", R"({"vibration": 3.2})");
        auto vibration = mc.getMachine()->getSensorValue("vibration");
        REQUIRE(vibration.has_value());
        REQUIRE_THAT(vibration.value(), WithinAbs(3.2, 0.001));
        REQUIRE_FALSE(mc.getMachine()->isExcessiveVibration());
    }

    SECTION("High vibration level triggers warning") {
        mc.handleSensor("vibration", R"({"vibration": 6.5})");
        auto vibration = mc.getMachine()->getSensorValue("vibration");
        REQUIRE(vibration.has_value());
        REQUIRE_THAT(vibration.value(), WithinAbs(6.5, 0.001));
        REQUIRE(mc.getMachine()->isExcessiveVibration());
    }

    SECTION("Malformed JSON does not crash") {
        REQUIRE_NOTHROW(mc.handleSensor("vibration", "not a json"));
    }

    SECTION("Missing 'vibration' field does not set sensor value") {
        mc.handleSensor("vibration", R"({"other": 1.0})");
        auto vibration = mc.getMachine()->getSensorValue("vibration");
        REQUIRE_FALSE(vibration.has_value());
    }
}
