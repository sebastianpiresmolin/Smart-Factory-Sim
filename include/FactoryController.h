#ifndef FACTORYCONTROLLER_H
#define FACTORYCONTROLLER_H

#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "MachineController.h"

class FactoryController {
public:
    void handleMessage(const std::string& topic, const std::string& payload);

private:
    std::unordered_map<std::string, std::shared_ptr<MachineController>> machines;

    std::string parseMachineId(const std::string& topic);
    std::string parseSensorType(const std::string& topic);
};




#endif //FACTORYCONTROLLER_H
