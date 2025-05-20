#include "Machine.h"

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


