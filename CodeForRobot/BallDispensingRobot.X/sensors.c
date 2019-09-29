/*
 * Author: Raghav Srinivasan
 * Description: Sensor interface with Arduino
 */

#include "sensors.h"
#include "lcd.h"

int shaftEncoderCount(void)
{
    unsigned int count[2];
    I2C_Master_Init(100000); //i2C clock
    
    //Data we wish to request:
    
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
    I2C_Master_Write(RotaryEncoder1); //define RotaryEncoder1 = 0 (id of sensor on Arduino)
    I2C_Master_Stop();
    
    //read Data
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b00010001); //address + read
    
    for (unsigned char i = 0; i < 1; i++)
    {
        count[i] = I2C_Master_Read(ACK); // Read with ACK to continue reading
    }
    count[1] = I2C_Master_Read(NACK);
    I2C_Master_Stop();
    
    return (count[0])|(count[1]<<8);
}

int shaftEncoderCount2(void)
{
    unsigned int count[2];
    I2C_Master_Init(100000); //i2C clock
    
    //Data we wish to request:
    
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
    I2C_Master_Write(RotaryEncoder2);
    I2C_Master_Stop();
    
    //read Data
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b00010001); //address + read
    
    for (unsigned char i = 0; i < 1; i++)
    {
        count[i] = I2C_Master_Read(ACK); // Read with ACK to continue reading
    }
    count[1] = I2C_Master_Read(NACK);
    I2C_Master_Stop();
    
    return (count[0])|(count[1]<<8);
}

int readRemoteStart(void)
{
    unsigned int count[2];
    I2C_Master_Init(100000); //i2C clock
    
    //Data we wish to request:
    
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
    I2C_Master_Write(RemoteStart); 
    I2C_Master_Stop();
    
    __delay_ms(20);
    //read Data
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b00010001); //address + read
    
    for (unsigned char i = 0; i < 1; i++)
    {
        count[i] = I2C_Master_Read(ACK); // Read with ACK to continue reading
    }
    count[1] = I2C_Master_Read(NACK);
    I2C_Master_Stop();
    
    return (count[0])|(count[1]<<8);
}

int distFromCanister(int sensor)
{
    unsigned int count[2];
    I2C_Master_Init(100000); //i2C clock
    
    if (sensor == AnalogSensor1)
    {
        I2C_Master_Start(); // Start condition
        I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
        I2C_Master_Write(AnalogSensor1);
        I2C_Master_Stop();
    }
    else if (sensor == AnalogSensor2)
    {
        I2C_Master_Start(); // Start condition
        I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
        I2C_Master_Write(AnalogSensor2); 
        I2C_Master_Stop();
    }
    else if (sensor == AnalogSensor3)
    {
        I2C_Master_Start(); // Start condition
        I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
        I2C_Master_Write(AnalogSensor3); 
        I2C_Master_Stop();
    }
    else if (sensor == AnalogSensor4)
    {
        I2C_Master_Start(); // Start condition
        I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
        I2C_Master_Write(AnalogSensor4); 
        I2C_Master_Stop();
    }
    else
    {
        return -1;
    }
    
    //read Data
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b00010001); //address + read
    
    for (unsigned char i = 0; i < 1; i++)
    {
        count[i] = I2C_Master_Read(ACK); // Read with ACK to continue reading
    }
    count[1] = I2C_Master_Read(NACK);
    I2C_Master_Stop();
    
    return (count[0])|(count[1]<<8);
}

int readDigitalSensor(void)
{
    unsigned int count[2];
    I2C_Master_Init(100000); //i2C clock
    
    //Data we wish to request:
    
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
    I2C_Master_Write(DigitalSensor); //(id of sensor on Arduino)
    I2C_Master_Stop();
    
    //read Data
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b00010001); //address + read
    
    for (unsigned char i = 0; i < 1; i++)
    {
        count[i] = I2C_Master_Read(ACK); // Read with ACK to continue reading
    }
    count[1] = I2C_Master_Read(NACK);
    I2C_Master_Stop();
    
    return (count[0])|(count[1]<<8);
}

int readHallEffect(void)
{
    unsigned int count[2];
    I2C_Master_Init(100000); //i2C clock
    
    //Data we wish to request:
    
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
    I2C_Master_Write(HallEffect); 
    I2C_Master_Stop();
    
    //read Data
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b00010001); //address + read
    
    for (unsigned char i = 0; i < 1; i++)
    {
        count[i] = I2C_Master_Read(ACK); // Read with ACK to continue reading
    }
    count[1] = I2C_Master_Read(NACK);
    I2C_Master_Stop();
    
    return (count[0])|(count[1]<<8);
}


int readFrontSensor(void)
{
    unsigned int count[2];
    I2C_Master_Init(100000); //i2C clock
    
    //Data we wish to request:
    
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
    I2C_Master_Write(FrontSensor); 
    I2C_Master_Stop();
    
    //read Data
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b00010001); //address + read
    
    for (unsigned char i = 0; i < 1; i++)
    {
        count[i] = I2C_Master_Read(ACK); // Read with ACK to continue reading
    }
    count[1] = I2C_Master_Read(NACK);
    I2C_Master_Stop();
    
    return (count[0])|(count[1]<<8);
}

int readFrontSensor2(void)
{
    unsigned int count[2];
    I2C_Master_Init(100000); //i2C clock
    
    //Data we wish to request:
    
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
    I2C_Master_Write(FrontSensor2); 
    I2C_Master_Stop();
    
    //read Data
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b00010001); //address + read
    
    for (unsigned char i = 0; i < 1; i++)
    {
        count[i] = I2C_Master_Read(ACK); // Read with ACK to continue reading
    }
    count[1] = I2C_Master_Read(NACK);
    I2C_Master_Stop();
    
    return (count[0])|(count[1]<<8);
}

void senseTest(void)
{
    unsigned int count[2];
    I2C_Master_Init(100000); //i2C clock
    
    //Data we wish to request:
    
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
    I2C_Master_Write(0x1);
    I2C_Master_Stop();
    
    //read Data
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b00010001); //address + read
    
    for (unsigned char i = 0; i < 1; i++)
    {
        count[i] = I2C_Master_Read(ACK); // Read with ACK to continue reading
    }
    count[1] = I2C_Master_Read(NACK);
    I2C_Master_Stop();
    
    lcd_home();
    lcd_clear();
    printf("1: %d, 2: %d", count[0], count[1]);
}

float getDistance(void)
{
    int count = shaftEncoderCount();
    return 1.409743*count;
}

float getDistance2(void)
{
    int count = shaftEncoderCount2();
    return 1.65*count;
}

void sendData(void)
{
    I2C_Master_Init(100000); //i2C clock
    
    //Data we wish to request:
    
    I2C_Master_Start(); // Start condition
    I2C_Master_Write(0b00010000); // 7-bit Arduino slave address + write
    I2C_Master_Write(12); //define RotaryEncoder1 = 0 (id of sensor on Arduino)
    I2C_Master_Stop();
}
