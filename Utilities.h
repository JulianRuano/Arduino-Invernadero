#ifndef UTILITIES_H
#define UTILITIES_H

#include <AverageValue.h>   
#include "DHT.h" 
#include <LiquidCrystal.h>

#define BUZZER 5

extern LiquidCrystal lcd;


enum State {
    intState = 0,   
    blocked = 1,    
    monitorTH = 2,   
    alarm = 3,   
    monitorLight = 4, 
};



enum Input
{
    InInit = 0,
    InBlocked = 1,
    InMonitorTH = 2,
    InAlarm = 3,
    InLight = 4,
    TimeOut5 = 5,
    TimeOut6 = 6,
    TimeOut7 = 7
};

extern int LED_RED;
extern int LED_GREEN;
extern int LED_BLUE;

extern unsigned int beginingTime;
extern unsigned int currentTime;
extern short int light;
extern int conditionRunway;

extern DHT dht;

const long MAX_VALUES_NUM = 5;
extern AverageValue<long> averageTemperatureValue;
extern AverageValue<long> averageHumidityValue;
extern AverageValue<long> cleanAverageValue;
extern AverageValue<long> averageLightValue;

void assignColor(unsigned char red, unsigned char green, unsigned char blue);
void clearAndPrintPrompt(const char *promptLabel);

#endif // UTILITIES_H