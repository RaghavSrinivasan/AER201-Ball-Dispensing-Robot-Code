/*
 * AER201-Arduino.ino
 * 
 * By: Raghav Srinivasan
 * Description: This file contains all the Arduino Code
 * for The Mouse Ball Dispensing Robot, which is meant to
 * provide all of the sensory input
*/

#include <Wire.h>
//id's for sensors:
const byte encoderId = 0;
const byte encoderId2 = 6;
const byte analogSensor1Id = 1;
const byte analogSensor2Id = 2;
const byte analogSensor3Id = 7;
const byte analogSensor4Id = 8;
const byte analogSensor5Id = 9;
const byte digitalID = 3;
const byte hallID = 4;
const byte frontSensorID = 5;
const byte frontSensor2ID = 10;
const byte remoteStartID = 11;

//Pin assignments:
const byte ledPin = 13;
const byte encoder = 2;
const byte encoder2 = 3;
const byte analogSensor1 = A0;
const byte analogSensor2 = A1;
const byte analogSensor3 = A2;
const byte analogSensor4 = A3;
const byte digitalSensor = 7;
const byte hallEffect = 12;
const byte frontSensor = 4;
const byte frontSensor2 = 5;
const byte remoteStart = 8;

//variable declaration:
volatile unsigned int counter1 = 0;
volatile unsigned int counter2 = 0;
int old_counter = 0;
volatile unsigned int data, dataToSend;
volatile uint8_t one;
volatile uint8_t two;
uint8_t x[2];
int distance;

unsigned int sensorData;

void setup() {
  /*
  //for circuit debugging of the shaft encoder
  Serial.begin(9600);
  Serial.println("Rotary Encoder debug");
  Serial.println(" ");
  pinMode(ledPin,OUTPUT);
  */
  //Serial.begin(9600);
  //configure pins:
  pinMode(encoder,INPUT);
  pinMode(encoder2, INPUT);
  pinMode(analogSensor1, INPUT);
  pinMode(analogSensor2, INPUT);
  pinMode(analogSensor3, INPUT);
  pinMode(analogSensor4, INPUT);
  pinMode(frontSensor, INPUT);
  pinMode(frontSensor2, INPUT);
  pinMode(hallEffect, INPUT);
  pinMode(remoteStart, INPUT);
  
  //configure interrupts for shaft encoders
  attachInterrupt(digitalPinToInterrupt(encoder),count,RISING); 
  attachInterrupt(digitalPinToInterrupt(encoder2), count2, RISING);

  //configure I2C
  Wire.begin(8); //join I2C buss with address 8
  Wire.onRequest(requestEvent); //request of information
  Wire.onReceive(receiveEvent); //receive transmission
}

void loop() {
}

//increase shaft encoder count if triggered:
void count(){
  counter1++;
}

void count2(){
  counter2++;
}

//PIC requests to read Arduino:
void requestEvent(void){
 
  one = data;
  two = data >> 8;
  x[0] = one;
  x[1] = two;
  Wire.write(x, 2);
}

//PIC writes to Arduino:
void receiveEvent(void){
    uint8_t x = Wire.read(); // Receive byte

    //load data with requested data:
    if (x == encoderId) //first shaft encoder
    {
        data = counter1;
    }
    if (x == encoderId2) //second shaft encoder
    {
        data = counter2;
    }
    //analog sensor readings:
    if (x == analogSensor1Id) 
    {
        data = analogRead(analogSensor1);
    }
    if (x == analogSensor2Id)
    {
        data = analogRead(analogSensor2);
    }
    if (x == analogSensor3Id)
    {
        data = analogRead(analogSensor3);
    }
    if (x == analogSensor4Id)
    {
        data = analogRead(analogSensor4);
    }
    //digital distance sensor:
    if (x == digitalID)
    {
      data = digitalRead(digitalSensor);
    }
    //hall effect sensor:
    if (x == hallID)
    {
      data = digitalRead(hallEffect);
    }
    //front sensors:
    if (x == frontSensorID)
    {
      data = digitalRead(frontSensor);
    }
    if (x == frontSensor2ID)
    {
      data = digitalRead(frontSensor2);
    }
    if (x == remoteStartID)
    {
      data = digitalRead(remoteStart);
    }
}



