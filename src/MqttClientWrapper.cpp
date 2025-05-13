#include "MqttClientWrapper.h"
#include <iostream>

MqttClientWrapper::MqttClientWrapper(const std::string &address, const std::string &clientId, const std::string &topic)
    : client(address, clientId), topic(topic) {
    client.set_callback(*this);
}

void MqttClientWrapper::setController(FactoryController *controller) {
    factoryController = controller;
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
    if (factoryController) {
        factoryController->handleMessage(msg->get_topic(), msg->to_string());

        auto snapshot = factoryController->getSensorStates();
        publishSensorData("factory/state_snapshot", snapshot.dump(2)); // pretty-print
    }
}

void MqttClientWrapper::publishSensorData(const std::string &topic, const std::string &message) {
    if (client.is_connected()) {
        try {
            auto msg = mqtt::make_message(topic, message);
            client.publish(msg);
        } catch (const mqtt::exception &e) {
            std::cerr << "Error publishing to topic " << topic << ": " << e.what() << std::endl;
        }
    }
}

