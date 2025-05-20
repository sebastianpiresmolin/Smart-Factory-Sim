#ifndef MACHINE_H
#define MACHINE_H

#pragma once
#include <string>
#include <unordered_map>
#include <optional>

class Machine {
public:
    explicit Machine(const std::string& id);

    void start();
    void stop();
    bool isRunning() const;

    void setSensorValue(const std::string& type, double value);
    std::optional<double> getSensorValue(const std::string& type) const;
    std::unordered_map<std::string, double> getAllSensorValues() const;

    void incrementProduced();
    int getTotalProduced() const;

    bool isOverheating() const;
    bool isTooCold() const;

    bool isExcessiveVibration() const;


    const std::string& getId() const;

private:
    std::string id;
    std::unordered_map<std::string, double> sensors;

    bool running = true;

    bool overheating = false;
    bool tooCold = false;

    void evaluateTemperature(double value);
    int totalProduced = 0;
};

#endif // MACHINE_H

