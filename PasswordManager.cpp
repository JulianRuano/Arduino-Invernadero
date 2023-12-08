#include "PasswordManager.h"

extern Input input;
extern AsyncTask initTask;
extern int counterPasswordMistakes;
extern LiquidCrystal lcd;

void accessGranted() {
    lcd.clear();
    // Asegúrate de que la función "color" esté disponible o declárala aquí
    lcd.print("--Identidad Reconocida--");
    delay(800);
    lcd.clear();
    // Asegúrate de que la función "color" esté disponible o declárala aquí
    input = InMonitorTH;
}

void accessDenied() {
    lcd.clear();
    if (counterPasswordMistakes < 2) {
        // Asegúrate de que la función "color" esté disponible o declárala aquí
        lcd.print("Esa no es pelao");
        delay(800);
        counterPasswordMistakes++;
        lcd.clear();
        lcd.print("PIN: ");
    } else {
        counterPasswordCorrects = -1;
        input = InBlocked;
    }
}

void inputInit() {
    lcd.clear();
    counterPasswordNumbers = 0;
    counterPasswordCorrects = 0;
    counterPasswordMistakes = 0;
    lcd.print("PIN: ");
    do {
        // Asegúrate de que la función "color" esté disponible o declárala aquí
        char key = keypad.getKey();
        if (key) {
            lcd.print('*');
            passwordNumbersArray[counterPasswordNumbers] = key;
            counterPasswordNumbers++;
            delay(100);
        }
        if (counterPasswordNumbers == 4) {
            counterPasswordCorrects = 0;
            for (int i = 0; i < counterPasswordNumbers; i++) {
                if (password[i] == passwordNumbersArray[i]) {
                    counterPasswordCorrects++;
                }
            }
            if (counterPasswordCorrects == counterPasswordNumbers) {
                accessGranted();
                counterPasswordNumbers = 0;
            } else {
                accessDenied();
                counterPasswordNumbers = 0;
            }
        }
    } while ((counterPasswordCorrects != -1) && (counterPasswordCorrects != 4));

    initTask.Stop();  // Detener la tarea de inicialización después de completar la entrada
}
