#ifndef TASK_H
#define TASK_H

#include "AsyncTaskLib.h"  // Asegúrate de incluir la librería necesaria aquí

extern AsyncTask monitorTHTask;
extern AsyncTask monitorLightTask;
extern AsyncTask alarmTask;
extern AsyncTask task5;
extern AsyncTask task3;
extern AsyncTask task6;

void funtionMonitorTH();
void funtionMonitorLuz();
void funtionAlarm();
void funtionTimeOut5();
void funtionTimeOut6();
void funtionTimeOut7();

#endif // TASK_H
