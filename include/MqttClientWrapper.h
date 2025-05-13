#ifndef MQTTCLIENTWRAPPER_H
#define MQTTCLIENTWRAPPER_H

#pragma once

#include <mqtt/async_client.h>
#include "FactoryController.h"

class MqttClientWrapper : public virtual mqtt::callback {
public:
    MqttClientWrapper(const std::string& address, const std::string& clientId, const std::string& topic);

    void start();
    void stop();
    void setController(FactoryController* controller);  // 👈 Add this

    void message_arrived(mqtt::const_message_ptr msg) override;

    void publishSensorData(const std::string& topic, const std::string& message);

private:
    mqtt::async_client client;
    mqtt::connect_options connOpts;
    std::string topic;
    FactoryController* factoryController = nullptr;  // 👈 Use pointer
};



#endif //MQTTCLIENTWRAPPER_H
