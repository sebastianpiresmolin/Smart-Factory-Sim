FROM ubuntu:22.04

# Install build dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    wget \
    ninja-build \
    pkg-config \
    libssl-dev \
    nlohmann-json3-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY third_party/paho.mqtt.c ./third_party/paho.mqtt.c
COPY third_party/paho.mqtt.cpp ./third_party/paho.mqtt.cpp

WORKDIR /app/third_party/paho.mqtt.c
WORKDIR /app/third_party/paho.mqtt.c
RUN cmake -Bbuild -H. -DPAHO_BUILD_STATIC=ON -DPAHO_BUILD_SHARED=OFF -DPAHO_ENABLE_TESTING=OFF -DPAHO_WITH_SSL=ON -DCMAKE_INSTALL_PREFIX=/usr/local && \
    cmake --build build --target install -- -j$(nproc)

WORKDIR /app/third_party/paho.mqtt.cpp
RUN cmake -Bbuild -H. -DPAHO_MQTT_C_PATH=/usr/local -DPAHO_BUILD_STATIC=ON -DPAHO_BUILD_SHARED=OFF -DCMAKE_INSTALL_PREFIX=/usr/local && \
    cmake --build build --target install -- -j$(nproc)

WORKDIR /app
COPY . .

RUN cmake -Bbuild -H. -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build --target smart_factory_sim -- -j$(nproc)

WORKDIR /app
CMD ["./build/smart_factory_sim"]