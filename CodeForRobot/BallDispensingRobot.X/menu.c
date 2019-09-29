/*
 * Author: Raghav Srinivasan
 * Description: Creates Menu and Keyboard Interface for User. As the interrupt
 * handler for the project is in this file, this also contains the method for 
 * obtaining timer value, as it utilizes interrupts
 */

#include "menu.h"
#include "RTC.h"
#include "data.h"

//global variables to use in interrupt:
volatile bool key_pressed = false; //key press
volatile unsigned char key;
volatile long int timeCount;


//creates start screen. This displays start method and RTC time, and moves on
//when there is a remote start or a key press
void initStartScreen(void)
{
    // RD2 is the character LCD RS
    // RD3 is the character LCD enable (E)
    // RD4-RD7 are character LCD data lines
    LATD = 0x00;
    TRISD = 0x00;
    
    // Set all A/D ports to digital (pg. 222)
    ADCON1 = 0b00001111;
    
    //obtain input from RB4:7 to find what key was pressed
    TRISBbits.RB7 = 1;
    TRISBbits.RB6 = 1;
    TRISBbits.RB5 = 1;
    TRISBbits.RB4 = 1;
    
    //Enable Keyboard Interrupts:
    INTCONbits.GIE = 1;
    INTCON3bits.INT1E = 1;
    ei();
    
    //RTC initialize:
    I2C_Master_Init(100000);
    unsigned char time[7]; // Create a byte array to hold time read from RTC
    
    initLCD();
    while (1)
    {
        
        // Reset RTC memory pointer
        I2C_Master_Start(); // Start condition
        I2C_Master_Write(0b11010000); // 7 bit RTC address + Write
        I2C_Master_Write(0x00); // Set memory pointer to seconds
        I2C_Master_Stop(); // Stop condition

        // Read current time
        I2C_Master_Start(); // Start condition
        I2C_Master_Write(0b11010001); // 7 bit RTC address + Read
        for(unsigned char i = 0; i < 6; i++){
            time[i] = I2C_Master_Read(ACK); //Read with ACK to continue reading
        }
        time[6] = I2C_Master_Read(NACK); // Final Read with NACK
        I2C_Master_Stop(); // Stop condition
        
        //start screen and RTC data on LCD
        lcd_home();
        printf("Press Any Key");
        lcd_set_ddram_addr(LCD_LINE2_ADDR);
        printf("To Begin");
        lcd_set_ddram_addr(LCD_LINE3_ADDR);
       
        //Print date in YY/MM/DD
        printf("%02x/%02x/%02x", time[6],time[5],time[4]); 
        lcd_set_ddram_addr(LCD_LINE4_ADDR);
        printf("%02x:%02x:%02x", time[2],time[1],time[0]); // HH:MM:SS

        __delay_ms(500);
        //exit menu when necessary:
        if (key_pressed || (readRemoteStart() == 1))
        {
            key_pressed = false;
            break;
        }
    }
    lcd_clear();
    
    
}

void initMenu(void)
{
    //setCanisterData(); //instantiate canisters for testing of data structures
    
    menState curr = end;
    //placeholders for actual values from run. To be replaced with 
    //function to retrieve data about run
    //key id (ie what is the numerical value of the key that was pressed)
    int id = 1; 
    int timeElapse = TMR1L;
    int ballGiven = getNumBallsGiven();
    int canisters = getNumCanisters();
     // RD2 is the character LCD RS
    // RD3 is the character LCD enable (E)
    // RD4-RD7 are character LCD data lines
    LATD = 0x00;
    TRISD = 0x00;
    
    // Set all A/D ports to digital (pg. 222)
    ADCON1 = 0b00001111;
    
    //obtain input from RB4:7 to find what key was pressed
    TRISBbits.RB7 = 1;
    TRISBbits.RB6 = 1;
    TRISBbits.RB5 = 1;
    TRISBbits.RB4 = 1;
    
    //Enable Keyboard Interrupts:
    INTCONbits.GIE = 1;
    INTCON3bits.INT1E = 1;
    ei();
    
    
    
    // Initialize LCD
    initLCD();
    
    buildEndScreen();
    
    while (1)
    {
        //change of state if relevant key was pressed:
        if (key_pressed)
        {
            key_pressed = false; //clear condition
            lcd_clear();
            //conditions for changing screen:
            switch(curr)
            {
                case end:
                    curr = endMenu;
                    break;
                case endMenu:
                    if (key == BACK)
                        curr = end;
                    else if (key == ONE)
                        curr = time;
                    else if (key == TWO)
                        curr = ballsGive;
                    else if (key == THREE)
                        curr = numCan;
                    else if (key == FOUR)
                        curr = canSel;
                    break;
                case time:
                case ballsGive:
                case numCan:
                    if (key == BACK)
                        curr = endMenu;
                    break;
                case canSel:
                    //check if user pressed back, else check if they pressed a 
                    //number:
                    if (key == BACK)
                        curr = endMenu;
                    else if (key == ONE)
                    {
                          id = 1;
                          curr = canData;
                    }
                    else if (key == TWO)
                    {
                          id = 2;
                          curr = canData;
                    }
                    else if (key == THREE)
                    {
                          id = 3;
                          curr = canData;
                    }
                    else if (key == FOUR)
                    {
                          id = 4;
                          curr = canData;
                    }
                    else if (key == FIVE)
                    {
                          id = 5;
                          curr = canData;
                    }
                    else if (key == SIX)
                    {
                          id = 6;
                          curr = canData;
                    }
                    else if (key == SEVEN)
                    {
                          id = 7;
                          curr = canData;
                    }
                    else if (key == EIGHT)
                    {
                          id = 8;
                          curr = canData;
                    }
                    else if (key == NINE)
                    {
                          id = 9;
                          curr = canData;
                    }
                    else if (key == ZERO)
                    {
                        id = 10;
                        curr = canData;
                    }
                    break;
                case canData:
                    if (key == BACK)
                        curr = canSel;
                    
                    
            }
        }
        
        //print current screen:
        switch(curr)
        {
            case end:
                buildEndScreen();
                break;
            case endMenu:
                buildEndScreenMenu();
                break;
            case time:
                buildTimeScreen();
                break;
            case ballsGive:
                ballsGivenScreen(ballGiven);
                break;
            case numCan:
                numCanistersScreen(canisters);
                break;
            case canSel:
                canisterSelectScreen();
                break;
            case canData:
                canisterDataScreen(id);
                break;
            default:
                lcd_home();
                printf("Error");
        }
    }
}


//The following functions are the different screens of the end menu:
void buildEndScreen(void)
{
    canister can = getData(1);
    float dist = can.distance;
    lcd_home();
    printf("Task Completed");
    //printf("Distance: %f", dist);
    lcd_set_ddram_addr(LCD_LINE3_ADDR);
    printf("Press Any Key");
    lcd_set_ddram_addr(LCD_LINE4_ADDR);
    printf("To Continue");
}

void buildEndScreenMenu(void)
{
    lcd_home();
    printf("1.Operation Time");
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    printf("2.# Balls Given");
    lcd_set_ddram_addr(LCD_LINE3_ADDR);
    printf("3.# Canisters");
    lcd_set_ddram_addr(LCD_LINE4_ADDR);
    printf("4.Canister Data");
    
}

void buildTimeScreen(void)
{
    //display time
    //frequency of time counter = (2^18)/2500 000
    float time = timeCount*0.0262144;
    lcd_home();
    printf("Time: %3.2f s", time);
}

void ballsGivenScreen(int balls)
{
    lcd_home();
    printf("# Balls Given:%d", balls);
}

void numCanistersScreen(int num)
{
    lcd_home();
    printf("# Canisters:%d", num);
}

void canisterSelectScreen(void)
{
    lcd_home();
    printf("Press # of ");
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    printf("Canister to be");
    lcd_set_ddram_addr(LCD_LINE3_ADDR);
    printf("Selected");
}

void canisterDataScreen(int id)
{
    canister can = getData(id);
    lcd_home();
    //a canister id of 0 indicates canister numbers not this high
    if (can.id == 0)
    {
        printf("ERROR INVALID");
    }
    else
    {
        printf("Canister: %d", id);

        //ideally for the following values, 
        //we would read them from memory using the id
        lcd_set_ddram_addr(LCD_LINE2_ADDR);
        if (can.status == false)    
            printf("Status: %s", "Empty");
        else
            printf("Status: Full");
        lcd_set_ddram_addr(LCD_LINE3_ADDR);
        if (can.ballDeposited == false)
            printf("Receive Ball?: %c", 'N');
        else
            printf("Receive Ball?: Y");
        lcd_set_ddram_addr(LCD_LINE4_ADDR);
        float dist = can.distance;
        printf("Distance: %d cm", (int) dist);
    }
}

//For testing purposes to display sensor data on the screen:
void sensorData(int data, int data2)
{
    lcd_clear();
    lcd_home();
    printf("Distance: %d cm", data);
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    printf("Distance2: %d cm", data2);
}

void printI2CData(int data1, int data2)
{
    lcd_clear();
    lcd_home();
    printf("count[0]: %d", data1);
    lcd_set_ddram_addr(LCD_LINE2_ADDR);
    printf("count[1]: %d", data2);
}
//For testing purposes, the following 
//function is there to initialize canister data:
void setCanisterData(void)
{
    storeCanisterData(Right, 180, true, true);
    storeCanisterData(Left, 40, false, false);
    storeCanisterData(Right, 420, true, false);
    storeCanisterData(Left, 69, false, false);
    storeCanisterData(Right, 4, true, true);
}

void detectCanisterScreen(void)
{
    lcd_clear();
    lcd_home();
    printf("Detected Canister");
}

float getTime(void)
{
    float time = timeCount*0.0262144; //constant based on prescaler used
    return time;
}
//The following function is for Interrupts:
void __interrupt() interruptHandler(void)
{
    //interrupt caused by keypad press
    if (INT1F)
    {
        INT1F = 0; //clear interrupt
        key_pressed = true;
        key = (PORTB >> 4); //get top for bits of port B for RB7:4
    }
    //interrupt caused by timer
    if (TMR0IF)
    {
        timeCount++;
        TMR0IF = 0;
    }
}