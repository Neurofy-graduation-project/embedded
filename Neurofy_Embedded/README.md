# Neurofy EEG Headband

This project implements Neurofy headband for EEG signal acquisition and transmission system using an ESP32 microcontroller, ADS1220 24-bit Analog-to-Digital Converter (ADC) for signal acquisition, and Bluetooth Low Energy (BLE) for wireless data transmission.

## Overview

The system consists of three main components:
- EEG signal acquisition using ADS1220 ADC
- Signal processing and management on ESP32
- Wireless data transmission using BLE

## Project Structure

```
/src
│   main.cpp              // Main program entry point
│
├── /lib
│   ├── BLE               // Bluetooth Low Energy implementation
│   │   ├── NeurofyBLE.cpp
│   │   ├── NeurofyBLE.h
│   │   └── README.md
│   │
│   └── EEGAcquisition    // EEG signal acquisition implementation
│       ├── EEGAcquisition.cpp
│       ├── EEGAcquisition.h
│       └── README.md
│
└── platformio.ini        // PlatformIO configuration file
```

## Hardware Requirements

- ESP32 Development Board
- ADS1220 ADC
- EEG electrodes and associated hardware

## Dependencies

- ADS1220_WE library (version 1.0.15)
- ESP32 BLE libraries

## Pin Configuration

| ADS1220 Pins | ESP32 Pins     | Notes                                      |
| :----------: | :------------: | :----------------------------------------- |
| VDD          | 3.3V           | ADS1220 operates between 2.3V and 5.5V     |
| GND          | GND            |                                            |
| SCLK         | GPIO 18 (SCK)  |                                            |
| CLK          | GND            |                                            |
| MOSI         | GPIO 23 (MOSI) | Sending data to the ADS1220                |
| MISO         | GPIO 19 (MISO) | Receiving data from the ADS1220            |
| DRDY         | GPIO 22        | Checking if data is ready for transmission |
| CS           | GPIO 5         | Selecting the ADS1220 for communication    |

## Hardware Wiring

The hardware wiring of Neurofy headband is illustrated below.

![Local Image](assets/images/Circuit Wiring - Neurofy.png)

### Note: 
The voltages to be measured need to be between negative VREF + 0.2 V and positive VREF -0.2 V if PGA is enabled.

## ADS1220 Configuration

### 1. ADS1220 Object:

ADS1220 object can be created in 2 ways:

1. 
```cpp
ADS1220_WE ads = ADS1220_WE(ADS1220_CS_PIN, ADS1220_DRDY_PIN);
```
2. Or you can also pass the SPI object as reference like the following */
```cpp
ADS1220_WE ads = ADS1220_WE(&SPI, ADS1220_CS_PIN, ADS1220_DRDY_PIN);
```

### 2. ADS1220 Mode:

You set the channels to be measured with `setCompareChannels();`.
You can choose the following parameters:
 | Parameter                 |     Pos. Input    |   Neg. Input   |     Comment    |
 | :-----------------------: | :---------------: | :------------: | :------------: |
 | ADS1220_MUX_0_1           |         AIN0      |     AIN1       |  |
 | ADS1220_MUX_0_2           |         AIN0      |     AIN2       | |
 | ADS1220_MUX_0_3           |         AIN0      |     AIN3       | |
 | ADS1220_MUX_1_2           |         AIN1      |     AIN2       | |
 | ADS1220_MUX_1_3           |         AIN1      |     AIN3       | |
 | ADS1220_MUX_2_3           |         AIN2      |     AIN2       | |
 | ADS1220_MUX_1_0           |         AIN1      |     AIN0       | |
 | ADS1220_MUX_3_2           |         AIN3      |     AIN2       | |
 | ADS1220_MUX_0_AVSS        |         AIN0      |     AVSS       |    single-ended  |
 | ADS1220_MUX_1_AVSS        |         AIN1      |     AVSS       |    single-ended  |
 | ADS1220_MUX_2_AVSS        |         AIN2      |     AVSS       |    single-ended  |
 | ADS1220_MUX_3_AVSS        |         AIN3      |     AVSS       |    single-ended  |
 | ADS1220_MUX_REFPX_REFNX_4 |      REFP0/REFP1  | REFN0/REFN1    | (REFPX-REFNX)/4; PGA bypassed  |
 | ADS1220_MUX_AVDD_M_AVSS_4 |         AVDD      |     AVSS       |    (AVDD-AVSS)/4; PGA bypassed  |
 | ADS1220_MUX_AVDD_P_AVSS_2 |         AVDD      |     AVSS       |    (AVDD+AVSS)/2  |
 

The last three modes use the internal reference (2.048 V) and gain = 1, independent of your settings.


## Setup and Installation

1. Install PlatformIO IDE
2. Clone this repository
3. Open the project in PlatformIO
4. Install required libraries using PlatformIO's library manager
5. Build and upload to your ESP32 device

## Usage

1. Power up the device
2. The device will start advertising as "Neurofy Headband"
3. Connect to the device using any BLE client
4. EEG data will be transmitted at 250Hz sampling rate
