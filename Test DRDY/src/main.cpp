#include <Arduino.h>

#define DRDY_PIN 13

void setup() {
  Serial.begin(9600);
  pinMode(DRDY_PIN, INPUT);

}

void loop() {
  int drdyState = digitalRead(DRDY_PIN);
  Serial.println(drdyState); // Print the state of the DRDY pin
  delay(100);
}

