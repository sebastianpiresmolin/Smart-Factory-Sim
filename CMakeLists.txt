cmake_minimum_required(VERSION 3.10)

project(SmartFactorySim VERSION 0.1 LANGUAGES CXX)

# Use C++17
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable testing
include(CTest)
enable_testing()

# vcpkg dependencies
find_package(nlohmann_json CONFIG REQUIRED)

# Third-party paths
set(PAHO_CPP_BUILD ${CMAKE_SOURCE_DIR}/third_party/paho.mqtt.cpp/build)
set(PAHO_C_INSTALL ${CMAKE_SOURCE_DIR}/third_party/paho.mqtt.c/install)

# Include directories
include_directories(
        ${PROJECT_SOURCE_DIR}/include
        ${PROJECT_SOURCE_DIR}/third_party/paho.mqtt.cpp/include
        ${PROJECT_SOURCE_DIR}/third_party/paho.mqtt.c/src
)

# Link directories
link_directories(
        ${PAHO_CPP_BUILD}/src/Release
        ${PAHO_C_INSTALL}/lib
)

# Main application executable
add_executable(smart_factory_sim
        src/main.cpp
        src/FactoryController.cpp
        src/MqttClientWrapper.cpp
        src/MachineController.cpp
        include/FactoryController.h
        include/MachineController.h
        include/MqttClientWrapper.h
        include/SensorMessage.h
        src/Machine.cpp
        include/Machine.h
)

target_link_libraries(smart_factory_sim PRIVATE
        paho-mqttpp3
        paho-mqtt3a
        nlohmann_json::nlohmann_json
)

add_subdirectory(tests)
