#include <Servo.h>
#include <Adafruit_MotorShield.h>

/* 
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control
For use with the Adafruit Motor Shield v2 
---->  http://www.adafruit.com/products/1438
*/

#include <Wire.h>
#define LoopTime  2000

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);
Servo rotatemotor;
Servo head;
// You can also make another motor on port M2
//Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);

int photocellPin1 = 0;
int photocellPin2 = 1;
int Reading1;
int Reading2;
int lightmax = 20;
int lightmin = -20;
int lightdiff;
int degree;
int prevdeg = 90;
long prevtime;
int temp;

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Adafruit Motorshield v2 - DC Motor test!");

  AFMS.begin();  // create with the default frequency 1.6KHz
  rotatemotor.attach(10); // pin 10
//  rotatemotor.write(89); // 90 means dont move
//  delay(2500);
  rotatemotor.write(90);
//  delay(1000);
//  rotatemotor.write(92);
//  delay(2500);
//  rotatemotor.write(90);
  getLight();
  temp = lightdiff;
  prevtime = millis();
  while ((millis() - prevtime) <= LoopTime) {
     temp += (analogRead(photocellPin1) - analogRead(photocellPin2))/2;
  }
}

void getLight() {
  Reading1 = analogRead(photocellPin1);
  Reading2 = analogRead(photocellPin2);
  lightdiff = Reading1 - Reading2;
  if (lightdiff < lightmin) {
    lightmin = lightdiff;
  } else if (lightdiff > lightmax) {
      lightmax = lightdiff;
  }
}
void loop() {
  /*
   * roughly 90-100 for base light in design lab
   * when light goes closer, the numbers get smaller
   * so 1 - (reading) gets us light readings\
   * the reading gives us darkness value
   */
  getLight();
  degree = map(lightdiff-temp, lightmin, lightmax, 0, 180);
  if (degree > prevdeg + 5) {
    rotatemotor.write(degree);
    prevdeg = degree;
    delay(500);
  } else if (degree < prevdeg - 5) {
    rotatemotor.write(degree);
    prevdeg = degree;
    delay(500);
  }
  Serial.print("sensor 1:");
  Serial.println(Reading1);
  Serial.print("sensor 2:");
  Serial.println(Reading2);
  while ((millis() - prevtime) <= LoopTime) {
     delay(millis() - prevtime);
  }
}
