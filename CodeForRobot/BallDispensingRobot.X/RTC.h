/* 
 * File:   RTC.h
 * Author: Raghav Srinivasan
 *
 * Created on February 6, 2019, 1:09 AM
 * Reads the RTC module using I2C
 */

#include <xc.h>
#include <stdio.h>
#include <configBits.h>
#include "I2C.h"
#include "lcd.h"

unsigned char* getRTCTime(void);
void rtc_set_time(void);