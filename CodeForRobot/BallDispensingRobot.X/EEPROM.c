/* 
 * File:   EEPROM.c
 * Author: Raghav Srinivasan
 * Description: To Write and Read to EEPROM
 * Created on March 22, 2019, 10:32 PM
 */

#include "EEPROM.h"

void EEPROM_write(unsigned int address, unsigned char data) {
    EECON1bits.WREN = 1;    // Allow write cycles to EEPROM / Flash
    
    // Set address registers
    EEADRH = (unsigned char)(address >> 8);     // Take the most significant 8 bits (6 MSB are empty)
    EEADR = (unsigned char)address;
    
    EEDATA = data;
    EECON1bits.EEPGD = 0;   // Access EEPROM instead of flash memory
    EECON1bits.CFGS = 0;    // Access EEPROM/Flash, not config registers
    
    di();                   // Disable interrupts during initial sequence
    EECON2 = 0x55;
    EECON2 = 0x0AA;
    EECON1bits.WR = 1;      // Write control bit: initiate EEPROM write cycle
    ei();
    
    // Poll for EEPROM completion, on the EEIF interrupt flag
    while (PIR2bits.EEIF == 0) {continue;}
    PIR2bits.EEIF = 0;      // Clear the interrupt flag
    EECON1bits.WREN = 0;    // Disallow write cycles to EEPROM / Flash   
}

unsigned char EEPROM_read(unsigned int address) {
    // Set address registers
    EEADRH = (unsigned char)(address >> 8);
    EEADR = (unsigned char)address;
    
    EECON1bits.EEPGD = 0;    // Access EEPROM instead of flash memory
    EECON1bits.RD = 1;       // Read control bit: initiate EEPROM read cycle
    
    // Read control bit cleared by hardware after one cycle
    while (EECON1bits.RD == 1) {continue;}
    return EEDATA;
}
