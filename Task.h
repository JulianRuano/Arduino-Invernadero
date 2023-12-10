/**
  * @file Task.h
  * @version 1.0
  * @date 09/12/2023
  * @author Ledy Astudillo <lmastudillo@unicauca.edu.co>- Julian majin <julianruano@unicauca.edu.co>
  * @title Taskpage greenhouse Project 
  * @brief Task project file.
*/
// Include libraries
#include "AsyncTaskLib.h"  
#include "StateMachineLib.h"

/** 
  @brief Prevent repeated inclusions 
 */
#ifndef TASK_H
#define TASK_H

/** 
  @brief External variables
 */
// External state machine
extern StateMachine stateMachine; 

// Async background tasks
extern AsyncTask initTask;
extern AsyncTask monitorTHTask;
extern AsyncTask monitorLightTask;
extern AsyncTask alarmTask;
extern AsyncTask task5;
extern AsyncTask task3;
extern AsyncTask task6;

/** 
  @brief State functions
 */


void funtionInit();
void funtionMonitorTH();
void funtionMonitorLuz();
void funtionAlarm();

/** 
  @brief Timeout handlers
 */
void funtionTimeOut5();
void funtionTimeOut6();
void funtionTimeOut7();


/** 
  @brief Input handlers- Output handlers
 */

void inputMonitorTH();
void outputMonitorTH();
void inputMonitorLight();
void outputMonitorLight();
void inputAlarm();
void outputAlarm();
void outputInit();
void inputBlocked();
// Setup state machine
void setupStateMachine();

// Close conditional compilation
#endif // TASK_H
