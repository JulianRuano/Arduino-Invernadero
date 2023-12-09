#include "Task.h"

AsyncTask monitorTHTask(500, true, []() { funtionMonitorTH(); });
AsyncTask monitorLightTask(200, true, []() { funtionMonitorLuz(); });
AsyncTask alarmTask(50, true, []() { funtionAlarm(); });
AsyncTask task5(5000, false, []() { funtionTimeOut5(); });
AsyncTask task3(3000, false, []() { funtionTimeOut6(); });
AsyncTask task6(6000, false, []() { funtionTimeOut7(); });