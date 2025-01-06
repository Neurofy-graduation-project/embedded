# BLE Communication Module

This module handles Bluetooth Low Energy (BLE) communication for Neurofy Headband.

## Features

- BLE Server implementation
- Notification-based data transmission
- Automatic reconnection handling
- Connection state management

## Files

- `NeurofyBLE.h`: Header file containing BLE definitions and function declarations
- `NeurofyBLE.cpp`: Implementation file containing BLE functionality

## Technical Details

### BLE Configuration

- Device Name: "Neurofy Headband"
- Service UUID: 6E400001-B5A3-F393-E0A9-E50E24DCCA9E
- Characteristic UUID (TX): 6E400001-B5A3-F393-E0A9-E50E24DCCA9E

### Key Functions

- `setupBLE()`: Initializes BLE server and characteristic
- `MyServerCallbacks`: Handles connection and disconnection events

### Data Format

- Transmits EEG data as string values
- Float values converted to strings with 2 decimal places
- Notification-based transmission at 250Hz

## Usage Example

```cpp
BLECharacteristic *pCharacteristic;
setupBLE(pCharacteristic);

// Check connection status
if (deviceConnected) {
    // Send data via BLE
}
```

## Dependencies

- BLEDevice.h
- BLEServer.h
- BLEUtils.h
- BLE2902.h
