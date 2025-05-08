#ifndef SENSORMESSAGE_H
#define SENSORMESSAGE_H

#pragma once
#include <string>

struct SensorMessage {
    std::string machineId;
    std::string sensorType;
    std::string payload;

    static SensorMessage parse(const std::string& topic, const std::string& payload) {
        //factory/<machineId>/<sensorType>
        SensorMessage msg;
        msg.payload = payload;
        size_t firstSlash = topic.find('/');
        size_t secondSlash = topic.find('/', firstSlash + 1);

        if (firstSlash != std::string::npos && secondSlash != std::string::npos) {
            msg.machineId = topic.substr(firstSlash + 1, secondSlash - firstSlash - 1);
            msg.sensorType = topic.substr(secondSlash + 1);
        }

        return msg;
    }
};


#endif //SENSORMESSAGE_H
