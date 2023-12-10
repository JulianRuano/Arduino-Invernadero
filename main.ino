/**
  * @file main.ino
  * @version 1.0
  * @date 09/12/2023
  * @author Ledy Astudillo <lmastudillo@unicauca.edu.co>- Julian majin <julianruano@unicauca.edu.co>
  * @title mainpage greenhouse Project 
  * @brief Main project file.
*/
// Include libraries and header files 
#include "Task.h"   
#include "Utilities.h"         
        

  


 
/** 
  @brief Initial setup  
 */
void setup() 
{
  // Initialize LCD display 
    lcd.begin(16, 2);
    // Configure LED and buzzer pins as output
    pinMode(LED_RED, OUTPUT); 
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    pinMode(BUZZER, OUTPUT); 

  
   // Initialize serial port
    Serial.begin(9600);
    // Configure state machine
    setupStateMachine();	
    // Set initial state
    stateMachine.SetState(intState, false, true);
}


/** 
  @brief Main loop  
 */
void loop() 
{
    // Update background tasks 
 
    monitorLightTask.Update();
    task5.Update();
    task3.Update();
    task6.Update();
    alarmTask.Update();
    initTask.Update();
    monitorTHTask.Update();
     
    // Update state machine
    stateMachine.Update();
}