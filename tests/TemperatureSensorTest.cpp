#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "MachineController.h"

using Catch::Matchers::WithinAbs;

TEST_CASE("Temperature sensor behavior", "[TemperatureSensor]") {
    MachineController mc("machine1");

    SECTION("Normal temperature") {
        mc.handleSensor("temp", R"({"temp": 22.5})");
        REQUIRE_THAT(mc.getLastTemperature(), WithinAbs(22.5, 0.001));
        REQUIRE_FALSE(mc.isOverheating());
    }

    SECTION("Low temperature warning") {
        mc.handleSensor("temp", R"({"temp": 5.0})");
        REQUIRE_THAT(mc.getLastTemperature(), WithinAbs(5.0, 0.001));
        REQUIRE(mc.isTooCold());
    }

    SECTION("High temperature triggers overheating") {
        mc.handleSensor("temp", R"({"temp": 90.0})");
        REQUIRE_THAT(mc.getLastTemperature(), WithinAbs(90.0, 0.001));
        REQUIRE(mc.isOverheating());
    }

    SECTION("Exactly 80.0°C (not overheating)") {
        mc.handleSensor("temp", R"({"temp": 80.0})");
        REQUIRE_FALSE(mc.isOverheating());
    }

    SECTION("Exactly 10.0°C (not too cold)") {
        mc.handleSensor("temp", R"({"temp": 10.0})");
        REQUIRE_FALSE(mc.isTooCold());
    }

    SECTION("Malformed JSON input") {
        mc.handleSensor("temp", R"(invalid json)");
        // No exception should be thrown and state remains unchanged
        REQUIRE_THAT(mc.getLastTemperature(), WithinAbs(0.0, 0.001));
        REQUIRE_FALSE(mc.isOverheating());
    }
}
