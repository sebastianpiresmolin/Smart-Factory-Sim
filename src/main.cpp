#include "MqttClientWrapper.h"

int main() {
    MqttClientWrapper mqtt("tcp://localhost:1883", "smart_factory_simulator", "factory/+/+");

    try {
        mqtt.start();
        std::cout << "Waiting for messages. Press Enter to quit...\n";
        std::cin.get();
        mqtt.stop();
    } catch (const mqtt::exception& e) {
        std::cerr << "MQTT error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
