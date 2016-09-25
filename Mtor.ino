#include "DualVNH5019MotorShield.h"
#include <PinChangeInt.h>
#include <digitalWriteFast.h>
DualVNH5019MotorShield md; //instantaneous object
//int encoderPin1 = 2;
int encoderPinLA = 3;
int encoderPinLB=  5;
int encoderPinRA= 11;
int encoderPinRB= 13;
double prevTime,currTime=0.0;
//volatile int lastEncoded = 0;
//volatile long encoderValue = 0;
volatile int motorLOldA = 0, motorROldA = 0, motorLNewB = 0, motorRNewB = 0;
//long lastencoderValue = 0;
int motorLacc=0;
int motorLprevaccm=0;
int motorLprevnet=0;
double motorLSpeed=0;
volatile int motorLnet=0;

int motorRacc=0;
int motorRprevaccm=0;
double motorRSpeed=0;
int motorRprevnet=0;
volatile int motorRnet=0;

double motorEncoderDiff=0;

//int lastMSB = 0;
//int lastLSB = 0;
void setup() {
  // put your setup code here, to run once:
    Serial.begin(1200);
  
    pinMode(encoderPinLA, INPUT);
    pinMode(encoderPinLB, INPUT);
    pinMode(encoderPinRA, INPUT);
    pinMode(encoderPinRB, INPUT);
    Serial.println("Dual VNH5019 Motor Shield");
    md.init();
    PCintPort::attachInterrupt(encoderPinLA, &motorLISRA, CHANGE);  //Attach left motor encoder interrupt pin to the ISR
    PCintPort::attachInterrupt(encoderPinLB, &motorLISRB, CHANGE);  //Attach left motor encoder interrupt pin to the ISR
    PCintPort::attachInterrupt(encoderPinRA, &motorRISRA, CHANGE);  //Attach right motor encoder interrupt pin to the ISR
    PCintPort::attachInterrupt(encoderPinRB, &motorRISRB, CHANGE);  //Attach left motor encoder interrupt pin to the ISR
}
/*
void stopIfFault()
{
  if (md.getM1Fault())
  {
    Serial.println("M1 fault");
    while(1);
  }
  if (md.getM2Fault())
  {
    Serial.println("M2 fault");
    while(1);
  }
}
*/
void loop() {
  // put your main code here, to run repeatedly:
  //rotateRight();
  
 // double val = digitalRead(encoderPinLA);
  //md.setM1Speed(200);
 // md.setM2Speed(200);
  //rotateClock(1,-1.120);
 // rotateClock(1,1);
  
  move();
  //rotateRight();
  
 // Serial.println(val);
}


void move(){
//  resetMove();
  int count=0;
  int motorLDiff=0;
  int motorRDiff=0;
  md.setM2Speed(100);
  md.setM1Speed(0);
  while(true){
    /*if(motorRacc>=2249)
    {
      motorRacc=0;
      md.setM2Speed(0);
      md.setM1Speed(200);
     }*/
     /*if(motorLacc==0)
     {
     md.setM1Speed(20);
     md.setM2Speed(0);
     }
     else
     */
     if(motorRacc>=1995)
     {
      md.setM1Speed(80);
      md.setM2Brake(400);
      motorRacc=0;
      }
     if(motorLacc>=1995)
     {
       md.setM1Brake(400);
      md.setM2Speed(80);
      motorLacc=0;
     }
     /*if(motorLacc>=2249)
     {
      motorLacc=0;
      md.setM2Speed(200);
      md.setM1Speed(0);
      }*/
    motorEncoderDiff = motorLacc - motorRacc;
        motorLDiff = motorLacc - motorLprevaccm;
        Serial.print("MotorLCount: ");
        Serial.println(motorLacc);
        motorRDiff = motorRacc - motorRprevaccm;
        currTime = millis() + 1;  //Current time since the start of execution. Plus 1 to avoid divide by 0.
        Serial.print("Current time");
        Serial.println(currTime);
        motorLSpeed = motorLDiff / (currTime - prevTime);
        Serial.print("Motor L Speed: ");
        Serial.println(motorLSpeed);
        motorRSpeed = motorRDiff / (currTime - prevTime);
        Serial.print("Motor R Speed: ");
        Serial.println(motorRSpeed);
        Serial.print("MotorRCount: ");
        Serial.println(motorRacc);
        motorLprevaccm = motorLacc;
        motorRprevaccm = motorRacc;
        prevTime = currTime;
        
    delay(1000);
    }
  
  }
void rotateClock(double speedChangeL,double speedChangeR)
{
  //md.setM1Speed(100);
  //md.setM2Speed(100);
  md.setM1Speed(150*speedChangeL);
  //stopIfFault();
  md.setM2Speed(150*speedChangeR);
  //stopIfFault();
  }

void rotateRight(){
 md.setM1Speed(100);
  //stopIfFault();
  md.setM2Speed(100*1.01);
  //stopIfFault();
  
}
void rotateLeft(){
  md.setM2Speed(100);
//  stopIfFault();
  md.setM1Speed(100);
  //stopIfFault();
  }

  /*void turn90(){
    md.setM1Speed(
    }*/

void motorLISRA()      //ISR for left motor encoder interrupt
{
    motorLacc++;
    motorLNewB ^motorLOldA ? motorLnet++ : motorLnet--;
    motorLOldA = digitalReadFast(encoderPinLA);
}

void motorLISRB()      //ISR for left motor encoder interrupt
{
    motorLacc++;
    motorLNewB = digitalReadFast(encoderPinLB);
    motorLNewB ^motorLOldA ? motorLnet++ : motorLnet--;
}

void motorRISRA()      //ISR for left motor encoder interrupt
{
    motorRacc++;
    motorRNewB ^motorROldA ? motorRnet-- : motorRnet++;
    motorROldA = digitalReadFast(encoderPinRA);
}

void motorRISRB()      //ISR for left motor encoder interrupt
{
    motorRacc++;
    motorRNewB = digitalReadFast(encoderPinRB);
    motorRNewB ^motorROldA ? motorRnet-- : motorRnet++;
}
