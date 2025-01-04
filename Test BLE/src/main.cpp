// Libraries
#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Global Variable Declarations
BLECharacteristic *pCharacteristic;  // Pointer to handle the BLE characteristic
bool deviceConnected = false;        // Flag to track whether a device is connected or disconnectes
int txValue = 0;                     // Value sent via BLE 

// Universally Unique IDs Definitions for the BLE Service and Characteristic
#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"  // Service ID
#define CHARACTERISTIC_UUID_TX "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"  // Characteristic ID

// Class to Handle Server Callbacks (Connect/Disconnect Events)
class MyServerCallbacks: public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    Serial.println("Device Connected");
    deviceConnected = true; // Device Connected
  };

  void onDisconnect(BLEServer* pServer) {
    Serial.println("Device Disconnected");
    deviceConnected = false;            // Device Disconnected
    pServer->getAdvertising()->start(); // Start Advertising to Make the Device Discoverable Again
  }
};

void setup() {

  // Start Serial Communication for Debugging
  Serial.begin(9600);

  //Create and Name the BLE Device
  BLEDevice::init("Neurofy Headband"); 

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks()); // Set callback for connection events

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);  

  // Create a BLE Charateristic for Notifications
  pCharacteristic = pService->createCharacteristic(
                                                CHARACTERISTIC_UUID_TX,             // Characteristic UUID
                                                BLECharacteristic::PROPERTY_NOTIFY  // Enable Notifications
                                                );

  // Add a Descriptor to the Characteristic (Required for Notifications)
  pCharacteristic->addDescriptor(new BLE2902());

  // Start the BLE Service
  pService->start();

  // Start Advertising the BLE Service to Allow Clients to Discover it
  pServer->getAdvertising()->start();
  Serial.println("Waiting for a client connection to notify ...");

}

void loop() {

  // Check if a Device is Connected
  if(deviceConnected) {
    // Generate a random value between -10 and 20
    txValue = random(-10, 20); 

    // Convert the Float Value to a String with 2 Decimal Places
    char txString[8];                  // Array of Char to Store the String Value
    dtostrf(txValue, 1, 2, txString);  // Convert the Float txValue to a String in txString with a Min Width of 1 Digit and Precision of 2

    // Setting the String Value to the BLE Characteristic
    pCharacteristic->setValue(txString);

    // Send the String Value to the Connected Client via Notification
    pCharacteristic->notify();
    Serial.println("Sent Value: " + String(txString));  // Print the Sent Value for Debugging

    //  Data Rate = 256 Hz >>> T = 3.9 ms
    delay(4); 
  }

}
