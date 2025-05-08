#ifndef MACHINECONTROLLER_H
#define MACHINECONTROLLER_H

#pragma once
#include <string>

class MachineController {
public:
    explicit MachineController(const std::string& id);
    void handleSensor(const std::string& sensorType, const std::string& payload);

private:
    std::string machineId;
    void handleTemperature(const std::string& payload);
};
#endif //MACHINECONTROLLER_H
