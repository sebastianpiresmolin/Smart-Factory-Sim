#ifndef MACHINE_H
#define MACHINE_H

#pragma once
#include <string>
#include <unordered_map>
#include <optional>
#include <nlohmann/json_fwd.hpp>

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
    void incrementLostMaterials();
    int getLostMaterials() const;
    int getTotalProduced() const;

    bool isOverheating() const;
    bool isTooCold() const;

    bool isExcessiveVibration() const;


    const std::string& getId() const;

    nlohmann::json toJson() const;
    void fromJson(const nlohmann::json& j);


private:
    std::string id;
    std::unordered_map<std::string, double> sensors;

    bool running = true;

    bool overheating = false;
    bool tooCold = false;

    void evaluateTemperature(double value);
    int totalProduced = 0;
    int lostMaterials = 0;
};

#endif // MACHINE_H

