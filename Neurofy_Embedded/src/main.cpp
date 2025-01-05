// Libraries
#include <Arduino.h>
#include <ADS1220_WE.h>
#include <SPI.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

// Pin Definition
#define ADS1220_CS_PIN 5    // Chip Select Pin
#define ADS1220_DRDY_PIN 22 // Data Ready Pin

// Data Rate Definition
/*
 *Sampling Frequency = 250 Hz
 *Then, Data Rate = 4 millisec
 */
#define DATA_RATE_MILLISEC 4 // Data Rate in milliseconds

// Create your ADS1220 object
ADS1220_WE ads = ADS1220_WE(ADS1220_CS_PIN, ADS1220_DRDY_PIN);

// Global Variable Declarations
BLECharacteristic *pCharacteristic; // Pointer to handle the BLE characteristic
bool deviceConnected = false;       // Flag to track whether a device is connected or disconnectes
int txValue = 0;                    // Value sent via BLE

// Universally Unique IDs Definitions for the BLE Service and Characteristic
#define SERVICE_UUID "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"           // Service ID
#define CHARACTERISTIC_UUID_TX "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // Characteristic ID

// Class to Handle Server Callbacks (Connect/Disconnect Events)
class MyServerCallbacks : public BLEServerCallbacks
{
  void onConnect(BLEServer *pServer)
  {
    Serial.println("Device Connected");
    deviceConnected = true; // Device Connected
  };

  void onDisconnect(BLEServer *pServer)
  {
    Serial.println("Device Disconnected");
    deviceConnected = false;            // Device Disconnected
    pServer->getAdvertising()->start(); // Start Advertising to Make the Device Discoverable Again
  }
};

void setup()
{

  // Serial Monitor Initialization for Debugging
  Serial.begin(9600);

  // Alert for ADS1220 Connection Failure
  if (!ads.init())
  {
    Serial.println("ADS1220 is not connected!");
    while (1)
      ;
  }

  // Gain Setup
  /* The voltages to be measured need to be between negative VREF + 0.2 V and positive
   * VREF -0.2 V if PGA is enabled.
   */
  ads.setGain(ADS1220_GAIN_128); // Set Gain to 128
  ads.bypassPGA(false);          // Ensure Disabling bypassPGA. Redundant since this is default.

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
  Serial.println("Waiting for a client connection to notify ...");
}

void loop()
{

  // Variables
  float EEG_mv = 0.0;
  float EEG_uv = 0.0;
  long rawADC = 0;

  // Read from ADS1220
  ads.setCompareChannels(ADS1220_MUX_0_1); // ADS1220 Mode Setup
  EEG_mv = ads.getVoltage_mV();            // Read data from ADS1220 in millivolt
  EEG_uv = ads.getVoltage_muV();           // Read data from ADS1220 in microvolt
  rawADC = ads.getRawData();               // Read raw data from ADS1220

  Serial.print("AIN0 vs. AIN1 (FP2-T8)  [mV]: "); // Print data in millivot
  Serial.println(EEG_mv);
  Serial.print("AIN0 vs. AIN1 (FP2-T8)  [uV]: "); // Print data in microvolt
  Serial.println(EEG_uv);
  Serial.print("AIN0 vs. AIN1 (FP2-T8)  [raw]: "); // Print raw data
  Serial.println(rawADC);
  Serial.println();

  // Check if a Device is Connected
  if (deviceConnected)
  {
    // Send EEG Data in microvolt
    txValue = EEG_uv;

    // Convert the Float Value to a String with 2 Decimal Places
    char txString[8];                 // Array of Char to Store the String Value
    dtostrf(txValue, 1, 2, txString); // Convert the Float txValue to a String in txString with a Min Width of 1 Digit and Precision of 2

    // Setting the String Value to the BLE Characteristic
    pCharacteristic->setValue(txString);

    // Send the String Value to the Connected Client via Notification
    pCharacteristic->notify();
    Serial.println("Sent Value: " + String(txString)); // Print the Sent Value for Debugging

    // Data Rate
    delay(DATA_RATE_MILLISEC);
  }
}