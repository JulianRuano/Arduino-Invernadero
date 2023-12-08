#include "PasswordManager.h"          
#include <LiquidCrystal.h>     
#include <Keypad.h>            
#include <AverageValue.h>       
#include "StateMachineLib.h"    
#include "AsyncTaskLib.h"   
#include "DHT.h" 
#define BUZZER 13  
#define PIN A5  
#define SENSOR_TYPE DHT11          
#define CELLPHOTO A0   


LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int redPin = 8;  
const int greenPin = 7; 
const int bluePin = 6;
const float gammaValue = 0.7;  
const float resistanceRL10Value = 50;  


unsigned int beginingTime = 0;
unsigned int currentTime = 0;
short int light = 0.0;

char password[] = {'1', '2', '3', 'D'}; 
int passwordNumbersArray[4];                 
int counterPasswordNumbers = 0;                    
int counterPasswordMistakes = 0;            
int counterPasswordCorrects = 0;            

const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {24, 26, 28, 30}; 
byte colPins[COLS] = {32, 34, 36, 38};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);


void color(unsigned char red, unsigned char green, unsigned char blue) {
    analogWrite(redPin, red);
    analogWrite(bluePin, blue);
    analogWrite(greenPin, green);
}


StateMachine stateMachine(5, 8);
int trackCondition = 0;      /**< Variable para el trackeo de una condición. */



Input input;  

enum State {
    initialState = 0,   
    blockedState = 1,    
    monitorTHState = 2,   
    alarmState = 3,   
    monitorLightState = 4, 
};


AsyncTask timeOutTask5(5000, false, []() { fnTimeOut5(); });  
AsyncTask timeOutTask3(3000, false, []() { fnTimeOut6(); });  
AsyncTask timeOutTask6(6000, false, []() { fnTimeOut7(); });  
AsyncTask initTask(50, true, []() { fnInit(); });            
AsyncTask monitorTHTask(500, true, []() { fnMonitorTH(); });  
AsyncTask monitorLightTask(200, true, []() { fnMonitorLuz(); });
AsyncTask alarmTask(50, true, []() { fnAlarma(); });         


void fnTimeOut5()
{
    input = TimeOut5;    
    trackCondition = 0;   
}


void fnTimeOut6()
{
    input = TimeOut6;    
    trackCondition = 0;  
}


void fnTimeOut7()
{
    input = TimeOut7;    /**< Asigna el timeout de 7 segundos como el último input de usuario. */
    trackCondition = 0;   /**< Resetea la condición. */
}


void outputInit()
{
    initTask.Stop();
}


void inputMonitorTH()
{
    monitorTHTask.Start();
}


void outputMonitorTH()
{
    monitorTHTask.Stop();
}


void inputMonitorLight()
{
    monitorTHTask.Stop();
    monitorLightTask.Start();
}


void outputMonitorLight()
{
    monitorLightTask.Stop();
}


void inputAlarm()
{
    alarmTask.Start();
}


void outputAlarm()
{
    alarmTask.Stop();
}


void fnInit()
{
    lcd.clear();
    counterPasswordNumbers = 0;
    counterPasswordCorrects = 0;
    counterPasswordMistakes = 0;
    lcd.print("PIN: ");
    do {
        color(0, 0, 0);
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
}


void inputBlocked()
{
    color(255, 0, 0);
    lcd.print("BLOQUEO INMINENTE");
    timeOutTask5.SetIntervalMillis(5000);
    timeOutTask5.Start();
}


void fnAlarma() {
    while (trackCondition == 0)
    {
        lcd.clear();
        color(255, 0, 0);
        lcd.print("ALERTA ATENCION");
        tone(BUZZER, 1000, 5000);
        timeOutTask6.Start();
        trackCondition++;
    }
}


void setupStateMachine()
{
    /* Transiciones */
    stateMachine.AddTransition(initialState, blockedState, []() { return input == InBlocked; });
    stateMachine.AddTransition(initialState, monitorTHState, []() { return input == InMonitorTH; });
    stateMachine.AddTransition(blockedState, initialState, []() { return input == TimeOut5; });
    stateMachine.AddTransition(monitorTHState, alarmState, []() { return input == InAlarm; });
    stateMachine.AddTransition(monitorTHState, monitorLightState, []() { return input == TimeOut5; });
    stateMachine.AddTransition(alarmState, monitorTHState, []() { return input == TimeOut7; });
    stateMachine.AddTransition(monitorLightState, alarmState, []() { return input == InAlarm; });
    stateMachine.AddTransition(monitorLightState, monitorTHState, []() { return input == TimeOut6; });

    /* Acciones */
    stateMachine.SetOnEntering(initialState, inputInit);
    stateMachine.SetOnEntering(blockedState, inputBlocked);
    stateMachine.SetOnEntering(monitorTHState, inputMonitorTH);
    stateMachine.SetOnEntering(alarmState, inputAlarm);
    stateMachine.SetOnEntering(monitorLightState, inputMonitorLight);
    stateMachine.SetOnLeaving(initialState, []() { outputInit(); });
    stateMachine.SetOnLeaving(monitorTHState, []() { outputMonitorTH(); });
    stateMachine.SetOnLeaving(alarmState, []() { outputAlarm(); });
    stateMachine.SetOnLeaving(monitorLightState, []() { outputMonitorLight(); });
}


DHT dht(PIN, SENSOR_TYPE); 
const long MAX_VALUES_NUM = 5;
AverageValue<long> averageTemperatureValue(MAX_VALUES_NUM); 
AverageValue<long> averageHumidityValue(MAX_VALUES_NUM); 
AverageValue<long> cleanAverageValue(MAX_VALUES_NUM); 


void fnMonitorTH()
{
    noTone(BUZZER);
    color(0, 0, 0);
    beginingTime = millis();
    lcd.clear();
    counterPasswordCorrects = 0;
    averageTemperatureValue = cleanAverageValue;
    averageHumidityValue = cleanAverageValue;
    counterPasswordNumbers = 0;
    dht.begin();
    while (trackCondition == 0)
    {
        float h = dht.readHumidity();
        float t = dht.readTemperature();
        float f = dht.readTemperature(true);

        if (isnan(h) || isnan(t) || isnan(f)) {
            Serial.println(F("Failed to read from DHT sensor!"));
            return;
        }

        float hif = dht.computeHeatIndex(f, h);
        float hic = dht.computeHeatIndex(t, h, false);

        lcd.setCursor(0, 0);
        lcd.print("Humedad: ");
        lcd.print(h);
        lcd.print("%");

        lcd.setCursor(0, 1);
        lcd.print("Tempe: ");
        lcd.print(t);
        lcd.print(" C");
        delay(900);
        averageTemperatureValue.push(t);
        averageHumidityValue.push(h);
        counterPasswordNumbers++;
        if (counterPasswordNumbers >= 5)
        {
            if (averageTemperatureValue.average() > 30 && averageHumidityValue.average() > 70)
            {
                input = InAlarm;
                counterPasswordCorrects++;
                break;
            }
            else
            {
                currentTime = millis();
                counterPasswordCorrects++;
                timeOutTask5.SetIntervalMillis(5000 - (currentTime - beginingTime));
                timeOutTask5.Start();
                trackCondition++;
            }
            counterPasswordNumbers = 0;
        }
    }
}


AverageValue<long> averageLightValue(MAX_VALUES_NUM);


void fnMonitorLuz()
{   
    beginingTime = millis();
    lcd.clear();
    counterPasswordNumbers = 0;
    averageLightValue = cleanAverageValue;
    while (trackCondition == 0)
    {
        lcd.setCursor(0, 0);
        int analogValue = analogRead(CELLPHOTO);
        float voltage = analogValue / 1024.0 * 5.0;
        float resistance = 2000.0 * voltage / (1.0 - voltage / 5.0);
        float light = pow(resistanceRL10Value * 1e3 * pow(10, gammaValue) / resistance, (1.0 / gammaValue));
        lcd.print("Luz: ");
        lcd.print(analogValue);
        averageLightValue.push(light);
        delay(500); 
        counterPasswordNumbers++;
        if (counterPasswordNumbers >= 5)
        {
            if (analogValue < 20.0) {
                input = InAlarm;
                break;
            } else {
                currentTime = millis();
                timeOutTask3.SetIntervalMillis(3000 - (currentTime - beginingTime));
                timeOutTask3.Start();
                trackCondition++;
            }
        }
    }
}


void setup() 
{

    lcd.begin(16, 2);

    pinMode(redPin, OUTPUT); 
    pinMode(greenPin, OUTPUT);
    pinMode(bluePin, OUTPUT);
    pinMode(BUZZER, OUTPUT); 


    Serial.begin(9600);
    setupStateMachine();	


    stateMachine.SetState(initialState, false, true);
}


void loop() 
{
    monitorLightTask.Update();
    timeOutTask5.Update();
    timeOutTask3.Update();
    timeOutTask6.Update();
    alarmTask.Update();
    initTask.Update();
    monitorTHTask.Update();
    stateMachine.Update();
}