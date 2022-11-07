#include <RH_ASK.h>
#include <SPI.h> // Not actualy used but needed to compile

RH_ASK driver;

int motor1;
int motor2;
unsigned long timeout;

void setup()
{
    timeout = millis();
    pinMode(5, OUTPUT);
    pinMode(6, OUTPUT);
    Serial.begin(9600);  // Debugging only
    if (!driver.init())
         Serial.println("init failed");
}

void loop()
{
    uint8_t buf[14];
    uint8_t buflen = sizeof(buf);
    if (driver.recv(buf, &buflen)) // Non-blocking
    {
      int i;
      // Message with a good checksum received, dump it.
      //Serial.print("Message: ");
      //Serial.println((char*)buf); 
      //Serial.println(buf[7]);
      String data = (char*)buf;

      int comma;
      int pound;
      for (int j = 0; j < data.length(); j++) {
        if (data[j] == ',')
          comma = j;

        if (data[j] == '#') {
          pound = j;
          break;
        }
      }

      String motor1String = data.substring(0,comma);
      String motor2String = data.substring(comma+1, pound);

      motor1 = abs(motor1String.toInt());
      motor2 = abs(motor2String.toInt());

      motor1 = map(motor1, 0, 512, 0, 255);
      motor2 = map(motor2, 0, 512, 0, 255);

      if ((motor1 <= 255 && motor1 >= 0) && (motor1 <= 255 && motor1 >= 0)) {
        analogWrite(5, motor1);
        analogWrite(6, motor2);
      }
      timeout = millis();
      
      Serial.print("Right: ");
      Serial.print(motor1);
      Serial.print(" Left: ");
      Serial.println(motor2);
              
    }
    if (millis() - timeout >= 1000) {
      analogWrite(5, 0);
      analogWrite(6, 0);
    }
}
// Test Change 11/7/22
