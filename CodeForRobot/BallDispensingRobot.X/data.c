/* 
 * File:  data.c
 * Author: Raghav Srinivasan
 * Date: February 6, 2019
 * Description: This file contains the global variable, cans, which is the 
 * array of canister structs, where each element represents a canister, 
 * and its inherent data
 */

#include "data.h"

//field variables:
volatile canister cans[10];
volatile int numCans; //number of canisters
volatile int numBalls; //number of balls

//reset canister data (IE all of them are null)
void initCanisters(void)
{
    int i = 0;
    for (i = 0; i < 10; i++)
    {
        (cans[i]).id = 0; //id of 0 means canister is not set
        (cans[i]).facing = Left;
        (cans[i]).distance = 0;
        (cans[i]).status = false;
        (cans[i]).ballDeposited = false;
    }
    numCans = 0;
}
//store data to array when this is first time encountering canister
void storeCanisterData(direction facing, float dist, bool status, bool deposit)
{
    numCans++;
    int id = numCans;
    (cans[id-1]).id = id;
    (cans[id-1]).facing = facing;
    (cans[id-1]).distance = dist;
    (cans[id-1]).status = status;
    (cans[id-1]).ballDeposited = deposit;
}

//update canister data after turn:
void updateCanData(int id, bool status, bool deposit)
{
    (cans[id-1]).status = status;
    (cans[id-1]).ballDeposited = deposit;
}

//getter methods:
canister getData(int id)
{
    return cans[id - 1];
}

int getNumCanisters(void)
{
    return numCans; 
}

int getNumBallsGiven(void)
{
    return numBalls;
}

//increase ball count:
void increaseNumBalls(void)
{
    numBalls++;
}