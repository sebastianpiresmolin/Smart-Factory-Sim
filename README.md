![CodeRabbit Pull Request Reviews](https://img.shields.io/coderabbit/prs/github/sebastianpiresmolin/Smart-Factory-Sim?utm_source=oss&utm_medium=github&utm_campaign=sebastianpiresmolin%2FSmart-Factory-Sim&labelColor=171717&color=FF570A&link=https%3A%2F%2Fcoderabbit.ai&label=CodeRabbit+Reviews)
# Smart Factory Simulator
**This project is a work in progress**

A comprehensive smart factory simulator built with C++, MQTT, and Node-RED that demonstrates realistic manufacturing environment behaviors through sensor simulation, machine state tracking, and production metrics.


![image](https://github.com/user-attachments/assets/9c9f715c-992b-4750-93cb-465e1ad2b814)

## Overview
The Smart Factory Simulator creates a virtual manufacturing environment that generates realistic sensor data, machine states, and production metrics. It's designed to simulate multiple machines in a factory setting with individual temperature, vibration, and production count monitoring. The system uses MQTT for communication, making it ideal for integration with other manufacturing systems, dashboards, or Manufacturing Execution Systems (MES).

This project serves as both a learning tool and a test bed for industrial IoT and industry concepts, with practical implementations of:

- Machine state monitoring and control
- Sensor data simulation with realistic variations
- Production metrics tracking (items produced, materials lost)
- Real-time data publishing via MQTT
- Factory state persistence

## Installation and Setup

Prerequisites:
- Docker
- Git

## Quick Start

Clone the repository:

`git clone https://github.com/sebastianpiresmolin/Smart-Factory-Sim.git`

`cd Smart-Factory-Sim`

Build the Docker images:

`docker compose build`

Start the simulator:

`docker compose up`

Access Node-RED at: `http://localhost:1880`

## Architecture


![image](https://github.com/user-attachments/assets/6b09d7d4-e481-42b1-bae4-eba17f3944f0)


The simulator consists of several key components:

- C++ Factory Simulator - Core simulation engine that models machines, sensors, and production
- MQTT Broker - Message broker (Mosquitto) for communication
- Node-RED - Visual editor for creating test workflows and visualizing data
- Storage Volume - Persistent storage for factory state snapshots


## Features


- Dynamic Machine Simulation: Creates and manages multiple virtual machines
  
- Realistic Sensor Behavior: Simulates temperature and vibration with configurable patterns
  
- Production Tracking: Monitors items produced and materials lost

- Machine States: Tracks running/stopped states with appropriate behavior

- MQTT Communication: Publishes all state changes and sensor readings via MQTT


## Core Components

**Machine Controller**

Manages individual machine instances with their associated sensors:

- Handles temperature and vibration data

- Manages machine operating state

- Tracks production and reject counts

- Detects anomalies (overheating, excessive vibration)

- Factory Controller

**Orchestrates the entire factory environment:**

- Manages multiple machine instances

- Handles MQTT message routing

- Saves and restores factory state

- Generates factory-level metrics and snapshots

- MQTT Client Wrapper

**Provides robust MQTT communication:**

- Maintains connection with MQTT broker

- Implements heartbeat mechanism

- Handles asynchronous message publishing

- Manages topic subscriptions and message routing

- MQTT Topics

**The simulator uses a structured MQTT topic hierarchy:**

- factory/machine{id}/temp - Temperature sensor readings

- factory/machine{id}/vibration - Vibration sensor readings

- factory/machine{id}/state - Machine state changes (start/stop)

- factory/machine{id}/produced - Production count updates

- factory/heartbeat - System heartbeat messages

- factory/state_snapshot - Complete factory state snapshots

- State Persistence: Saves and restores factory state between runs

- Automatic Recovery: Handles connection issues and broker interruptions
