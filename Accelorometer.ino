#include "MMA7455.h"
#include <Servo.h>

MMA7455 my_mma;

Servo servo1;
Servo servo2;

int servoPin1 = 3;
int servoPin2 = 4;

int valX = 90;
int valY = 90;

void setup()
{
  uint8_t c;

  Serial.begin(9600);
  Serial.println("Freescale MMA7455 accelerometer");
  Serial.println("May 2012");
  
#if defined __AVR__
  my_mma.begin();       
#else
  my_mma.begin(D3,D4);
#endif

  my_mma.read(MMA7455_MCTL, &c, 1);
  Serial.print("Mode: "); Serial.println(c);
  
  my_mma.read(MMA7455_STATUS, &c, 1);
  Serial.print("Status: "); Serial.println(c);

  my_mma.read(MMA7455_WHOAMI, &c, 1);
  Serial.print("WhoAmI: "); Serial.println(c);

  my_mma.read(MMA7455_TOUT, &c, 1);
  Serial.print("Temp.: "); Serial.println(c);

  servo1.attach(servoPin1);
  servo2.attach(servoPin2);
}


void loop()
{
  uint16_t x,y,z, error;
  double dX,dY,dZ;

  x = y = z = 0;
  error = my_mma.xyz(&x, &y, &z); // get the accelerometer values.

  dX = (int16_t) x / 64.0;          // calculate the 'g' values.
  dY = (int16_t) y / 64.0;
  dZ = (int16_t) z / 64.0;

  if (error != 0 || abs(dX) > 0.04 || abs(dY) > 0.04 || abs(dZ) > 1.04) {
    valX = map(dX * 50 + 50, 0, 100, 0, 180);
    valY = map(dY * 50 + 50, 0, 100, 0, 180);
  }


  servo1.write(valY);
  servo2.write(valX);
  delay(5);
}
