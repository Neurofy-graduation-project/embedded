#ifndef EEG_ACQUISITION_H // Include Guard
#define EEG_ACQUISITION_H

// Libraries
#include <ADS1220_WE.h>
#include <SPI.h>

/*
 * Ensure that BLE2902.h is included once
 * as it also included in NeurofyBLE.h
 */
#pragma once
#include <BLEDevice.h>

// ADS1220 Pins Definition
#define ADS1220_CS_PIN   5  // ADS1220 Chip Select Pin
#define ADS1220_DRDY_PIN 22 // ADS1220 Data Ready Pin

// Data Rate Definition
/*
 *Sampling Frequency = 250 Hz
 *Then, Data Rate = 4 millisec
 */
#define DATA_RATE_MILLISEC 4 // Data Rate in milliseconds

// Function Prototype to Initialize ADS1220
void initADS1220(ADS1220_WE &ads);

// Function Prototype to Send EEG Data via BLE
void sendEEGDataBLE(ADS1220_WE &ads, BLECharacteristic *pCharacteristic, bool deviceConnected);

#endif // EEG_ACQUISITION_H
