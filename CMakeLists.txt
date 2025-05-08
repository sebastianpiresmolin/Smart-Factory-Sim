cmake_minimum_required(VERSION 3.10)

project(SmartFactorySim VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(
        "C:/Users/Sebastian Molin/SmartFactoryPahoCpp/include"
        "C:/Users/Sebastian Molin/paho-install/include"
)

link_directories(
        "C:/Users/Sebastian Molin/SmartFactoryPahoCpp/cppbuild/src/Release"
        "C:/Users/Sebastian Molin/paho-install/lib"
)

add_executable(smart_factory_sim
        src/main.cpp
)

target_link_libraries(smart_factory_sim PRIVATE
        paho-mqttpp3-static
        paho-mqtt3a-static
        ws2_32
)


target_include_directories(smart_factory_sim PRIVATE include)
