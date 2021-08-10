#include "ReceiveOnlySoftwareSerial.h"

#define RX_SOFT_SERIAL_PIN 16

ReceiveOnlySoftwareSerial RXSoftSerial(RX_SOFT_SERIAL_PIN);

void setup() {
  Serial.begin(9600);
  RXSoftSerial.begin(9600);
}

void loop() {
    if (RXSoftSerial.available())
      Serial.write(RXSoftSerial.read());
}
