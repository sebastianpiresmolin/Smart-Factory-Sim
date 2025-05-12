#include <catch2/catch_test_macros.hpp>
#include "../include/FactoryController.h"

TEST_CASE("FactoryController handles new machine messages") {
    FactoryController fc;

    std::string topic = "factory/machineX/temp";
    std::string payload = R"({"temp": 42.0})";

    fc.handleMessage(topic, payload);

    auto mc = fc.getMachine("machineX");
    REQUIRE(mc != nullptr);
    REQUIRE(mc->getLastTemperature() == 42.0);
}

TEST_CASE("FactoryController handles malformed topics gracefully") {
    FactoryController fc;

    SECTION("Empty topic") {
        fc.handleMessage("", R"({"temp": 42.0})");
        REQUIRE(fc.getMachine("machine1") == nullptr);  // Nothing should be created
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