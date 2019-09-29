/* 
 * File:   motion.h
 * Author: Raghav Srinivasan
 * Date: February 6, 2019, 2:19 PM
 * Description: This file defines a datatype for each of the modes of motion, as
 * well as giving a prototype for the function that moves the robot (using the 
 * circuit interface provided by Martin Ffrench)
 */

#include <stdbool.h>
#include "configBits.h"

#define BREAK 0
#define FORWARD 1
#define BACKWARD 2
#define LEFT 3
#define RIGHT 4
#define CCW 5
#define CW 6

#define JamTime 0.6

//This method moves the robot by inputting a particular movement from the
//constant above, and executing that action by activating the pins as desired 
//connected to the motor logic circuit
void moveRobot(int move);

//release door method to trigger revolving door:
bool releaseDoor(void);

//2 revolving door method depending on Hall Effect Sensor placements when
//initializing:
//if Hall Effect not active
bool releaseDoor1(void);
//if Hall Effect active
bool releaseDoor2(void);

//swerve right if front detector detects something (for avoidance)
void avoidCan(void);