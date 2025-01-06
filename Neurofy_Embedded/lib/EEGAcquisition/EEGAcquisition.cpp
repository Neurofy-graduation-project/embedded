#include "EEGAcquisition.h"

// Function to Initialize ADS1220
void initADS1220(ADS1220_WE &ads) {
    // Alert for ADS1220 Connection Failure
    if (!ads.init()) {
        Serial.println("ADS1220 is not connected!");
        while (1);
    }

    // ADS1220 Gain Configuration
    /* The voltages to be measured need to be between 
     * negative VREF + 0.2 V and positive VREF - 0.2 V if PGA is enabled.
     */
    ads.setGain(ADS1220_GAIN_128); // Set Gain to 128
    ads.bypassPGA(false);          // Ensure Disabling bypassPGA. Redundant since this is default.

    // ADS1220 Mode Setup to Compare Channel 0 and 1
    ads.setCompareChannels(ADS1220_MUX_0_1); 

    Serial.println("ADS1220 Initialized and Gain Configured");
}


// Function to Send EEG Data via BLE
void sendEEGDataBLE(ADS1220_WE &ads, BLECharacteristic *pCharacteristic, bool deviceConnected) {
    // Read EEG Data from ADS1220 in microvolts
    float EEG_uv = ads.getVoltage_muV();
    
    // Check if a Device is Connected
    if (deviceConnected) {

        // Convert the Float Value of EEG_uv to a String with a Min Width of 1 and 2 Decimal Places
        char txString[8];
        dtostrf(EEG_uv, 1, 2, txString); 

        // Setting the String Value to the BLE Characteristic
        pCharacteristic->setValue(txString);

        // Send the String Value to the Connected Client via Notifications
        pCharacteristic->notify();
        Serial.print("EEG Data Sent: ");
        Serial.println(txString);
        
        // Data Rate
        delay(DATA_RATE_MILLISEC);
    }
}
