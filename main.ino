#include "PasswordManager.h"          
#include <LiquidCrystal.h>     
#include <Keypad.h>            
#include <AverageValue.h>       
#include "StateMachineLib.h"    
#include "AsyncTaskLib.h"   
#include "DHT.h" 
#define BUZZER 13            
  

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int redPin = 8;  
const int greenPin = 7; 
const int bluePin = 6;  


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
int conditionRunway = 0;


Input input;  

enum State {
    intState = 0,   
    blocked = 1,    
    monitorTH = 2,   
    alarm = 3,   
    monitorLight = 4, 
};


AsyncTask initTask(50, true, []() { funtionInit(); });            
AsyncTask monitorTHTask(500, true, []() { funtionMonitorTH(); });  
AsyncTask monitorLightTask(200, true, []() { funtionMonitorLuz(); });
AsyncTask alarmTask(50, true, []() { funtionAlarm(); });         
AsyncTask task5(5000, false, []() { funtionTimeOut5(); });  
AsyncTask task3(3000, false, []() { funtionTimeOut6(); });  
AsyncTask task6(6000, false, []() { funtionTimeOut7(); });  

void funtionTimeOut5()
{
    input = TimeOut5;    
    conditionRunway = 0;   
}


void funtionTimeOut6()
{
    input = TimeOut6;    
    conditionRunway = 0;  
}


void funtionTimeOut7()
{
    input = TimeOut7;    /**< Asigna el timeout de 7 segundos como el último input de usuario. */
    conditionRunway = 0;   /**< Resetea la condición. */
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


void funtionInit()
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
    task5.SetIntervalMillis(5000);
    task5.Start();
}


void funtionAlarm() {
    while (conditionRunway == 0)
    {
        lcd.clear();
        color(255, 0, 0);
        lcd.print("ALERTA ATENCION");
        tone(BUZZER, 1000, 5000);
        task6.Start();
        conditionRunway++;
    }
}


void setupStateMachine()
{
 
    stateMachine.AddTransition(intState, blocked, []() { return input == InBlocked; });
    stateMachine.AddTransition(intState, monitorTH, []() { return input == InMonitorTH; });
    stateMachine.AddTransition(blocked, intState, []() { return input == TimeOut5; });
    stateMachine.AddTransition(monitorTH, alarm, []() { return input == InAlarm; });
    stateMachine.AddTransition(monitorTH, monitorLight, []() { return input == TimeOut5; });
    stateMachine.AddTransition(alarm, monitorTH, []() { return input == TimeOut7; });
    stateMachine.AddTransition(monitorLight, alarm, []() { return input == InAlarm; });
    stateMachine.AddTransition(monitorLight, monitorTH, []() { return input == TimeOut6; });


    stateMachine.SetOnEntering(intState, inputInit);
    stateMachine.SetOnEntering(blocked, inputBlocked);
    stateMachine.SetOnEntering(monitorTH, inputMonitorTH);
    stateMachine.SetOnEntering(alarm, inputAlarm);
    stateMachine.SetOnEntering(monitorLight, inputMonitorLight);
    stateMachine.SetOnLeaving(intState, []() { outputInit(); });
    stateMachine.SetOnLeaving(monitorTH, []() { outputMonitorTH(); });
    stateMachine.SetOnLeaving(alarm, []() { outputAlarm(); });
    stateMachine.SetOnLeaving(monitorLight, []() { outputMonitorLight(); });
}


DHT dht(A5, DHT11); 
const long MAX_VALUES_NUM = 5;
AverageValue<long> averageTemperatureValue(MAX_VALUES_NUM); 
AverageValue<long> averageHumidityValue(MAX_VALUES_NUM); 
AverageValue<long> cleanAverageValue(MAX_VALUES_NUM); 


void funtionMonitorTH()
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
    while (conditionRunway == 0)
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
                task5.SetIntervalMillis(5000 - (currentTime - beginingTime));
                task5.Start();
                conditionRunway++;
            }
            counterPasswordNumbers = 0;
        }
    }
}


AverageValue<long> averageLightValue(MAX_VALUES_NUM);


void funtionMonitorLuz()
{   
    beginingTime = millis();
    lcd.clear();
    counterPasswordNumbers = 0;
    averageLightValue = cleanAverageValue;
    while (conditionRunway == 0)
    {
        lcd.setCursor(0, 0);
        int analogValue = analogRead(A0);
        float voltage = analogValue / 1024.0 * 5.0;
        float resistance = 2000.0 * voltage / (1.0 - voltage / 5.0);
        float light = pow(50 * 1e3 * pow(10, 0.7) / resistance, (1.0 / 0.7));
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
                task3.SetIntervalMillis(3000 - (currentTime - beginingTime));
                task3.Start();
                conditionRunway++;
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


    stateMachine.SetState(intState, false, true);
}


void loop() 
{
    monitorLightTask.Update();
    task5.Update();
    task3.Update();
    task6.Update();
    alarmTask.Update();
    initTask.Update();
    monitorTHTask.Update();
    stateMachine.Update();
}