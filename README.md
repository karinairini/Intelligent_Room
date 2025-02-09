# Intelligent Room System

## Overview
This project focuses on the development of an intelligent room system using Arduino technology, specifically with an ESP32 development board and the Blynk application for creating a user-friendly interface. The main goal is to enhance the comfort and security of a room by controlling temperature, humidity, air quality, and automatically opening the door when the user is nearby.

## Features

- **Automatic Door Opening**: The system automatically opens the door using an ultrasonic sensor and a servo motor when the user is within a certain distance.
- **Temperature and Humidity Monitoring**: Monitors room temperature and humidity using the DHT11 sensor, with real-time data displayed on the Blynk app.
- **Air Quality Monitoring**: The MQ-135 sensor detects the air quality and triggers the fan to turn on if the air quality falls below 60%.
- **Fan Control**: The fan can be controlled manually via the Blynk app or automatically adjusted based on air quality readings.
- **Alerts**: Emits alerts when the air quality falls below the desired threshold (60%) and other significant changes.

## Components

1. **ESP32 Development Board**: Handles Wi-Fi/Bluetooth communication and system control.
2. **DHT11 Sensor**: Measures temperature and humidity.
3. **MQ-135 Sensor**: Detects air quality and pollutants.
4. **HC-SR04 Ultrasonic Sensor**: Detects distance to control automatic door opening.
5. **SG90 Servo Motor**: Controls the door's automatic opening mechanism.
6. **Piezoelectric Buzzer**: Provides alerts for low air quality.
7. **LEDs**: Visual indicators for system status (green for open door, red for closed door, blue for poor air quality).
8. **Fan**: Adjustable fan to improve air quality, controlled either automatically or manually.
