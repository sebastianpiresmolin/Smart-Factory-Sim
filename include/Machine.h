#ifndef MACHINE_H
#define MACHINE_H

#pragma once
#include <string>
#include <unordered_map>
#include <optional>

class Machine {
public:
    explicit Machine(const std::string& id);

    void setSensorValue(const std::string& type, double value);
    std::optional<double> getSensorValue(const std::string& type) const;
    std::unordered_map<std::string, double> getAllSensorValues() const;

    bool isOverheating() const;
    bool isTooCold() const;

    const std::string& getId() const;

private:
    std::string id;
    std::unordered_map<std::string, double> sensors;

    bool overheating = false;
    bool tooCold = false;

    void evaluateTemperature(double value);
};

#endif // MACHINE_H

