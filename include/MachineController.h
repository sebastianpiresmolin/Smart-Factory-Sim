﻿#ifndef MACHINECONTROLLER_H
#define MACHINECONTROLLER_H

#pragma once
#include <string>
#include <memory>
#include "Machine.h"
#include "FactoryController.h"

class FactoryController;

class MachineController {
    FactoryController* parentController = nullptr;
public:
    explicit MachineController(const std::string& id, FactoryController* parent = nullptr);
    void setParentController(FactoryController* parent) { parentController = parent; }

    void handleSensor(const std::string& sensorType, const std::string& payload);

    void handleProduced(const std::string& payload);

    std::shared_ptr<Machine> getMachine() const;
    std::string getMachineId() const;

    bool isOverheating() const;
    bool isTooCold() const;

    bool isExcessiveVibration() const;

    std::unordered_map<std::string, double> getAllSensorValues() const;

private:
    std::string machineId;
    std::shared_ptr<Machine> machine;

    void handleTemperature(const std::string& payload);
    void handleVibration(const std::string& payload);
    void handleState(const std::string& payload);
};

#endif // MACHINECONTROLLER_H
