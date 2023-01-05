// Arduino nano transmitter code to control RC car using nRF24L01+PA+LNA transceiver
// and TB6612FNG motor driver
// Made for K-State Solar Club

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include "printf.h"

RF24 radio(9, 8); // CE, CSN
const byte address[10] = "ADDRESS01";

int data[4];
unsigned long timer;
int interval = 20; // interval between data transmissions

void setup() {
  Serial.begin(9600);

  // Set up nRF24L01
  radio.begin();
  radio.setAutoAck(false);
  radio.openWritingPipe(address);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  printf_begin();
  radio.printDetails();
  Serial.println("Starting");

  timer = millis();
};

void loop() {
  
  if (millis() - timer >= interval) {
    data[0] = analogRead(A6) - 512; // Left motor
    data[1] = analogRead(A7) - 512; // Right motor
    data[2] = 1;  // Could put a push button here
    data[3] = 0;  // same as above
    radio.write(&data, sizeof(data));
    timer += interval;
  }
  
};
