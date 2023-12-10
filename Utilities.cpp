/**
  * @file Utilities.cpp
  * @version 1.0
  * @date 09/12/2023
  * @author Ledy Astudillo <lmastudillo@unicauca.edu.co>- Julian majin <julianruano@unicauca.edu.co>
  * @title Utilitiespage greenhouse Project 
  * @brief Utilities project file.
  */

#include "Utilities.h"
#include "Arduino.h"
 

Input input;


LiquidCrystal lcd(12, 11, 10, 9, 8, 7);


int LED_RED = 49;  
int LED_GREEN = 51; 
int LED_BLUE = 53; 


unsigned int beginingTime = 0;
unsigned int currentTime = 0;
short int light = 0.0;
int conditionRunway = 0;



DHT dht(A1, DHT11); 


AverageValue<long> averageTemperatureValue(MAX_VALUES_NUM);
AverageValue<long> averageHumidityValue(MAX_VALUES_NUM);
AverageValue<long> cleanAverageValue(MAX_VALUES_NUM);
AverageValue<long> averageLightValue(MAX_VALUES_NUM);

/**
  * @brief Assigns RGB color to the LED.
  * 
  * This function takes three parameters - red, green, and blue - and assigns 
  * corresponding analog PWM values to the respective LED pins.
  *
  * @param red   An unsigned char representing the intensity of the red color (0-255).
  * @param green An unsigned char representing the intensity of the green color (0-255).
  * @param blue  An unsigned char representing the intensity of the blue color (0-255).
  * 
  * @return void
  */
void assignColor(unsigned char red, unsigned char green, unsigned char blue) {
    analogWrite(LED_RED, red);
    analogWrite(LED_BLUE, blue);
    analogWrite(LED_GREEN, green);
}

/**
  * @brief Clears the LCD screen and prints a given prompt label.
  * 
  * This function takes a prompt label as a parameter, clears the LCD screen,
  * and prints the provided label on the screen.
  *
  * @param promptLabel The label to be displayed on the LCD screen.
  * 
  * @return void
  */
void clearAndPrintPrompt(const char *promptLabel) {
    lcd.clear();
    lcd.print(promptLabel);
}
