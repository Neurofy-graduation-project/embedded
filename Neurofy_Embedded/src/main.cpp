// Libraries
#include <Arduino.h>

// Header Files
#include "NeurofyBLE.h"
#include "EEGAcquisition.h"

// Create an ADS1220 Object
ADS1220_WE ads(ADS1220_CS_PIN, ADS1220_DRDY_PIN); 

// Pointer to Handle the BLE Characteristic
BLECharacteristic *pCharacteristic;

// Flag to Track the Device Connection Status. Initialized with False.
bool deviceConnected = false;

void setup() {
  // Initialization
  Serial.begin(9600);        // Serial Monitor Initialization for Debugging
  initADS1220(ads);          // ADS1220 Initialization
  setupBLE(pCharacteristic); // BLE Initialization
}

void loop() {
  // Send EEG Data via BLE
  sendEEGDataBLE(ads, pCharacteristic, deviceConnected); 
}
