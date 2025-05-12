#include "MachineController.h"
#include <iostream>
#include <nlohmann/json.hpp>

MachineController::MachineController(const std::string& id)
    : machineId(id), machine(std::make_shared<Machine>(id)) {}

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
            machine->setSensorValue("temp", temperature);

            auto currentTemp = machine->getSensorValue("temp");
            if (currentTemp) {
                std::cout << "[" << machineId << "] Live Temp: " << *currentTemp << "°C\n";

                if (machine->isOverheating()) {
                    std::cout << "[" << machineId << "] WARNING: Temperature too high!\n";
                } else if (machine->isTooCold()) {
                    std::cout << "[" << machineId << "] WARNING: Temperature too low!\n";
                } else {
                    std::cout << "[" << machineId << "] Temperature is normal.\n";
                }
            }
        }
    } catch (const std::exception& ex) {
        std::cerr << "[" << machineId << "] Failed to parse temperature JSON: " << ex.what() << "\n";
    }
}


std::unordered_map<std::string, double> MachineController::getAllSensorValues() const {
    return machine->getAllSensorValues();
}


std::shared_ptr<Machine> MachineController::getMachine() const {
    return machine;
}

std::string MachineController::getMachineId() const {
    return machineId;
}

bool MachineController::isOverheating() const {
    return machine->getSensorValue("temp") > 80.0;
}

bool MachineController::isTooCold() const {
    return machine->getSensorValue("temp") < 10.0;
}
