#ifndef PASSWORD_MANAGER_H
#define PASSWORD_MANAGER_H

#include <Keypad.h>
#include <LiquidCrystal.h>
#include "AsyncTaskLib.h"

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

extern char password[];
extern int passwordNumbersArray[4];
extern int counterPasswordNumbers;
extern int counterPasswordMistakes;
extern int counterPasswordCorrects;

extern Keypad keypad;
extern LiquidCrystal lcd;

void accessGranted();
void accessDenied();
void inputInit();

#endif // PASSWORD_MANAGER_H
