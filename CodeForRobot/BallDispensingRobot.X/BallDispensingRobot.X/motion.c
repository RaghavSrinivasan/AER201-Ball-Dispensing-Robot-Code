   /* 
 * File:   motion.c
 * Author: Raghav Srinivasan
 * Date: February 6, 2019
 */

#include <xc.h>
#include "motion.h"
#include "menu.h"

void moveRobot(int move)
{
    //Set RC0:2 to output, as these are the pins that
    //interface with Martin's Logic Circuit
    TRISCbits.RC0 = 0;
    TRISCbits.RC1 = 0;
    TRISCbits.RC2 = 0;
    
    //fire required pins based on Logic Circuit by Martin 
    //(refer to proposal section 5.2.7.3)
    switch(move)
    {
        case BREAK:
            LATCbits.LATC0 = 0;
            LATCbits.LATC1 = 0;
            LATCbits.LATC2 = 0;
            break;
        case FORWARD:
            LATCbits.LATC0 = 0;
            LATCbits.LATC1 = 0;
            LATCbits.LATC2 = 1;
            break;
        case BACKWARD:
            LATCbits.LATC0 = 0;
            LATCbits.LATC1 = 1;
            LATCbits.LATC2 = 0;
            break;
        case LEFT:
            LATCbits.LATC0 = 0;
            LATCbits.LATC1 = 1;
            LATCbits.LATC2 = 1;
            break;
        case RIGHT:
            LATCbits.LATC0 = 1;
            LATCbits.LATC1 = 0;
            LATCbits.LATC2 = 0;
            break;
        case CCW:
            LATCbits.LATC0 = 1;
            LATCbits.LATC1 = 0;
            LATCbits.LATC2 = 1;
            break;
        case CW:
            LATCbits.LATC0 = 1;
            LATCbits.LATC1 = 1;
            LATCbits.LATC2 = 0;
    }
}

bool releaseDoor(void)
{
    if (readHallEffect() == 1)
        return releaseDoor1();
    else
        return releaseDoor2();
}
bool releaseDoor1(void)
{
    TRISCbits.RC5 = 0;
    
    float start = getTime(); //for timeout
    while (readHallEffect() && (getTime() - start) < JamTime)
    {
        LATCbits.LATC5 = 1;
    }
    if ((getTime() - start) > JamTime)
    {
            LATCbits.LATC5 = 0;
            return false;
    }
    LATCbits.LATC5 = 0;
    return true;
}
bool releaseDoor2(void)
{
    TRISCbits.RC5 = 0;
    
    float start = getTime();
    
    while (readHallEffect() == 0 && (getTime() - start) < JamTime)
    {
        LATCbits.LATC5 = 1;
        if (readHallEffect()==1)
        {
            break;
        }
    }
    if ((getTime() - start) > JamTime)
    {
        LATCbits.LATC5 = 0;
        return false;
    }
    start = getTime();
    while (readHallEffect() && (getTime() - start) < JamTime)
    {
        LATCbits.LATC5 = 1;
        
    }
    if ((getTime() - start) > JamTime)
    {
        LATCbits.LATC5 = 0;
        return false;
    }
    LATCbits.LATC5 = 0;
    return true;
}

void avoidCan(void)
{
    moveRobot(RIGHT);
    __delay_ms(500);//move right for a short distance
    while (readFrontSensor())
        moveRobot(RIGHT);
    moveRobot(BREAK);
}