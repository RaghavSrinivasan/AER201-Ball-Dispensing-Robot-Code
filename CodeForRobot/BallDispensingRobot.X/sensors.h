/* 
 * File:   sensors.h
 * Author: Raghav Srinivasan
 * Description: This file contains all the sensor interfaces with the Arduino
 * Nano
 * Created on February 25, 2019, 10:27 PM
 */

#include <xc.h>
#include "I2C.h"

//The following are constants for which sensor we are requesting data from 
//in the Arduino
#define RotaryEncoder1 0
#define RotaryEncoder2 6
#define AnalogSensor1 1
#define AnalogSensor2 2
#define AnalogSensor3 7
#define AnalogSensor4 8
#define AnalogSensor5 9
#define DigitalSensor 3
#define HallEffect 4
#define FrontSensor 5
#define FrontSensor2 10
#define RemoteStart 11
//shaft encoder data
int shaftEncoderCount(void);

int shaftEncoderCount2(void);

//tests I2C functionality and communication
void senseTest(void);
//reads analog sensor values based on input. Input is code of Analog sensor we
//wish to request. i.e distFromCanister(AnalogSensor1) (which has a value of 1)
//will return the analog input from Analog Sensor 1 on the Arduino
int distFromCanister(int sensor);

//reads digital IR sensor
int readDigitalSensor(void);

//reads Hall Effect Sensor:
int readHallEffect(void);

//reads Front IR sensors:
int readFrontSensor(void);

int readFrontSensor2(void);

//reads remote start pin from ESP
int readRemoteStart(void);
//maps distance from shaft encoder to actual measurements, 
//based on tests. It is to be noted getDistance2() is not very functional, 
//mainly only the first shaft encoder
float getDistance(void);
float getDistance2(void);

//tests sending I2C
void sendData(void);