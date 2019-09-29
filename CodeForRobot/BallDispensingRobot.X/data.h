/* 
 * File:   data.h
 * Author: Raghav Srinivasan
 * Date: February 6, 2019
 * Description: This file contains the struct definition for the canisters, as
 * well as the function prototypes for the data retrieval functions
 */

#include <stdbool.h>
#include <stdlib.h>

//direction enum:
enum direction {Left = 0, Right = 1};
typedef enum direction direction;

//struct definition:
struct canister{
    int id;
    direction facing;
    float distance;
    bool status;
    bool ballDeposited;
};

typedef struct canister canister;

//resetting canister data:
void initCanisters(void);

//setter method when first encountering canister:
void storeCanisterData(direction facing, float dist, bool status, bool deposit);
//setter method when updating canister data after turn:
void updateCanData(int id, bool status, bool deposit);
//getter methods to retrieve data:
canister getData(int id);
int getNumCanisters(void);
int getNumBallsGiven(void);
//method to manually increase ball count when deployed
void increaseNumBalls(void);