#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>
#include "../include/Machine.h"

using Catch::Approx;

TEST_CASE("Machine stores and retrieves sensor values", "[Machine]") {
    Machine m("machine1");

    SECTION("Initial sensor map is empty") {
        REQUIRE(m.getAllSensorValues().empty());
    }

    SECTION("Can set and get sensor value") {
        m.setSensorValue("temp", 42.0);
        auto val = m.getSensorValue("temp");

        REQUIRE(val.has_value());
        REQUIRE(val.value() == Approx(42.0));
    }

    SECTION("Returns std::nullopt for unknown sensor") {
        REQUIRE_FALSE(m.getSensorValue("humidity").has_value());
    }

    SECTION("Multiple sensors are stored independently") {
        m.setSensorValue("temp", 42.0);
        m.setSensorValue("pressure", 101.3);

        REQUIRE(m.getSensorValue("temp").value() == Approx(42.0));
        REQUIRE(m.getSensorValue("pressure").value() == Approx(101.3));
    }
}

TEST_CASE("Machine counters increment correctly", "[Machine]") {
    Machine m("m0");

    SECTION("Produced increments") {
        REQUIRE(m.getTotalProduced() == 0);
        m.incrementProduced();
        REQUIRE(m.getTotalProduced() == 1);
        m.incrementProduced();
        REQUIRE(m.getTotalProduced() == 2);
    }

    SECTION("Lost materials increments") {
        REQUIRE(m.getLostMaterials() == 0);
        m.incrementLostMaterials();
        REQUIRE(m.getLostMaterials() == 1);
        m.incrementLostMaterials();
        REQUIRE(m.getLostMaterials() == 2);
    }
}

TEST_CASE("Produced only increments when running", "[Machine]") {
    Machine m("m1");
    m.stop();
    m.incrementProduced();
    REQUIRE(m.getTotalProduced() == 1);
}
