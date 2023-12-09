#include "Task.h"   
#include "Utilities.h"         
        
 
void setup() 
{

    lcd.begin(16, 2);

    pinMode(LED_RED, OUTPUT); 
    pinMode(LED_GREEN, OUTPUT);
    pinMode(LED_BLUE, OUTPUT);
    pinMode(BUZZER, OUTPUT); 


    Serial.begin(9600);
    setupStateMachine();	


    stateMachine.SetState(intState, false, true);
}


void loop() 
{
    monitorLightTask.Update();
    task5.Update();
    task3.Update();
    task6.Update();
    alarmTask.Update();
    initTask.Update();
    monitorTHTask.Update();
    stateMachine.Update();
}