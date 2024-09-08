# **Discovery Driven Numerical, Material, Chromatic Exploration**

# Discovery-Driven Game

This project is an interactive game using an ESP32, various sensors, and RFID technology. The game consists of multiple levels, each with unique challenges that test different skills and knowledge.

## Game Overview

The game is divided into 5 levels, each with increasing complexity:

1. Color Recognition
2. Number Recognition
3. Material Recognition
4. Color and Number Combination
5. Basic Arithmetic

Players interact with the game using RFID cards and various objects, which are detected by different sensors.

## Game Levels

### Level 1: Color Recognition

- Player is asked to place a Red, Green, or Blue RFID card.
- Must complete 2 correct placements to advance.

### Level 2: Number Recognition

- Player is asked to place an RFID card corresponding to numbers 1-6.
- Must complete 2 correct placements to advance.

### Level 3: Material Recognition

- Player is asked to place objects made of different materials:
  - Metal (detected by inductive sensor)
  - Black plastic (detected by combination of ultrasonic and IR sensors)
  - Rigid form (detected by combination of sensors)
  - Wood (detected by RFID)
- Must complete 3 correct placements to advance.

### Level 4: Color and Number Combination

- Player is asked to place specific color-number combination RFID cards.
- Must complete 2 correct placements to advance.

### Level 5: Basic Arithmetic

- Player is presented with simple arithmetic questions.
- Must answer correctly by placing the appropriate RFID card representing the answer.
- Must complete 2 correct answers to finish the game.

## Hardware Components

- ESP32 microcontroller
- MFRC522 RFID reader
- Ultrasonic sensor
- IR sensor
- Inductive sensor
- Servo motor (for gate control)

## Software Features

- Web server for real-time game status updates
- Multi-level game logic
- Random question generation for each level
- Sensor fusion for object detection and classification

## How to Play

1. Connect to the ESP32's Wi-Fi network (SSID: "Discovery Driven", Password: "123456789")
2. Open a web browser and navigate to the ESP32's IP address
3. Follow the instructions displayed on the web page for each level
4. Place the correct RFID card or object on the sensor area
5. The gate will open if the answer is correct, allowing you to proceed
6. Complete all levels to win the game

## Future Improvements

- Add more levels or increase the complexity of existing levels
- Implement a scoring system
- Add sound effects or visual feedback using additional hardware
- Create a mobile app interface for better user experience

## Team Members

- [@Lasitha2001](https://github.com/Lasitha2001)
- [@JehanPinto](https://github.com/JehanPinto)
- [@saknarajapakshe](https://github.com/saknarajapakshe)
- [@gimhanijayamanna](https://github.com/gimhanijayamanna)
- [@VishwaJaya01](https://github.com/VishwaJaya01)
