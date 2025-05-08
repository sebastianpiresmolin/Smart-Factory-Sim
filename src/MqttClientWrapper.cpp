#include "MqttClientWrapper.h"

MqttClientWrapper::MqttClientWrapper(const std::string& server, const std::string& clientId)
    : client(server, clientId) {}

void MqttClientWrapper::connect() {
    mqtt::connect_options options;
    client.connect(options)->wait();
}

void MqttClientWrapper::disconnect() {
    client.disconnect()->wait();
}

void MqttClientWrapper::subscribe(const std::string& topic) {
    client.subscribe(topic, 1)->wait();
}

void MqttClientWrapper::publish(const std::string& topic, const std::string& message) {
    client.publish(topic, message, 1, false)->wait();
}

void MqttClientWrapper::setCallback(mqtt::callback& cb) {
    client.set_callback(cb);
}
