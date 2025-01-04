#include <Arduino.h>
#include <SPI.h>

// Pin Definitions
#define CS_PIN 5     // Chip Select for ADS1220
#define DRDY_PIN 22  // Data Ready Pin for ADS1220

// SPI settings for ADS1220
SPISettings ads1220SPI(1000000, MSBFIRST, SPI_MODE1);
// 4000000 sets the SPI clock speed to 1 MHz. This speed controls how fast data is transferred between the ESP32 and the ADS1220.
// MSBFIRST means that the Most Significant Bit (MSB) is sent first during data transmission. The ADS1220 expects this format.
// SPI_MODE1 configures the clock polarity and phase. For the ADS1220, Mode 1 is required (CPOL = 0, CPHA = 1), meaning the clock is low when idle and data is sampled on the falling edge.

// ADS1220 Command Bytes
#define START_SYNC 0x08
#define RDATA 0x10 // Read data command

// Function Prototype
void ads1220_init(void);
long readADC(void);
float convertToMicroVoltage(long adcValue);

void setup()
{
  Serial.begin(9600); // Begin serial communication for debugging

  // Setup SPI
  SPI.begin();                // Initialize SPI communication
  pinMode(DRDY_PIN, INPUT);   // Set DRDY pin as input 
  pinMode(CS_PIN, OUTPUT);    // Set CS pin as output
  digitalWrite(CS_PIN, HIGH); // Start with CS disabled (HIGH)

  // Initialize ADS1220
  ads1220_init();
}

void loop()
{
  // Get raw data from ADS1220
  long adcValue = readADC();
  Serial.println(adcValue);

  // Convert raw ADC value to micro voltage (assuming PGA = 128, VREF = 2.048V)
  float microVoltage = convertToMicroVoltage(adcValue);

  // Print the voltage
  Serial.print("EGG: ");
  Serial.print(microVoltage, 3); // 3 decimal places
  Serial.println(" uV");
  delay(1000);
}

// Initialize the ADS1220 (Send configuration bytes)
void ads1220_init()
{
  // Configuration register settings (depends on your requirements)
  // Set PGA, sampling rate, and enable continuous mode, etc.
  digitalWrite(CS_PIN, LOW);  // Select ADS1220
  SPI.transfer(0x40);         // Send a Write Register Command to the ADS1220 to configure it. 0x40 is the command to write to register 0 of the ADS1220.
  SPI.transfer(0x00);         // Send configuration data for the first configuration register.
  SPI.transfer(0x0E);         // Set the gain to 128x and enables continuous conversion mode.
  SPI.transfer(0x20);         // Data rate and reference selection
  digitalWrite(CS_PIN, HIGH); // Deselect ADS1220
  delay(10);
}

// Read raw ADC data from ADS1220
long readADC()
{
  long result = 0;

  digitalWrite(CS_PIN, LOW); // Select ADS1220
  SPI.transfer(RDATA);       // Send read data command

  // Read 3 bytes (24 bits) of data from the ADC
  result = SPI.transfer(0x00);                 // First byte (MSB)
  result = (result << 8) | SPI.transfer(0x00); // Second byte
  result = (result << 8) | SPI.transfer(0x00); // Third byte

  digitalWrite(CS_PIN, HIGH); // Deselect ADS1220

  // Convert to signed 24-bit value
  if (result & 0x800000)
  {                       // Check if the MSB of the 24 bits is set, meaning that the result is negative
    result |= 0xFF000000; // Sign-extends the 24-bit number to 32 bits. 0xFF000000 is a hexadecimal number where the top 8 bits (bits 24-31) are all set to 1
  }

  return result;
}

// Convert raw ADC value to voltage
float convertToMicroVoltage(long adcValue)
{
  // ADS1220 has a 24-bit resolution, and the reference voltage is typically 2.048V by default
  // PGA = 128x, so the input range is +/- 0.016V (Max Input Voltage = VREF / PGA = 2.048V / 128 = 0.016V)

  // Voltage = (adcValue * VREF) / (PGA * (2^23)) in Micro Volt
  // VREF = 2.048V, PGA = 128
  // Thus, Resolution = 0.002 uV
  float microVoltage = (adcValue * 2.048) / (128 * 8388608.0) * 1000000.0; // 8388608 = 2^23 for 24-bit resolution (the 24th bit is for the sign)
  return microVoltage;
}
