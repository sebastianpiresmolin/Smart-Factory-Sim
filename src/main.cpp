#include "MqttClientWrapper.h"
#include "FactoryController.h"
#include <iostream>

const std::string SNAPSHOT_FILE = "../storage/factory_snapshot.json";

int main() {
    auto factoryController = std::make_unique<FactoryController>();

    try {
        factoryController->loadSnapshot(SNAPSHOT_FILE);
        std::cout << "Factory state restored successfully.\n";
    } catch (const std::exception &e) {
        std::cerr << "Failed to restore factory state: " << e.what() << std::endl;
    }

    MqttClientWrapper mqtt("tcp://localhost:1883", "smart_factory_simulator", "factory/+/+");
    mqtt.setController(factoryController.get());

    try {
        mqtt.start();
        std::cout << "Waiting for messages. Press Enter to quit...\n";
        std::cin.get();
        mqtt.stop();
    } catch (const mqtt::exception &e) {
        std::cerr << "MQTT error: " << e.what() << std::endl;
        return 1;
    }

    try {
        factoryController->saveSnapshot(SNAPSHOT_FILE);
        std::cout << "Factory state saved successfully.\n";
    } catch (const std::exception &e) {
        std::cerr << "Failed to save factory state: " << e.what() << std::endl;
    }

    return 0;
}
