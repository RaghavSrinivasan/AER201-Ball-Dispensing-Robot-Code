/**
 * By: Raghav Srinivasan
 *
 * Description: Contains main method of code for robot to execute. This is the 
 * highest level of control the robot will exhibit, in terms of how it will move
 * and react when it detects a canister
 */
#include <pic18f4620.h>

#include "menu.h"
#include "motion.h"
#include "sensors.h"
#include "data.h"
#include "EEPROM.h"
#include "UART_PIC.h"

#define distAwayTolerance 975
#define ballTolerance1 700
#define ballTolerance2 500
#define emptyTolerance 750
#define noMoveInThreshold 0

volatile int canID; //can we are on now
volatile float lateralDist;
volatile float suspendedDist;
volatile float forwardDist;
volatile float analog1Ambient;
volatile float analog2Ambient;
volatile float analog3Ambient;
volatile float analog4Ambient;

char ESPdata[2] = {1, 1};
//function prototypes:

void detectCanister(void);
void detectCanisterTurned(void);
void testDrive(void);
void testDrive2(void);
void testTurn(int count);
void setForward(int count);
void setBackward(int count);
void setRight(int count);
void dispSensors(void);
void printState(void);
void adjustCanister(void);
void dispSensors2(void);
void testShaftEncoder(void);

void main(void) {
    //NOTE: Subtract lateral movement from distance count!!!!
    //Setup
    //Variable Declaration:
    bool turned = false;
    bool finished = false;
    unsigned int newCount = 0;
    unsigned int count[2];
    
    //port management
    //this is for the revolving door motor
    TRISCbits.RC5 = 0;
    LATCbits.LATC5 = 0;
    
    //this is for the canister detection
    TRISBbits.RB3 = 0;
    LATBbits.LATB3 = 0;
 
    //UART INIT
    //UART_Init(9600);
    
    //set canisters
    initCanisters();
    //Timer:
    TMR0ON = 0; //disable timer in beginning
    T08BIT = 0; // Set to 16 bit counter
    // Count at positive edge of OSC0 (2.5MHz)
    T0CS = 0;
    T0SE = 0;
    //No prescaler
    PSA = 1;
    //reset timer0 registers
    TMR0L = 0b00000000;
    TMR0H = 0b00000000;
    
    TMR0IE = 1; //Enable Timer Interrupt (after overflow)
    
    moveRobot(BREAK);
    //get ambient sensor readings:
    analog1Ambient = distFromCanister(AnalogSensor1);
    analog2Ambient = distFromCanister(AnalogSensor2);
    analog3Ambient = distFromCanister(AnalogSensor3);
    analog4Ambient = distFromCanister(AnalogSensor4);
    initStartScreen();//Start Menus
    
    //testShaftEncoder();
    
    //releaseDoor();
    //dispSensors();
    //testDrive();
    
    TMR0ON = 1; //timer on
    __delay_ms(2000);
    //forward leg:
    testDrive();
    //turn:
    testTurn(21);
    setForward(48);
    testTurn(21);
    turned = true;
    canID = getNumCanisters(); //set canID to utilize in update canister method
    //leg after turn
    testDrive2();
    
    //End task
    moveRobot(BREAK);
    finished = true;
    TMR0ON = 0; //stop timer
    initMenu();  //End Menu
   
}

void detectCanister(void)
{
    int newDist, oldDist;
    int dist1, dist2, dist3, dist4, dist5;
    float avgDist = 0;
    int i;
    direction facing;
    float location, oldLocation;
    canister prevCan;
    int prevID;
    int state; //0 = close, 1 = open
    bool status, deposited;
    bool success; //if we successfully deposited ball
    bool right = false;
    
    LATBbits.LATB3 = 1; //send output canister detection signal
    
    //get previous canister distance, so we can compare distances:
    prevID = getNumCanisters();
    if (prevID >0)
    {
        prevCan = getData(prevID);
        oldLocation = prevCan.distance;
    }
    else
    {
        oldLocation = -30;
    }
    
    //Use initial location of canister when detected + distance to centerline 
    //(accounting for error)
    location = suspendedDist + 4;
    
    while(readDigitalSensor())//move forward until end of canister
    {
        moveRobot(FORWARD);
    }
    while(!readDigitalSensor())//move backward until end of canister
    {
        moveRobot(BACKWARD);
    }
    setBackward(1); //move back a bit more
    
    //move to a set distance from canister:
    dist2 = distFromCanister(AnalogSensor2);
    dist3 = distFromCanister(AnalogSensor3);
    dist4 = distFromCanister(AnalogSensor4);
    if (dist2 > noMoveInThreshold )
    {
        while ((dist3 > distAwayTolerance)|| (dist3 < distAwayTolerance))
        {
            if ((dist3 > distAwayTolerance))
                moveRobot(LEFT);
            else if ((dist3 < distAwayTolerance))
                moveRobot(RIGHT);

            dist3 = distFromCanister(AnalogSensor3);
            dist4 = distFromCanister(AnalogSensor4);
        }
    }
    //deposit ball:
    moveRobot(BREAK);
    __delay_ms(500);
    //sensors to detect ball:
    dist1 = distFromCanister(AnalogSensor1);
    dist2 = distFromCanister(AnalogSensor2); //for detecting ball
    //sensor to tell distance:
    dist3 = distFromCanister(AnalogSensor3);
    //sensor to detect back vs front
    dist4 = distFromCanister(AnalogSensor4);
    //discern canister state:
    if ((dist1 > ballTolerance1)&&(dist2 > ballTolerance2)&&(dist4 < emptyTolerance))
    {
        state = 1; //front of canister
    }
    else
    {
        state = 0; //back of canister
    }
    
    //logic for what to do in certain states:
    if (state == 0)
    {
        //facing the back or ball
        facing = 0; //facing left
        deposited = false;
        status = false; //unknown status

    }
    else if (state == 1)
    {
        facing = 1; //facing right
        setRight(5);
        if ((location - oldLocation) >= 30) //if away enough from prev can
        {
            //move forward slightly and release:
            moveRobot(BREAK);
            lcd_clear();
            lcd_home();
            printf("Depositing Ball");
            setForward(7);
            moveRobot(BREAK);
            success = releaseDoor();
            __delay_ms(1000);
            lcd_clear();
            //if timeout is triggered (if revolving door fails)
            if (success)
            {
                increaseNumBalls();
                status = true;
                deposited = true;
            }
            else
            {
                status = false;
                deposited = false;
            }
        }
        else
        {
            status = false;
            deposited = false;
        }
    }
    storeCanisterData(facing, location, status, deposited); //store canister data
    
    
    while(readDigitalSensor())//move forward until end of canister
    {
        moveRobot(FORWARD);
    }
    
    LATBbits.LATB3 = 0; //turn light off
}

void detectCanisterTurned(void)
{
    LATBbits.LATB3 = 1; //send output canister detection signal
    
    canister can;
    canister prevCan;
    float prevDist, curDist;
    int i;
    float avgDist;
    bool status, deposited;
    bool success;
    int dist1, dist2, dist3, dist4, dist5;
    int state;
   
    //can information to check canister state:
    can = getData(canID);
    if (canID > 1)
    {
        prevCan = getData(canID - 1);
        prevDist = (prevCan.distance);
    }
    else
    {
        prevDist = -50; //arbitrarily small
    }
    
    curDist = can.distance;
    
    while(readDigitalSensor())//move forward until end of canister
    {
        moveRobot(FORWARD);
    }
    while(!readDigitalSensor())//move backward until end of canister
    {
        moveRobot(BACKWARD);
    }
    setBackward(1); //move set distance back
    
    //move a set distance away from canister:
    dist2 = distFromCanister(AnalogSensor2);
    dist3 = distFromCanister(AnalogSensor3);
    dist4 = distFromCanister(AnalogSensor4);
    if (dist2 > noMoveInThreshold )
    {
        while ((dist3 > distAwayTolerance)|| (dist3 < distAwayTolerance))
        {
            if ((dist3 > distAwayTolerance))
                moveRobot(LEFT);
            else if ((dist3 < distAwayTolerance))
                moveRobot(RIGHT);

            dist3 = distFromCanister(AnalogSensor3);
            dist4 = distFromCanister(AnalogSensor4);
        }
    }
    
    moveRobot(BREAK);
    __delay_ms(500);
    //gets if there is ball:
    dist1 = distFromCanister(AnalogSensor1);
    dist2 = distFromCanister(AnalogSensor2); 
    //distance to can:
    dist3 = distFromCanister(AnalogSensor3);
    //detect back vs front
    dist4 = distFromCanister(AnalogSensor4); 
    
    //discern canister state:
    if ((dist1 > ballTolerance1)&&(dist2 > ballTolerance2)&&(dist4 < emptyTolerance))
    {
        state = 1; //front
    }
    else
    {
        state = 0; //back
    }
    
    //logic for what to do in each state
    if (state == 0)
    {
        if (can.facing == 0)//detected as blocked on other side, so seen as left
        {
            //if blocked on both sides, it has a ball in it
            status = true;
            deposited = false;
        }
        else
        {
            //keep canister state, as it was empty on other side 
            status = can.status;
            deposited = can.ballDeposited;
        }
    }
    else if (state == 1)
    {
        //move away from robot
        setRight(5);
        //dispense if appropriate:
        if ((curDist - prevDist) >= 30)
        {
            //deposit ball
            setForward(7);
            moveRobot(BREAK);
            success = releaseDoor();
            __delay_ms(1000);
            //new canister data:
            if (success)
            {
                increaseNumBalls();
                status = true;
                deposited = true;
            }
            else
            {
                status = false;
                deposited = false;
            }
        }
        else
        {
            //within 30cm, don't deposit
            status = false;
            deposited = false;
        }
    }
    updateCanData(canID, status, deposited);
    canID--;
    while(readDigitalSensor())//move forward until end of canister
    {
        moveRobot(FORWARD);
    }
    LATBbits.LATB3 = 0;
}

void testDrive(void)
{
    float newDist = getDistance() - lateralDist;
    float dist = newDist;
    //go a bit further than 4m to clear canisters
    while ((dist - newDist) <= 415)
    {
        moveRobot(FORWARD);
        dist = getDistance() - lateralDist;
        lcd_clear();
        lcd_home();
        printf("%.2f", dist); //display distance
        //if we detect something, trigger detect canister method:
        if (readDigitalSensor())
        {
            //lateralDist to not account for lateral motion during subroutine
            suspendedDist = getDistance() - lateralDist;
            lateralDist -= getDistance();
            detectCanister();
            lateralDist += getDistance();
            lateralDist -= 13;
        }
        //avoid canister if front sensor detected
        if(readFrontSensor() || readFrontSensor2())
        {
            lateralDist -= getDistance();
            avoidCan();
            lateralDist += getDistance();
        }
        //if remote start pressed, stop
        if (readRemoteStart() == 1)
        {
            //End task
            moveRobot(BREAK);
            TMR0ON = 0; //stop timer
            initMenu();  //End Menu
        }
       
    }
}

void testDrive2(void)
{
    float newDist = getDistance() - lateralDist;
    float dist = newDist;
    //go a bit further than 4m to clear canisters
    while ((dist - newDist) <= 415)
    {
        moveRobot(FORWARD);
        dist = getDistance() - lateralDist;
        lcd_clear();
        lcd_home();
        printf("%.2f", dist); //display distance
        //if we detect something, trigger detect canister method:
        if (readDigitalSensor())
        {
            //lateralDist to not account for lateral motion during subroutine
            suspendedDist = getDistance() - lateralDist;
            lateralDist -= getDistance();
            detectCanisterTurned();
            lateralDist += getDistance();
            lateralDist -= 13;
        }
        //avoid canister if front sensor detected
        if(readFrontSensor() || readFrontSensor2())
        {
            lateralDist -= getDistance();
            avoidCan();
            lateralDist += getDistance();
        }
        //if remote start pressed, stop
        if (readRemoteStart() == 1)
        {
            //End task
            moveRobot(BREAK);
            TMR0ON = 0; //stop timer
            initMenu();  //End Menu
        }
       
    }
}

//turns for a specific count on the shaft encoder
void testTurn(int count)
{
    unsigned int shaftCount = shaftEncoderCount();
    int newCount = shaftCount;
    unsigned int shaftCount2 = shaftEncoderCount2();
    while ((shaftCount - newCount) <= count)//set turn
    {
        moveRobot(CCW);
        shaftCount = shaftEncoderCount();
        shaftCount2 = shaftEncoderCount2();
        sensorData(shaftCount, shaftCount2);
    }
}

//move forward for specific distance
void setForward(int count)
{
    unsigned int shaftCount = shaftEncoderCount();
    int newCount = shaftCount;
    unsigned int shaftCount2 = shaftEncoderCount2();
    while ((shaftCount - newCount) <= count)//set count
    {
        moveRobot(FORWARD);
        shaftCount = shaftEncoderCount();
        shaftCount2 = shaftEncoderCount2();
        sensorData(shaftCount, shaftCount2);
    }
}

//move backward for specific distance
void setBackward(int count)
{
    unsigned int shaftCount = shaftEncoderCount();
    int newCount = shaftCount;
    unsigned int shaftCount2 = shaftEncoderCount2();
    while ((shaftCount - newCount) <= count)//set count
    {
        moveRobot(BACKWARD);
        shaftCount = shaftEncoderCount();
        shaftCount2 = shaftEncoderCount2();
        sensorData(shaftCount, shaftCount2);
    }
}

//move right for specific distance
void setRight(int count)
{
    unsigned int shaftCount = shaftEncoderCount();
    int newCount = shaftCount;
    unsigned int shaftCount2 = shaftEncoderCount2();
    while ((shaftCount - newCount) <= count)//set count
    {
        moveRobot(RIGHT);
        shaftCount = shaftEncoderCount();
        shaftCount2 = shaftEncoderCount2();
        sensorData(shaftCount, shaftCount2);
    }
}

//to debug sensors, display sensor values
void dispSensors(void)
{
    int dist1,dist2,dist3,dist4,dist5;
    int digDist;
    lcd_clear();
    while (1){
        dist1 = distFromCanister(AnalogSensor1);
        dist2 = distFromCanister(AnalogSensor2);
        dist3 = distFromCanister(AnalogSensor3);
        dist4 = distFromCanister(AnalogSensor4);
        dist5 = readRemoteStart();
        digDist = readDigitalSensor();
        lcd_home();
        printf("1:%d 2:%d", dist1, dist2);
        lcd_set_ddram_addr(LCD_LINE2_ADDR);
        printf("3:%d 4:%d", dist3,dist4);
        lcd_set_ddram_addr(LCD_LINE3_ADDR);
        printf("D:%d", digDist);
        __delay_ms(200);
        lcd_clear();
    }
}

//moves for set shaft encoder count to match distances to counts
void testShaftEncoder(void)
{
    unsigned int shaftCount = shaftEncoderCount();
    int newCount = shaftCount;
    unsigned int shaftCount2 = shaftEncoderCount2();
    float newDist = getDistance();
    float dist = newDist;
    //300 is an arbitrary count, that is sufficiently  large
    while ((shaftCount - newCount) <= 300)
    {
        moveRobot(FORWARD);
        shaftCount = shaftEncoderCount();

        shaftCount2 = shaftEncoderCount2();
        sensorData(shaftCount, shaftCount2);
       
    }
    moveRobot(BREAK);
    //display shaft encoder count that is ended
    shaftCount = shaftEncoderCount();
    while (1)
    {
        lcd_clear();
        lcd_home();
        printf("%d", shaftCount);
        __delay_ms(1000);
    }
}