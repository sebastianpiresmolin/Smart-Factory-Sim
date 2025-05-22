#include "MqttClientWrapper.h"
#include <iostream>
#include <queue>

MqttClientWrapper::MqttClientWrapper(const std::string &address, const std::string &clientId, const std::string &topic)
    : client(address, clientId), topic(topic), publishRunning(false) {

    // Set callback
    client.set_callback(*this);

    // Configure connection options
    connOpts.set_keep_alive_interval(30); // 30 seconds keep-alive
    connOpts.set_clean_session(true);

    // We'll handle reconnect manually for more control
    connOpts.set_automatic_reconnect(false);
}

void MqttClientWrapper::publishWorker() {
    std::cout << "Publish worker thread started" << std::endl;

    while (publishRunning) {
        std::pair<std::string, std::string> item;
        bool hasItem = false;

        {
            std::lock_guard<std::mutex> lock(publishMutex);
            if (!publishQueue.empty()) {
                item = publishQueue.front();
                publishQueue.pop();
                hasItem = true;
            }
        }

        if (hasItem) {
            try {
                if (client.is_connected()) {
                    auto msg = mqtt::make_message(item.first, item.second);
                    msg->set_qos(0);
                    client.publish(msg);
                    std::cout << "Published to " << item.first << " (" << item.second.size() << " bytes)" << std::endl;
                }
            }
            catch (const std::exception &e) {
                std::cerr << "Error in publish worker: " << e.what() << std::endl;
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    std::cout << "Publish worker thread stopped" << std::endl;
}

MqttClientWrapper::~MqttClientWrapper() {
    stop();
}

void MqttClientWrapper::setController(FactoryController *controller) {
    std::lock_guard<std::mutex> lock(mutex);
    factoryController = controller;
}

void MqttClientWrapper::start() {
    try {
        // Connect to the broker
        std::cout << "Connecting to MQTT broker..." << std::endl;
        mqtt::token_ptr conntok = client.connect(connOpts);
        conntok->wait();

        if (!client.is_connected()) {
            std::cerr << "Failed to connect to MQTT broker!" << std::endl;
            return;
        }

        // Subscribe to topics
        client.subscribe(topic, 1)->wait();
        std::cout << "Subscribed to topic: " << topic << std::endl;

        // Start keep-alive thread
        running = true;
        keep_alive_thread = std::thread(&MqttClientWrapper::keep_alive_loop, this);

        // Start publish worker thread
        publishRunning = true;
        publishThread = std::thread(&MqttClientWrapper::publishWorker, this);
    }
    catch (const mqtt::exception& e) {
        std::cerr << "Error starting MQTT client: " << e.what() << std::endl;
    }
}

void MqttClientWrapper::keep_alive_loop() {
    std::cout << "Starting MQTT keep-alive thread" << std::endl;

    // Use shorter interval than the broker's timeout
    const int PING_INTERVAL_MS = 5000; // 5 seconds

    while (running) {
        try {
            if (!client.is_connected()) {
                std::cout << "*** CONNECTION LOST! Attempting to reconnect... ***" << std::endl;

                try {
                    // Reconnect with same options
                    client.connect(connOpts)->wait();

                    if (client.is_connected()) {
                        std::cout << "Successfully reconnected!" << std::endl;
                        // Resubscribe
                        client.subscribe(topic, 1)->wait();
                    }
                }
                catch (const mqtt::exception& e) {
                    std::cerr << "Reconnection failed: " << e.what() << std::endl;
                }
            }
            else {
                // Send explicit ping to keep connection alive
                heartbeat_counter++;

                // Three different ways to maintain connection:

                // 1. Standard MQTT ping request (most reliable)
                client.publish("$SYS/ping", "", 0, false);

                // 2. Custom heartbeat message (for visibility)
                std::string payload = "ping-" + std::to_string(heartbeat_counter);
                client.publish("factory/heartbeat", payload, 0, false);

                // 3. Force the underlying client to send a PINGREQ
                // This line is critical - it tells the MQTT client to send a PINGREQ packet
                client.get_client_id(); // Any API call forces client activity

                std::cout << ">>> HEARTBEAT #" << heartbeat_counter
                          << " SENT (Connection: "
                          << (client.is_connected() ? "ALIVE" : "DEAD")
                          << ") <<<" << std::endl;
            }
        }
        catch (const mqtt::exception& e) {
            std::cerr << "Error in keep-alive loop: " << e.what() << std::endl;
        }
        catch (const std::exception& e) {
            std::cerr << "General error in keep-alive loop: " << e.what() << std::endl;
        }

        // Sleep for the ping interval
        std::this_thread::sleep_for(std::chrono::milliseconds(PING_INTERVAL_MS));
    }

    std::cout << "MQTT keep-alive thread stopped" << std::endl;
}

void MqttClientWrapper::stop() {
    running = false;
    publishRunning = false;

    if (keep_alive_thread.joinable()) {
        keep_alive_thread.join();
    }

    if (publishThread.joinable()) {
        publishThread.join();
    }

    if (client.is_connected()) {
        try {
            client.disconnect()->wait();
            std::cout << "Disconnected from MQTT broker" << std::endl;
        }
        catch (const mqtt::exception& e) {
            std::cerr << "Error during disconnect: " << e.what() << std::endl;
        }
    }
}

void MqttClientWrapper::message_arrived(mqtt::const_message_ptr msg) {
    try {
        std::cout << "Received message on topic: " << msg->get_topic()
                  << " payload: " << msg->get_payload_str() << std::endl;

        // Skip heartbeat messages
        if (msg->get_topic() == "factory/heartbeat" || msg->get_topic() == "$SYS/ping") {
            return;
        }

        if (factoryController) {
            factoryController->handleMessage(msg->get_topic(), msg->get_payload());
            auto snapshot = factoryController->getSensorStates();
            publishSensorData("factory/state_snapshot", snapshot.dump(2));
        }
    } catch (const std::exception& e) {
        std::cerr << "[ERROR] Error processing message: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "[ERROR] Unknown error processing message!" << std::endl;
    }
}

void MqttClientWrapper::publishSensorData(const std::string &topic, const std::string &message) {
    std::cerr << "Entering publishSensorData" << std::endl;

    // Check payload size and warn if large
    if (message.size() > 10000) {
        std::cerr << "WARNING: Large payload size (" << message.size()
                  << " bytes) might cause issues" << std::endl;
    }

    // Add message to publish queue
    std::lock_guard<std::mutex> lock(publishMutex);
    publishQueue.push(std::make_pair(topic, message));
    std::cerr << "Message queued for publishing to " << topic << std::endl;
}

void MqttClientWrapper::connection_lost(const std::string& cause) {
    std::cerr << "Connection lost! Cause: " << cause << std::endl;
}

void MqttClientWrapper::delivery_complete(mqtt::delivery_token_ptr token) {
}