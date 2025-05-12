include(CTest)
enable_testing()

# MachineController tests
add_executable(machine_controller_tests
        ${PROJECT_SOURCE_DIR}/tests/MachineControllerTest.cpp
        ${PROJECT_SOURCE_DIR}/src/MachineController.cpp
)
target_include_directories(machine_controller_tests PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(machine_controller_tests PRIVATE Catch2::Catch2WithMain)
add_test(NAME MachineControllerTests COMMAND machine_controller_tests)

# TemperatureSensor tests
add_executable(temperature_sensor_tests
        ${PROJECT_SOURCE_DIR}/tests/TemperatureSensorTest.cpp
        ${PROJECT_SOURCE_DIR}/src/MachineController.cpp
)
target_include_directories(temperature_sensor_tests PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(temperature_sensor_tests PRIVATE Catch2::Catch2WithMain nlohmann_json::nlohmann_json)
add_test(NAME TemperatureSensorTests COMMAND temperature_sensor_tests)

# FactoryController tests
add_executable(factory_controller_tests
        ${PROJECT_SOURCE_DIR}/tests/FactoryControllerTest.cpp
        ${PROJECT_SOURCE_DIR}/src/FactoryController.cpp
        ${PROJECT_SOURCE_DIR}/src/MachineController.cpp
)
target_include_directories(factory_controller_tests PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(factory_controller_tests PRIVATE Catch2::Catch2WithMain nlohmann_json::nlohmann_json)
add_test(NAME FactoryControllerTests COMMAND factory_controller_tests)

# Optional meta target to build all test binaries
add_custom_target(all_tests
        DEPENDS machine_controller_tests temperature_sensor_tests factory_controller_tests
)