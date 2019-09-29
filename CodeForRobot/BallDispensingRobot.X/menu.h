/* 
 * File:   menu.h
 * Author: Raghav Srinivasan
 *
 * Created on February 1, 2019, 2:28 PM
 */
#include <xc.h>
#include <stdio.h>
#include <stdbool.h>
#include "configBits.h"
#include "lcd.h"

//States the menu can be in:
enum menuState {end, endMenu, time, ballsGive, numCan, canSel, canData};
typedef enum menuState menState;

//constants for the keys pressed in how they are transmitted to B ports:
#define ONE 0
#define TWO 1
#define THREE 2
#define A_KEY 3
#define FOUR 4
#define FIVE 5
#define SIX 6
#define B_KEY 7
#define SEVEN 8
#define EIGHT 9
#define NINE 10
#define C_KEY 11
#define BACK 12 //asterisk key defined as back key
#define ZERO 13
#define SEL 14 //# key defined as select key
#define D_KEY 15

//creates start screen and manages interrupts to get past start screen
void initStartScreen(void);

/*
 * initMenu creates the ending menu and manages all the ending screens,
 * keyboard inputs, and user interactions with menu
 */
void initMenu(void);

/*
 * Creates ending screen
 */
void buildEndScreen(void);

/*
 * Creates end screen menu
 */
void buildEndScreenMenu(void);

//creates end screen that displays operation time
void buildTimeScreen(void);

//creates end screen that displays balls dispensed
void ballsGivenScreen(int balls);

//creates end screen that displays the number of canisters
void numCanistersScreen(int num);

//creates end screen that allows user to select canister they want to see
void canisterSelectScreen(void);

//creates end screen that displays specific canister data
void canisterDataScreen(int id);

//For testing purposes, the following function is there to 
//initialize canister data:
void setCanisterData(void);

//For testing purposes to display sensor data on the screen:
void sensorData(int data, int data2);

//for debugging purposes to display if canister detected
void detectCanisterScreen(void);

//displays 2 data values permanently:
void printI2CData(int data1, int data2);

//displays timer count to test timer functionality
void displayTime(void);

//gets time value
float getTime(void);