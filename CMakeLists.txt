cmake_minimum_required(VERSION 3.10)

project(SmartFactorySim VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(smart_factory_sim
        src/main.cpp
)

target_include_directories(smart_factory_sim PRIVATE include)