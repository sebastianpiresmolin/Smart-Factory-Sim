#include "MachineController.h"
#include <iostream>
#include <random>
#include <nlohmann/json.hpp>

MachineController::MachineController(const std::string &id, FactoryController* parent)
    : machineId(id), machine(std::make_shared<Machine>(id)), parentController(parent) {
}

void MachineController::handleSensor(const std::string &sensorType, const std::string &payload) {
    if (sensorType == "temp") {
        handleTemperature(payload);
    } else if (sensorType == "vibration") {
        handleVibration(payload);
    } else if (sensorType == "state") {
        handleState(payload);
    } else if (sensorType == "produced") {
        handleProduced(payload);
    } else {
        std::cout << "[" << machineId << "] Unhandled sensor type: " << sensorType << "\n";
    }
}

void MachineController::handleTemperature(const std::string &payload) {
    try {
        if (!machine->isRunning()) {
            std::cout << "[" << machineId << "] Machine is OFF. Ignoring sensor data.\n";
            return;
        }

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
            // Save after temperature handled
            if (parentController) parentController->saveSnapshot("/app/storage/factory_snapshot.json");
        }
    } catch (const std::exception &ex) {
        std::cerr << "[" << machineId << "] Failed to parse temperature JSON: " << ex.what() << "\n";
    }
}

void MachineController::handleVibration(const std::string &payload) {
    try {
        if (!machine->isRunning()) {
            std::cout << "[" << machineId << "] Machine is OFF. Ignoring sensor data.\n";
            return;
        }

        auto json = nlohmann::json::parse(payload);
        if (json.contains("vibration")) {
            double vibration = json["vibration"];
            machine->setSensorValue("vibration", vibration);

            std::cout << "[" << machineId << "] Live Vibration: " << vibration << "\n";

            if (machine->isExcessiveVibration()) {
                std::cout << "[" << machineId << "] WARNING: Excessive vibration detected!\n";
            } else {
                std::cout << "[" << machineId << "] Vibration level is normal.\n";
            }
            // Save after vibration handled
            if (parentController) parentController->saveSnapshot("/app/storage/factory_snapshot.json");
        }
    } catch (const std::exception &ex) {
        std::cerr << "[" << machineId << "] Failed to parse vibration JSON: " << ex.what() << "\n";
    }
}

void MachineController::handleState(const std::string &payload) {
    try {
        auto json = nlohmann::json::parse(payload);
        if (json.contains("state")) {
            std::string state = json["state"];
            machine->setSensorValue("state", state == "start" ? 1.0 : 0.0); // Store numeric form

            if (machine->isRunning()) {
                std::cout << "[" << machineId << "] Machine is RUNNING\n";
            } else {
                std::cout << "[" << machineId << "] Machine is STOPPED\n";
            }
            // Save after state handled
            if (parentController) parentController->saveSnapshot("/app/storage/factory_snapshot.json");
        }
    } catch (const std::exception &ex) {
        std::cerr << "[" << machineId << "] Failed to parse state JSON: " << ex.what() << "\n";
    }
}

void MachineController::handleProduced(const std::string &payload) {
    if (!machine->isRunning()) {
        return;
    }

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(1, 100);
    int chance = distr(gen);

    if (chance >= 1 && chance <= 5) {
        try {
            machine->incrementLostMaterials();
            if (parentController) parentController->saveSnapshot("/app/storage/factory_snapshot.json");
        } catch (const std::exception &ex) {
            std::cerr << "[" << machineId << "] Failed to parse produced JSON: " << ex.what() << "\n";
        }
    } else {
        try {
            machine->incrementProduced();
            if (parentController) parentController->saveSnapshot("/app/storage/factory_snapshot.json");
        } catch (const std::exception &ex) {
            std::cerr << "[" << machineId << "] Failed to parse produced JSON: " << ex.what() << "\n";
        }
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
    return machine->isOverheating();
}

bool MachineController::isTooCold() const {
    return machine->isTooCold();
}

bool MachineController::isExcessiveVibration() const {
    return machine->isExcessiveVibration();
}