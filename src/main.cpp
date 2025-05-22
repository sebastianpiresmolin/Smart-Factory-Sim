#include "MqttClientWrapper.h"
#include "FactoryController.h"
#include <iostream>
#include <filesystem>

const std::string SNAPSHOT_FILE = "../storage/factory_snapshot.json";

int main() {
    try {
        std::cout << "=== Main started ===" << std::endl;
        std::string address = "tcp://mosquitto:1883";
        std::string clientId = "smart_factory_simulator";
        std::string topic = "factory/#";

        auto factoryController = std::make_unique<FactoryController>();

        std::filesystem::create_directories(std::filesystem::path(SNAPSHOT_FILE).parent_path());

        try {
            factoryController->loadSnapshot(SNAPSHOT_FILE);
            std::cout << "Factory state restored successfully.\n";
        } catch (const std::exception &e) {
            std::cerr << "Failed to restore factory state: " << e.what() << std::endl;
        }

        MqttClientWrapper mqtt(address, clientId, topic);
        mqtt.setController(factoryController.get());

        mqtt.start();

        std::cout << "Waiting for messages. Press Ctrl+C to quit..." << std::endl;
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }
        std::cout << "=== Main exiting ===" << std::endl;
        mqtt.stop();
        return 0;
    } catch (const std::exception& e) {
        std::cerr << "FATAL exception in main: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "FATAL unknown exception in main!" << std::endl;
        return 2;
    }
}
