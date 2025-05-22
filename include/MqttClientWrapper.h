#ifndef MQTTCLIENTWRAPPER_H
#define MQTTCLIENTWRAPPER_H

#pragma once

#include <mqtt/async_client.h>
#include <thread>
#include <atomic>
#include <chrono>
#include <mutex>
#include "FactoryController.h"
#include <queue>

class MqttClientWrapper : public virtual mqtt::callback {
public:
    MqttClientWrapper(const std::string& address, const std::string& clientId, const std::string& topic);
    ~MqttClientWrapper();

    void start();
    void stop();
    void setController(FactoryController* controller);

    void message_arrived(mqtt::const_message_ptr msg) override;
    void connection_lost(const std::string& cause) override;
    void delivery_complete(mqtt::delivery_token_ptr token) override;

    void publishSensorData(const std::string& topic, const std::string& message);



private:
    mqtt::async_client client;
    mqtt::connect_options connOpts;
    std::string topic;
    FactoryController* factoryController = nullptr;

    // Thread management
    std::thread keep_alive_thread;
    std::atomic<bool> running{false};
    std::atomic<int> heartbeat_counter{0};
    std::mutex mutex;

    std::mutex publishMutex;
    std::queue<std::pair<std::string, std::string>> publishQueue;
    std::thread publishThread;
    bool publishRunning = false;
    void publishWorker();

    void keep_alive_loop();
};

#endif //MQTTCLIENTWRAPPER_H