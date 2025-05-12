#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

#include "../include/FactoryController.h"

using Catch::Approx;
using Catch::Matchers::WithinAbs;

TEST_CASE("FactoryController handles new machine messages") {
    FactoryController fc;

    std::string topic = "factory/machineX/temp";
    std::string payload = R"({"temp": 42.0})";

    fc.handleMessage(topic, payload);

    auto mc = fc.getMachine("machineX");
    REQUIRE(mc != nullptr);

    auto machine = mc->getMachine();
    REQUIRE(machine != nullptr);

    auto temp = machine->getSensorValue("temp");
    REQUIRE(temp.has_value());
    REQUIRE(temp.value() == Approx(42.0));
}

TEST_CASE("FactoryController handles malformed topics gracefully") {
    FactoryController fc;

    SECTION("Empty topic") {
        fc.handleMessage("", R"({"temp": 42.0})");
        REQUIRE(fc.getMachine("machine1") == nullptr);
    }

    SECTION("Missing sensor type in topic") {
        fc.handleMessage("factory/machine1", R"({"temp": 42.0})");
        REQUIRE(fc.getMachine("machine1") == nullptr);
    }

    SECTION("Completely invalid topic format") {
        fc.handleMessage("invalidtopic", R"({"temp": 42.0})");
        REQUIRE(fc.getMachine("machine1") == nullptr);
    }
}

TEST_CASE("FactoryController returns correct snapshot of sensor states", "[FactoryController][Snapshot]") {
    FactoryController factory;

    // Simulate messages from machines
    factory.handleMessage("factory/machine0/temp", R"({"temp": 12.5})");
    factory.handleMessage("factory/machine1/temp", R"({"temp": 78.0})");
    factory.handleMessage("factory/machine2/temp", R"({"temp": 91.3})");

    auto snapshot = factory.getSensorStates();

    REQUIRE(snapshot.contains("machine0"));
    REQUIRE(snapshot.contains("machine1"));
    REQUIRE(snapshot.contains("machine2"));

    REQUIRE_THAT(snapshot["machine0"]["temp"], WithinAbs(12.5, 0.001));
    REQUIRE_THAT(snapshot["machine1"]["temp"], WithinAbs(78.0, 0.001));
    REQUIRE_THAT(snapshot["machine2"]["temp"], WithinAbs(91.3, 0.001));
}
