#include <RH_ASK.h>
#include <SPI.h> // Not actually used but needed to compile

RH_ASK driver;

int motor1;
int motor2;


int adcX;
int adcY;

void setup()
{
   
    pinMode(A7, INPUT);
    pinMode(A6, INPUT);
    Serial.begin(9600);    // Debugging only
    if (!driver.init())
         Serial.println("init failed");
}

void loop()
{
    
    adcX = analogRead(A7)-512;
    adcY = analogRead(A6)-512;
    
    motor1 = adcY;
    motor2 = adcY;

    if (adcX < 0) {
      if (adcY <= 0) {
        motor1 -= adcX;
      }
      else {
        motor1 += adcX;
      }
    }
    else if (adcX > 0) {
      if (adcY <= 0) {
        motor2 += adcX;
      }
      else {
        motor2 -= adcX;
      }
    }
    
    String stringMsg = String(motor2) + "," + String(motor1) + "#";
    const char *msg = stringMsg.c_str();
    //const char *msg = "Hello world";
    driver.send((uint8_t*)msg, strlen(msg));
    Serial.println(msg);
    driver.waitPacketSent();
    delay(10);

    /*
    const char *msg = "Hello World!";
    driver.send((uint8_t *)msg, strlen(msg));
    driver.waitPacketSent();
    delay(1000);
    */
}
//test Gantzen was here
