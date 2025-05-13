#include "FactoryController.h"
#include <sstream>
#include <iostream>
#include <nlohmann/json.hpp>

void FactoryController::handleMessage(const std::string& topic, const std::string& payload) {
    std::string machineId = parseMachineId(topic);
    std::string sensorType = parseSensorType(topic);

    if (machineId.empty() || sensorType.empty()) {
        std::cerr << "Invalid topic: " << topic << "\n";
        return;
    }

    if (machines.find(machineId) == machines.end()) {
        machines[machineId] = std::make_shared<MachineController>(machineId);
    }

    machines[machineId]->handleSensor(sensorType, payload);
}

std::string FactoryController::parseMachineId(const std::string& topic) {
    std::istringstream stream(topic);
    std::string part1, part2;
    if (getline(stream, part1, '/') && getline(stream, part2, '/')) {
        return part2;
    }
    return "";
}

std::string FactoryController::parseSensorType(const std::string& topic) {
    std::istringstream stream(topic);
    std::string part1, part2, part3;
    if (getline(stream, part1, '/') && getline(stream, part2, '/') && getline(stream, part3, '/')) {
        return part3;
    }
    return "";
}

std::shared_ptr<MachineController> FactoryController::getMachine(const std::string& machineId) const {
    auto it = machines.find(machineId);
    if (it != machines.end()) {
        return it->second;
    }
    return nullptr;
}

const std::unordered_map<std::string, std::shared_ptr<MachineController>>& FactoryController::getMachines() const {
    return machines;
}

nlohmann::json FactoryController::getSensorStates() const {
    nlohmann::json snapshot;

    for (const auto& [id, controller] : machines) {
        auto machine = controller->getMachine();
        if (machine) {
            snapshot[id] = {
                { "sensors", machine->getAllSensorValues() },
                { "state", machine->isRunning() ? "running" : "stopped" }
            };
        }
    }

    return snapshot;
}


