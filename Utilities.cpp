#include "Utilities.h"
#include "Arduino.h"
 

Input input;

//LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);


int LED_RED = 49;  
int LED_GREEN = 51; 
int LED_BLUE = 53; 
//int LED_RED = 8;  
//int LED_GREEN = 7; 
//int LED_BLUE = 6;  

unsigned int beginingTime = 0;
unsigned int currentTime = 0;
short int light = 0.0;
int conditionRunway = 0;


//DHT dht(A5, DHT11); 
DHT dht(A1, DHT11); 


AverageValue<long> averageTemperatureValue(MAX_VALUES_NUM);
AverageValue<long> averageHumidityValue(MAX_VALUES_NUM);
AverageValue<long> cleanAverageValue(MAX_VALUES_NUM);
AverageValue<long> averageLightValue(MAX_VALUES_NUM);


void assignColor(unsigned char red, unsigned char green, unsigned char blue) {
    analogWrite(LED_RED, red);
    analogWrite(LED_BLUE, blue);
    analogWrite(LED_GREEN, green);
}

void clearAndPrintPrompt(const char *promptLabel) {
    lcd.clear();
    lcd.print(promptLabel);
}