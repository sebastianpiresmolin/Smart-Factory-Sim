#include "FactoryController.h"
#include "MqttClientWrapper.h"
#include <nlohmann/json.hpp>
#include <iostream>

using json = nlohmann::json;

class TempCallback : public mqtt::callback {
    MqttClientWrapper& client;

public:
    TempCallback(MqttClientWrapper& clientRef) : client(clientRef) {}

    void message_arrived(mqtt::const_message_ptr msg) override {
        try {
            auto payload = json::parse(msg->to_string());
            if (payload.contains("temp") && payload["temp"].is_number()) {
                double temp = payload["temp"];
                std::cout << "[Sensor] Temp: " << temp << "\n";

                if (temp > 25.0) {
                    std::cout << "⚠️ Temp high. Sending command: FAN ON\n";
                    client.publish("factory/fan", "ON");
                }
            }
        } catch (const std::exception& e) {
            std::cerr << "JSON parse error: " << e.what() << "\n";
        }
    }
};

FactoryController::FactoryController() {}

void FactoryController::run() {
    MqttClientWrapper mqttClient("tcp://localhost:1883", "factory_controller");

    TempCallback cb(mqttClient);
    mqttClient.setCallback(cb);
    mqttClient.connect();
    mqttClient.subscribe("factory/temperature");

    std::cout << "[MQTT] Subscribed. Press Enter to exit.\n";
    std::cin.get();
    mqttClient.disconnect();
}
