#include "Machine.h"

#include <nlohmann/json.hpp>

Machine::Machine(const std::string& id) : id(id) {}

void Machine::start() {
    running = true;
}

void Machine::stop() {
    running = false;
}

void Machine::incrementProduced() {
    ++totalProduced;
}

void Machine::incrementLostMaterials() { ++lostMaterials; }
int Machine::getLostMaterials() const { return lostMaterials; }

int Machine::getTotalProduced() const {
    return totalProduced;
}

bool Machine::isRunning() const {
    auto val = getSensorValue("state");
    return val.has_value() && val.value() > 0.5;
}

void Machine::setSensorValue(const std::string& type, double value) {

    if (!running) return;
    sensors[type] = value;

    if (type == "temp") {
        evaluateTemperature(value);
    }
}

void Machine::evaluateTemperature(double value) {
    tooCold = value < 10.0;
    overheating = value > 80.0;
}

std::optional<double> Machine::getSensorValue(const std::string& type) const {
    auto it = sensors.find(type);
    if (it != sensors.end()) {
        return it->second;
    }
    return std::nullopt;
}

std::unordered_map<std::string, double> Machine::getAllSensorValues() const {
    return sensors;
}

nlohmann::json Machine::toJson() const {
    return {
            {"sensors", sensors},
            {"state", running ? "running" : "stopped"},
            {"totalProduced", totalProduced},
            {"lostMaterials", lostMaterials}
    };
}

void Machine::fromJson(const nlohmann::json& j) {
    if (j.contains("sensors")) sensors = j.at("sensors").get<std::unordered_map<std::string, double>>();
    running = (j.value("state", "stopped") == "running");
    totalProduced = j.value("totalProduced", 0);
    lostMaterials = j.value("lostMaterials", 0);
}

bool Machine::isExcessiveVibration() const {
    auto it = sensors.find("vibration");
    return it != sensors.end() && it->second > 5.0;
}


bool Machine::isOverheating() const {
    return overheating;
}

bool Machine::isTooCold() const {
    return tooCold;
}

const std::string& Machine::getId() const {
    return id;
}


