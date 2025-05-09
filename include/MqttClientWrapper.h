#ifndef MQTTCLIENTWRAPPER_H
#define MQTTCLIENTWRAPPER_H

#pragma once

#include <mqtt/async_client.h>
#include "FactoryController.h"

class FactoryController;

class MqttClientWrapper : public virtual mqtt::callback {
public:
    MqttClientWrapper(const std::string& address, const std::string& clientId, const std::string& topic);

    void start();
    void stop();

    // ✅ Setter for injecting a mock/test instance
    void setController(FactoryController* controller);
    void message_arrived(mqtt::const_message_ptr msg) override;

private:
    mqtt::async_client client;
    mqtt::connect_options connOpts;
    std::string topic;

    FactoryController* factoryController = nullptr;

};



#endif //MQTTCLIENTWRAPPER_H
