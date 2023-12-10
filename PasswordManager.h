
/**
  * @file PasswordManager.h
  * @version 1.0
  * @date 09/12/2023
  * @author Ledy Astudillo <lmastudillo@unicauca.edu.co>- Julian majin <julianruano@unicauca.edu.co>
  * @title PasswordManagerpage greenhouse Project 
  * @brief PasswordManager project file.
*/
  
// Include libraries
#include <Keypad.h>
#include "AsyncTaskLib.h"

/** 
  @brief Prevent repeated inclusions
 */
#ifndef PASSWORD_MANAGER_H
#define PASSWORD_MANAGER_H


/** 
  @brief // External declarations
 */
extern char password[];
extern int passwordNumbersArray[4];
extern int digitCount;
extern int numberAttempts;
extern int passwordCorrects;

extern Keypad keypad;

/** 
  @brief Function declarations
 */
void accessGranted();
void accessDenied();
void inputInit();

// Close conditional compilation
#endif // PASSWORD_MANAGER_H
