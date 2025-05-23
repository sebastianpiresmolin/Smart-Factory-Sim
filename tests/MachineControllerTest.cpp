﻿#include <catch2/catch_test_macros.hpp>
#include "MachineController.h"

TEST_CASE("MachineController basics", "[MachineController]") {
    FactoryController dummyFactory;
    MachineController mc("machine42", &dummyFactory);

    SECTION("Machine ID is stored correctly") {
        REQUIRE(mc.getMachineId() == "machine42");
    }

    SECTION("Handles unknown sensor type without throwing") {
        REQUIRE_NOTHROW(mc.handleSensor("unknown", R"({"foo":42})"));
    }
}