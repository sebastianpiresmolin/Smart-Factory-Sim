#ifndef FACTORYCONTROLLER_H
#define FACTORYCONTROLLER_H

#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include <nlohmann/json.hpp>

#include "MachineController.h"

class FactoryController {
public:
    virtual ~FactoryController() = default;

    virtual void handleMessage(const std::string& topic, const std::string& payload);

    nlohmann::json getSensorStates() const;


    std::shared_ptr<MachineController> getMachine(const std::string& machineId) const;
    const std::unordered_map<std::string, std::shared_ptr<MachineController>>& getMachines() const;

    void saveSnapshot(const std::string& filename) const;
    void loadSnapshot(const std::string& filename);

    std::shared_ptr<MachineController> getOrCreateMachine(const std::string& id);

private:
    std::unordered_map<std::string, std::shared_ptr<MachineController>> machines;

    std::string parseMachineId(const std::string& topic);
    std::string parseSensorType(const std::string& topic);

};




#endif //FACTORYCONTROLLER_H
