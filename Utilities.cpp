#include "Utilities.h"
#include <Arduino.h>

const int LED_RED = 49;  
const int LED_GREEN = 51; 
const int LED_BLUE = 53; 
//const int LED_RED = 8;  
//const int LED_GREEN = 7; 
//const int LED_BLUE = 6;  

unsigned int beginingTime = 0;
unsigned int currentTime = 0;
short int light = 0.0;
int conditionRunway = 0;

void assignColor(unsigned char red, unsigned char green, unsigned char blue) {
    analogWrite(LED_RED, red);
    analogWrite(LED_BLUE, blue);
    analogWrite(LED_GREEN, green);
}