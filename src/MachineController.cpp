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
            lastTemperature = json["temp"];
            std::cout << "[" << machineId << "] Temperature: " << lastTemperature << "C\n";

            tooCold = lastTemperature < 10.0;
            overheating = lastTemperature > 80.0;

            if (overheating) {
                std::cout << "[" << machineId << "] ⚠️ WARNING: Temperature too high!\n";
            } else if (lastTemperature < 10.0) {
                std::cout << "[" << machineId << "] ⚠️ WARNING: Temperature too low!\n";
            }
        }
    } catch (const std::exception& ex) {
        std::cerr << "[" << machineId << "] Failed to parse temperature JSON: " << ex.what() << "\n";
    }
}

double MachineController::getLastTemperature() const {
    return lastTemperature;
}

bool MachineController::isTooCold() const {
    return tooCold;
}

bool MachineController::isOverheating() const {
    return overheating;
}

std::string MachineController::getMachineId() const {
    return machineId;
}
