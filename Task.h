#ifndef TASK_H
#define TASK_H

#include "AsyncTaskLib.h"  
#include "StateMachineLib.h"

extern StateMachine stateMachine; 

extern AsyncTask initTask;
extern AsyncTask monitorTHTask;
extern AsyncTask monitorLightTask;
extern AsyncTask alarmTask;
extern AsyncTask task5;
extern AsyncTask task3;
extern AsyncTask task6;

void funtionInit();
void funtionMonitorTH();
void funtionMonitorLuz();
void funtionAlarm();
void funtionTimeOut5();
void funtionTimeOut6();
void funtionTimeOut7();

void inputMonitorTH();
void outputMonitorTH();
void inputMonitorLight();
void outputMonitorLight();
void inputAlarm();
void outputAlarm();
void outputInit();
void inputBlocked();

void setupStateMachine();

#endif // TASK_H
