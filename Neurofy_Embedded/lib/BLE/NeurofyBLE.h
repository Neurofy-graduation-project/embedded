#ifndef NEUROFY_BLE_H // Include Guard
#define NEUROFY_BLE_H

// BLE Libraries
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

/*
 * Ensure that BLE2902.h is included once
 * as it also included in EEGAcquisition.h
 */
#pragma once  
#include <BLE2902.h>

// Universally Unique IDs Definitions for the BLE Service and Characteristic
#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"

extern bool deviceConnected;                        // Flag to track the device connection status
void setupBLE(BLECharacteristic *&pCharacteristic); // Function prototype to setup BLE
#endif // NEUROFY_BLE_H
