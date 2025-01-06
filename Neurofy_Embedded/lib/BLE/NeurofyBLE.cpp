#include <Arduino.h>
#include "NeurofyBLE.h"

// Class to Handle Server Callbacks (Connect/Disconnect Events)
class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
        deviceConnected = true;             // Device Connected
    }
    void onDisconnect(BLEServer* pServer) {
        deviceConnected = false;            // Device Disconnected
        pServer->getAdvertising()->start(); // Start Advertising to Make the Device Discoverable Again
    }
};

// Function to Setup BLE
void setupBLE(BLECharacteristic *&pCharacteristic) {
    
    // Create and Name the BLE Device    
    BLEDevice::init("Neurofy Headband");
    
    // Create the BLE Server
    BLEServer *pServer = BLEDevice::createServer();
    pServer->setCallbacks(new MyServerCallbacks()); // Set callback for connection events

    // Create the BLE Service
    BLEService *pService = pServer->createService(SERVICE_UUID);

    // Create a BLE Charateristic for Notifications
    pCharacteristic = pService->createCharacteristic(
                        CHARACTERISTIC_UUID_TX,            // Characteristic UUID
                        BLECharacteristic::PROPERTY_NOTIFY // Enable Notifications
    );

    // Add a Descriptor to the Characteristic (Required for Notifications)
    pCharacteristic->addDescriptor(new BLE2902());
    
    // Start the BLE Service
    pService->start();

    // Start Advertising the BLE Service to Allow Clients to Discover it
    pServer->getAdvertising()->start();
    Serial.println("BLE Initialized, waiting for connections...");
}
