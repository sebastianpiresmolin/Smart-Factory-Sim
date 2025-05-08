#include "FactoryController.h"
#include <sstream>
#include <iostream>

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
    std::string segment;
    getline(stream, segment, '/'); // factory
    getline(stream, segment, '/'); // machineId
    return segment;
}

std::string FactoryController::parseSensorType(const std::string& topic) {
    std::istringstream stream(topic);
    std::string segment;
    getline(stream, segment, '/'); // factory
    getline(stream, segment, '/'); // machineId
    getline(stream, segment, '/'); // sensorType
    return segment;
}

