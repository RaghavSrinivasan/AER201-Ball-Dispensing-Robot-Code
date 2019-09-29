/* 
 * File:   EEPROM.h
 * Author: Raghav Srinivasan
 * Description: To Write and Read to EEPROM
 * Created on March 22, 2019, 10:32 PM
 */
#include <xc.h>
#include <configbits.h>


void myEEPROM_write(unsigned int address, unsigned char data);
unsigned char myEEPROM_read(unsigned int address);
//void EEPROM_logOperation(struct opInfo *data);
//int EEPROM_readLog(unsigned int logNum, struct opInfo *data);
//void EEPROM_clear(void);



