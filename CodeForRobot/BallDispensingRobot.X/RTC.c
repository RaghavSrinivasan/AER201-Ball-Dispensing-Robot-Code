/* 
 * File:   RTC.c
 * Author: Raghav Srinivasan
 *
 * Created on February 6, 2019, 1:09 AM
 * Reads the RTC module using I2C, using code from Tyler Gamvrellis as a basis 
 * for operation. All 
 */

#include"RTC.h"

const char year[7] = {
    0x00, // 0 Seconds 
    0x40, // 30 Minutes
    0x01, // 24 hour mode, set to 1:35
    0x04, // Wednesday
    0x06, // 23rd
    0x2, // February
    0x19  // 2019
};

//funtion uses I2C protocol to read data from RTC and return it as a pointer to the
//first element of the created char array
unsigned char* getRTCTime(void)
{
    unsigned char time[7]; // Create a byte array to hold time read from RTC
    // Initialize I2C Master with 100 kHz clock
    I2C_Master_Init(100000);
    // Reset RTC memory pointer
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b11010000); // 7 bit RTC address + Write
    I2C_Master_Write(0x00); // Set memory pointer to seconds
    I2C_Master_Stop(); // Stop condition
    // Read current time
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b11010001); // 7 bit RTC address + Read
    for(unsigned char i = 0; i < 6; i++){
        time[i] = I2C_Master_Read(ACK); // Read with ACK to continue reading
    }
    time[6] = I2C_Master_Read(NACK); // Final Read with NACK
    I2C_Master_Stop(); // Stop condition
    
    return &(time[0]); //return value read from RTC
}

/** @brief Writes the year array to the RTC memory */
void rtc_set_time(void){
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b11010000); //7 bit RTC address + Write
    I2C_Master_Write(0x00); // Set memory pointer to seconds
    
    // Write array
    for(char i=0; i < 7; i++){
        I2C_Master_Write(year[i]);
    }
    
    I2C_Master_Stop(); //Stop condition
}