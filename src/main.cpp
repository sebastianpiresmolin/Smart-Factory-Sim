#include "MqttClientWrapper.h"
#include "FactoryController.h"
#include <iostream>

int main() {
    auto factoryController = std::make_unique<FactoryController>();
    factoryController->loadSnapshot("../storage/factory_snapshot.json");

    MqttClientWrapper mqtt("tcp://localhost:1883", "smart_factory_simulator", "factory/+/+");
    mqtt.setController(factoryController.get());

    try {
        mqtt.start();
        std::cout << "Waiting for messages. Press Enter to quit...\n";
        std::cin.get();
        mqtt.stop();
    } catch (const mqtt::exception& e) {
        std::cerr << "MQTT error: " << e.what() << std::endl;
        return 1;
    }

    factoryController->saveSnapshot("../storage/factory_snapshot.json");

    return 0;
}