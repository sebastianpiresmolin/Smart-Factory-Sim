#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "../include/FactoryController.h"

using Catch::Approx;
using Catch::Matchers::WithinAbs;

TEST_CASE("FactoryController returns correct snapshot of sensor states", "[FactoryController][Snapshot]") {
    FactoryController factory;

    // Start all machines
    factory.handleMessage("factory/machine0/state", R"({"state": "start"})");
    factory.handleMessage("factory/machine1/state", R"({"state": "start"})");
    factory.handleMessage("factory/machine2/state", R"({"state": "start"})");

    // Simulate messages from machines
    factory.handleMessage("factory/machine0/temp", R"({"temp": 12.5})");
    factory.handleMessage("factory/machine0/vibration", R"({"vibration": 2.0})");

    factory.handleMessage("factory/machine1/temp", R"({"temp": 78.0})");
    factory.handleMessage("factory/machine1/vibration", R"({"vibration": 5.2})");

    factory.handleMessage("factory/machine2/temp", R"({"temp": 91.3})");
    factory.handleMessage("factory/machine2/vibration", R"({"vibration": 8.1})");

    auto snapshot = factory.getSensorStates();

    REQUIRE(snapshot.contains("machine0"));
    REQUIRE(snapshot.contains("machine1"));
    REQUIRE(snapshot.contains("machine2"));

    // ✅ Check machine0
    REQUIRE(snapshot["machine0"]["state"] == "running");
    REQUIRE_THAT(snapshot["machine0"]["sensors"]["temp"], WithinAbs(12.5, 0.001));
    REQUIRE_THAT(snapshot["machine0"]["sensors"]["vibration"], WithinAbs(2.0, 0.001));

    // ✅ Check machine1
    REQUIRE(snapshot["machine1"]["state"] == "running");
    REQUIRE_THAT(snapshot["machine1"]["sensors"]["temp"], WithinAbs(78.0, 0.001));
    REQUIRE_THAT(snapshot["machine1"]["sensors"]["vibration"], WithinAbs(5.2, 0.001));

    // ✅ Check machine2
    REQUIRE(snapshot["machine2"]["state"] == "running");
    REQUIRE_THAT(snapshot["machine2"]["sensors"]["temp"], WithinAbs(91.3, 0.001));
    REQUIRE_THAT(snapshot["machine2"]["sensors"]["vibration"], WithinAbs(8.1, 0.001));
}
