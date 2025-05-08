#ifndef MQTTCLIENTWRAPPER_H
#define MQTTCLIENTWRAPPER_H

#pragma once
#include <mqtt/async_client.h>
#include <string>

class MqttClientWrapper {
public:
    MqttClientWrapper(const std::string& server, const std::string& clientId);

    void connect();
    void disconnect();
    void subscribe(const std::string& topic);
    void publish(const std::string& topic, const std::string& message);
    void setCallback(mqtt::callback& cb);

private:
    mqtt::async_client client;
};



#endif //MQTTCLIENTWRAPPER_H
