#include "MqttClientWrapper.h"
#include <iostream>

MqttClientWrapper::MqttClientWrapper(const std::string& address, const std::string& clientId, const std::string& topic)
    : client(address, clientId), topic(topic)
{
    client.set_callback(*this);
}

void MqttClientWrapper::start() {
    client.connect(connOpts)->wait();
    client.subscribe(topic, 1)->wait();
    std::cout << "Subscribed to topic: " << topic << std::endl;
}

void MqttClientWrapper::stop() {
    client.disconnect()->wait();
}

void MqttClientWrapper::message_arrived(mqtt::const_message_ptr msg) {
    factoryController.handleMessage(msg->get_topic(), msg->to_string());
}

