cmake_minimum_required(VERSION 3.10)

project(SmartFactorySim VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find JSON lib via vcpkg
find_package(nlohmann_json CONFIG REQUIRED)

# Paths to third-party libs
set(PAHO_CPP_BUILD ${CMAKE_SOURCE_DIR}/third_party/paho.mqtt.cpp/build)
set(PAHO_C_INSTALL ${CMAKE_SOURCE_DIR}/third_party/paho.mqtt.c/install)

# Include paths
include_directories(
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/third_party/paho.mqtt.cpp/include
        ${CMAKE_SOURCE_DIR}/third_party/paho.mqtt.c/src
)

# Link directories
link_directories(
        ${PAHO_CPP_BUILD}/src/Release
        ${PAHO_C_INSTALL}/lib
)

# Main executable
add_executable(smart_factory_sim
        src/main.cpp
        src/FactoryController.cpp
        src/MqttClientWrapper.cpp
        src/MachineController.cpp
        include/FactoryController.h
        include/MachineController.h
        include/MqttClientWrapper.h
        include/SensorMessage.h
)

# Link libraries
target_link_libraries(smart_factory_sim PRIVATE
        paho-mqttpp3-static
        paho-mqtt3a-static
        ws2_32
        nlohmann_json::nlohmann_json
)
