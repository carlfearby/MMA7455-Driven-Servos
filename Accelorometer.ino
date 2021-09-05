#include "MMA7455.h"
#include <Servo.h>

MMA7455 my_mma;

Servo servo1;
Servo servo2;

int servoPin1 = 3;
int servoPin2 = 5;

int valX = 90;
int valY = 90;


float valXScaled;
float valXSmoothed;
float valXSmoothedPrev;

float valYScaled;
float valYSmoothed;
float valYSmoothedPrev;

//int checkXYZ = 300; // ms
//int lastXYZ = 0;
//
//int doServos = 10;
//int lastServos = 0;

uint16_t x,y,z;
double dX,dY,dZ;

void setup()
{
  uint8_t c;

  Serial.begin(9600);
  Serial.println("Freescale MMA7455 accelerometer");
  
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

  servo1.write(valX);
  servo1.write(valY);
}


void loop()
{
  checkSensor();
  setServos();
  delay(30);
}

void checkSensor() {
  static unsigned long lastTime = 0;
  const long interval = 500;
  unsigned long now = millis();

  if ( now - lastTime > interval) {
        
    my_mma.xyz(&x, &y, &z);
  
    if (y > 32000) {
      valY = y - 65535;
    } else {
      valY = y;
    }
  
    if (x > 32000) {
      valX = x - 65535;
    } else {
      valX = x;
    }
  
    valY = map(valY, -64, 64, 0, 180);
    valX = map(valX, -64, 64, 0, 180);    
  }

}

void setServos() {
  valXSmoothed = (valX * 0.1) + (valXSmoothedPrev * 0.9);
  valXSmoothedPrev = valXSmoothed;

  valYSmoothed = (valY * 0.1) + (valYSmoothedPrev * 0.9);
  valYSmoothedPrev = valYSmoothed;

  
  if(valX >=0 || valX <= 180) {
    servo1.write(valXSmoothed);
  }
  
  if(valY >= 0 || valY <= 180) {
    servo2.write(valYSmoothed);
  }
  
}
