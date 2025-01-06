# EEG Acquisition Module

This module handles EEG signal acquisition using the ADS1220 Analog-to-Digital Converter (ADC).

## Features

- High-precision EEG signal acquisition through a 24-bit ADC
- Configurable gain settings
- Differential input measurement
- 250Hz sampling rate

## Files

- `EEGAcquisition.h`: Header file containing ADC definitions and function declarations
- `EEGAcquisition.cpp`: Implementation file containing ADC functionality

## Technical Details

### Hardware Configuration

- ADS1220 CS Pin: GPIO 5 in ESP32
- ADS1220 DRDY Pin: GPIO 22 in ESP32
- Sampling Rate: 250 Hz (4ms interval)
- Gain: 128x
- Input Configuration: Differential (Channel 0 - Channel 1)

### Key Functions

- `initADS1220()`: Initializes and configures the ADS1220
- `sendEEGDataBLE()`: Reads ADC data and prepares it for BLE transmission

### Signal Processing

- Measures differential voltage between channels 0 and 1
- Returns values in microvolts
- PGA enabled with gain of 128
- Operating range: ± ( VREF - 0.2V )

## Usage Example

```cpp
ADS1220_WE ads(ADS1220_CS_PIN, ADS1220_DRDY_PIN);
initADS1220(ads);

// Read EEG data
float eegData = ads.getVoltage_muV();
```

## Dependencies

- ADS1220_WE library (version 1.0.15)
- SPI.h
