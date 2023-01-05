// Arduino nano receiver code to control RC car using nRF24L01+PA+LNA transceiver
// and TB6612FNG motor driver
// Made for K-State Solar Club

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include "printf.h"

RF24 radio(9, 8); // CE, CSN
const byte address[10] = "ADDRESS01";

// These pins are used to control motor direction
#define AIN1 A5
#define AIN2 A4
#define BIN1 A3
#define BIN2 A2
#define leftMotorPin 5
#define rightMotorPin 6

unsigned long timeout;  // Timer for timeout caused by lack of data from radios


void setup() {

  // motor related pins
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(leftMotorPin, OUTPUT);
  pinMode(rightMotorPin, OUTPUT);
  
  Serial.begin(9600);

  // Set up nRF24L01
  radio.begin();
  radio.setAutoAck(false);
  radio.openReadingPipe(0, address);
  radio.setDataRate(RF24_250KBPS);
  radio.setChannel(108);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
  printf_begin();
  radio.printDetails();

  timeout = millis();
}

void loop() {
  if (radio.available()) {
    Serial.println("Data received");
    int txt[4] = {};
    radio.read(&txt, sizeof(txt));

    // Check for direction of left motor
    if (txt[0] < 0) {
      digitalWrite(AIN1, HIGH);
      digitalWrite(AIN2, LOW);
    }
    else {
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, HIGH);
    }
    // Check for direction of right motor
    if (txt[1] < 0) {
      digitalWrite(BIN1, HIGH);
      digitalWrite(BIN2, LOW);
    }
    else {
      digitalWrite(BIN1, LOW);
      digitalWrite(BIN2, HIGH);
    }

    // Write motor vals to PWM pins
    analogWrite(leftMotorPin, map(abs(txt[0]), 0, 512, 0, 255));
    analogWrite(rightMotorPin, map(abs(txt[1]), 0, 512, 0, 255));
    timeout = millis();
  } // end of if radio.available()
  
  else {
    if (millis() - timeout >= 500) {  // If it's been 500ms since last radio transmission
      // should put motor driver into "stop" mode
      digitalWrite(AIN1, LOW);
      digitalWrite(AIN2, LOW);
      digitalWrite(BIN1, LOW);
      digitalWrite(BIN2, LOW);
      analogWrite(leftMotorPin, 255);
      analogWrite(rightMotorPin, 255);
    }
  } // end of timeout 
  
}
