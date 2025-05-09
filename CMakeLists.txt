cmake_minimum_required(VERSION 3.10)

project(SmartFactorySim VERSION 0.1 LANGUAGES CXX)

find_package(nlohmann_json CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(PAHO_C_INSTALL "${CMAKE_SOURCE_DIR}/third-party/paho.mqtt.c/install")
set(PAHO_CPP_BUILD "${CMAKE_SOURCE_DIR}/third-party/paho.mqtt.cpp/build")

link_directories(
        ${PAHO_C_INSTALL}/lib
        ${PAHO_CPP_BUILD}/src/Release
)


add_executable(smart_factory_sim
        src/main.cpp
        src/FactoryController.cpp
        src/MqttClientWrapper.cpp
        include/SensorMessage.h
        include/MachineController.h
        include/MqttClientWrapper.h
        include/FactoryController.h
        src/MachineController.cpp
)

target_link_libraries(smart_factory_sim PRIVATE
        paho-mqttpp3-static
        paho-mqtt3a-static
        ws2_32
        nlohmann_json::nlohmann_json
)


target_include_directories(smart_factory_sim PRIVATE include)
