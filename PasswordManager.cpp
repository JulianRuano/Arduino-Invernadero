/**
  * @file PasswordManager.cpp
  * @version 1.0
  * @date 09/12/2023
  * @author Ledy Astudillo <lmastudillo@unicauca.edu.co>- Julian majin <julianruano@unicauca.edu.co>
  * @title PasswordManagerPage greenhouse Project 
  *  @brief PasswordManager project file.
*/
// Include libraries and declarations
#include "PasswordManager.h"
#include "Utilities.h"

/** 
  @brief Global variables
 */
extern Input input;
extern AsyncTask initTask;
extern int numberAttempts;

/** 
  @brief Correct password  
 */
char password[] = {'1', '2', '3', 'D'}; 

/** 
  @brief Input buffer
 */
int passwordNumbersArray[4]; 

/** 
  @brief Counters
 */               
int digitCount = 0;                    
int numberAttempts = 0;            
int passwordCorrects = 0;   

/** 
  @brief Keypad setup 
 */  
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};


byte rowPins[ROWS] = {22, 23, 24, 25}; 
byte colPins[COLS] = {26, 27, 28, 29};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


/** 
  @brief Function when correct password is entered
 */

void accessGranted() {
  // Displays welcome message 
    clearAndPrintPrompt("Bienvenido");
    delay(800);
    
  // Clears screen
    lcd.clear();
    
  // Changes input state
    input = InMonitorTH;
}

/** 
  @brief Function when incorrect password is entered
 */
void accessDenied() {
    lcd.clear();
    if (numberAttempts < 2) {
       // Displays error message
        lcd.print("No es correcta");
        delay(800);
        // Increments attempts counter 
        numberAttempts++;
        clearAndPrintPrompt("Clave: ");
    } else {
       // Changes state if max attempts reached
        passwordCorrects = -1;
        
        input = InBlocked;
    }
}



 
 /** 
  @brief  Function to get inputted digits
 */
void getInput(char passwordNumbersArray[], int &digitCount) {
    do {
       // Reads pressed keys
        char key = keypad.getKey();
        
        if (key) {
            lcd.print('*');
             // Saves digits in array
            passwordNumbersArray[digitCount++] = key;
            delay(100);
        }
      // Exits when 4 digits are complete
    } while (digitCount < 4);
}

/** 
  @brief Function to check password
 */

void checkPassword(const char password[], const char passwordNumbersArray[], int &passwordCorrects) {
    passwordCorrects = 0;
    // Compares entered password and real password arrays 
    // Counts number of correct digits
    for (int i = 0; i < 4; i++) {
        if (password[i] == passwordNumbersArray[i]) {
          
            passwordCorrects++;
        }
    }
}
/** 
  @brief // Initialization function
 */
void inputInit() {
    lcd.clear();
    // Initializes variables
    digitCount = 0;
    passwordCorrects = 0;
    numberAttempts = 0;
    // Displays message on screen
    clearAndPrintPrompt("Clave: ");

    do {
        
      // Reads and verifies password in loop
        char passwordNumbersArray[4];
        getInput(passwordNumbersArray, digitCount);
        checkPassword(password, passwordNumbersArray, passwordCorrects);

        if (passwordCorrects == 4) {
            accessGranted();
        } else {
            accessDenied();
        }

        digitCount = 0;
    } while (passwordCorrects != -1 && passwordCorrects != 4);
  
    // Stops other running tasks
    initTask.Stop(); 
}