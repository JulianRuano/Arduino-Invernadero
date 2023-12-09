#ifndef PASSWORD_MANAGER_H
#define PASSWORD_MANAGER_H

#include <Keypad.h>
#include "AsyncTaskLib.h"


extern char password[];
extern int passwordNumbersArray[4];
extern int digitCount;
extern int numberAttempts;
extern int passwordCorrects;

extern Keypad keypad;


void accessGranted();
void accessDenied();
void inputInit();


#endif // PASSWORD_MANAGER_H
