#include "PasswordManager.h"
#include "Utilities.h"

extern Input input;
extern AsyncTask initTask;
extern int numberAttempts;


char password[] = {'1', '2', '3', 'D'}; 
int passwordNumbersArray[4];                 
int digitCount = 0;                    
int numberAttempts = 0;            
int passwordCorrects = 0;   


const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};

//byte rowPins[ROWS] = {24, 26, 28, 30}; 
//byte colPins[COLS] = {32, 34, 36, 38};

byte rowPins[ROWS] = {22, 23, 24, 25}; 
byte colPins[COLS] = {26, 27, 28, 29};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


void accessGranted() {
    clearAndPrintPrompt("Bienvenido");
    delay(800);
    lcd.clear();
    input = InMonitorTH;
}


void accessDenied() {
    lcd.clear();
    if (numberAttempts < 2) {
        lcd.print("jumm sospechoso");
        delay(800);
        numberAttempts++;
        clearAndPrintPrompt("Contra: ");
    } else {
        passwordCorrects = -1;
        input = InBlocked;
    }
}



void getInput(char passwordNumbersArray[], int &digitCount) {
    do {
        char key = keypad.getKey();
        if (key) {
            lcd.print('*');
            passwordNumbersArray[digitCount++] = key;
            delay(100);
        }
    } while (digitCount < 4);
}

void checkPassword(const char password[], const char passwordNumbersArray[], int &passwordCorrects) {
    passwordCorrects = 0;

    for (int i = 0; i < 4; i++) {
        if (password[i] == passwordNumbersArray[i]) {
            passwordCorrects++;
        }
    }
}

void inputInit() {
    lcd.clear();
    digitCount = 0;
    passwordCorrects = 0;
    numberAttempts = 0;
    clearAndPrintPrompt("Contra: ");

    do {
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

    initTask.Stop(); 
}