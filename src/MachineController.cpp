#include "MachineController.h"
#include <iostream>
#include <nlohmann/json.hpp>

MachineController::MachineController(const std::string& id) : machineId(id) {}

void MachineController::handleSensor(const std::string& sensorType, const std::string& payload) {
    if (sensorType == "temp") {
        handleTemperature(payload);
    } else {
        std::cout << "[" << machineId << "] Unhandled sensor type: " << sensorType << "\n";
    }
}

void MachineController::handleTemperature(const std::string& payload) {
    try {
        auto json = nlohmann::json::parse(payload);
        if (json.contains("temp")) {
            double temperature = json["temp"];
            std::cout << "[" << machineId << "] Temperature: " << temperature << "C\n";

            if (temperature > 80.0) {
                std::cout << "[" << machineId << "] ⚠️ WARNING: Temperature too high!\n";
            } else if (temperature < 10.0) {
                std::cout << "[" << machineId << "] ⚠️ WARNING: Temperature too low!\n";
            }
        }
    }
    catch (const std::exception& ex) {
        std::cerr << "[" << machineId << "] Failed to parse temperature JSON: " << ex.what() << "\n";
    }
}
