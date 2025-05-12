#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>
#include "FactoryController.h"

using Catch::Matchers::WithinAbs;

TEST_CASE("Temperature sensor updates for multiple machines", "[TemperatureSensor]") {
    FactoryController factory;

    SECTION("Each machine stores temperature independently") {
        factory.handleMessage("factory/machine0/temp", R"({"temp": 5.0})");
        factory.handleMessage("factory/machine1/temp", R"({"temp": 22.5})");
        factory.handleMessage("factory/machine2/temp", R"({"temp": 90.0})");

        auto m0 = factory.getMachine("machine0");
        auto m1 = factory.getMachine("machine1");
        auto m2 = factory.getMachine("machine2");

        REQUIRE(m0);
        REQUIRE(m1);
        REQUIRE(m2);

        REQUIRE_THAT(m0->getMachine()->getSensorValue("temp").value(), WithinAbs(5.0, 0.001));
        REQUIRE_THAT(m1->getMachine()->getSensorValue("temp").value(), WithinAbs(22.5, 0.001));
        REQUIRE_THAT(m2->getMachine()->getSensorValue("temp").value(), WithinAbs(90.0, 0.001));

        REQUIRE(m0->isTooCold());
        REQUIRE_FALSE(m1->isTooCold());
        REQUIRE_FALSE(m1->isOverheating());
        REQUIRE(m2->isOverheating());
    }
}

