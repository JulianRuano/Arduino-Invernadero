/**
  * @file utilities.h
  * @version 1.0
  * @date 09/12/2023
  * @author Ledy Astudillo <lmastudillo@unicauca.edu.co>- Julian majin <julianruano@unicauca.edu.co>
  * @title utilitiespage greenhouse Project 
  * @brief utilities project file.
*/
// Prevents multiple file inclusion
#ifndef UTILITIES_H
#define UTILITIES_H
// Includes libraries
#include <AverageValue.h>   
#include "DHT.h" 
#include <LiquidCrystal.h>

// Define pins
#define BUZZER 5


// LCD display declaration
extern LiquidCrystal lcd;

/** 
  @brief State enum 
 */
enum State {
    intState = 0,   
    blocked = 1,    
    monitorTH = 2,   
    alarm = 3,   
    monitorLight = 4, 
};

/** 
  @brief Input enum
 */
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

/** 
  @brief Global variables declarations
 */
extern int LED_RED;
extern int LED_GREEN;
extern int LED_BLUE;

extern unsigned int beginingTime;
extern unsigned int currentTime;
extern short int light;
extern int conditionRunway;

// Sensors 
extern DHT dht;
// Values averages  
const long MAX_VALUES_NUM = 5;
extern AverageValue<long> averageTemperatureValue;
extern AverageValue<long> averageHumidityValue;
extern AverageValue<long> cleanAverageValue;
extern AverageValue<long> averageLightValue;
// Function prototypes 
void assignColor(unsigned char red, unsigned char green, unsigned char blue);
void clearAndPrintPrompt(const char *promptLabel);
// End preprocessor directives
#endif // UTILITIES_H