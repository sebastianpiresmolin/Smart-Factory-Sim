#define CATCH_CONFIG_MAIN
#include <catch2/catch_all.hpp>
#include <fstream>
#include <catch2/catch_test_macros.hpp>
#include "FactoryController.h"
#include "../include/Machine.h"

const std::string test_snapshot_path = "test_factory_snapshot.json";

TEST_CASE("FactoryController can persist and restore Machine state", "[Persistence]") {
    // Clean up any existing snapshot
    std::remove(test_snapshot_path.c_str());

    SECTION("Save and load a single machine") {
        FactoryController controller;
        // Add a machine and increment counters
        auto machineController = controller.getOrCreateMachine("machine1");
        auto machine = machineController->getMachine();
        machine->incrementProduced();
        machine->incrementLostMaterials();
        machine->incrementLostMaterials();

        // Save state
        controller.saveSnapshot(test_snapshot_path);

        // Create new controller and load state
        FactoryController restoredController;
        restoredController.loadSnapshot(test_snapshot_path);
        auto restoredMachineController = restoredController.getOrCreateMachine("machine1");
        auto restoredMachine = restoredMachineController->getMachine();

        REQUIRE(restoredMachine->getTotalProduced() == 1);
        REQUIRE(restoredMachine->getLostMaterials() == 2);
    }

    SECTION("Multiple machines persist state correctly") {
        FactoryController controller;
        auto m1 = controller.getOrCreateMachine("A")->getMachine();
        auto m2 = controller.getOrCreateMachine("B")->getMachine();
        m1->incrementProduced();
        m2->incrementProduced();
        m2->incrementProduced();

        controller.saveSnapshot(test_snapshot_path);

        FactoryController restored;
        restored.loadSnapshot(test_snapshot_path);

        auto r1 = restored.getOrCreateMachine("A")->getMachine();
        auto r2 = restored.getOrCreateMachine("B")->getMachine();
        REQUIRE(r1->getTotalProduced() == 1);
        REQUIRE(r2->getTotalProduced() == 2);
    }

    // Clean up at end
    std::remove(test_snapshot_path.c_str());
}