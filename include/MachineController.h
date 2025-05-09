#ifndef MACHINECONTROLLER_H
#define MACHINECONTROLLER_H

#pragma once
#include <string>

class MachineController {
public:
    explicit MachineController(const std::string& id);
    void handleSensor(const std::string& sensorType, const std::string& payload);

    double getLastTemperature() const;
    bool isOverheating() const;
    std::string getMachineId() const;
    bool isTooCold() const;

private:
    std::string machineId;
    double lastTemperature = 0.0;
    bool overheating = false;
    bool tooCold = false;

    void handleTemperature(const std::string& payload);
};
#endif //MACHINECONTROLLER_H
