#include "Task.h"
#include "Utilities.h"
#include "PasswordManager.h" 

extern Input input;

StateMachine stateMachine(5, 8);

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
    input = TimeOut7;   
    conditionRunway = 0;   
}

void funtionInit()
{
    lcd.clear();
    digitCount = 0;
    passwordCorrects = 0;

    lcd.print("CONTRA: ");
    do {
        assignColor(0, 0, 0);
        char key = keypad.getKey();
        if (key) {
            lcd.print('*');
            passwordNumbersArray[digitCount] = key;
            digitCount++;
            delay(100);
        }
        if (digitCount == 4) {
            passwordCorrects = 0;
            for (int i = 0; i < digitCount; i++) {
                if (password[i] == passwordNumbersArray[i]) {
                    passwordCorrects++;
                }
            }
            if (passwordCorrects == digitCount) {
                accessGranted();
                digitCount = 0;
            } else {
                accessDenied();
                digitCount = 0;
            }
        }
    } while ((passwordCorrects != -1) && (passwordCorrects != 4));
}



void funtionAlarm() {
  while (conditionRunway == 0) {
          clearAndPrintPrompt("ALERTA");
          assignColor(255, 0, 0);
        
          tone(BUZZER, 659, 500); 
          
          task6.Start();
          conditionRunway++;
          Serial.println("ALERTA...");
      }
}

void funtionMonitorTH()
{
    noTone(BUZZER);
    assignColor(0, 0, 0);
    beginingTime = millis();
    lcd.clear();
    passwordCorrects = 0;
    averageTemperatureValue = cleanAverageValue;
    averageHumidityValue = cleanAverageValue;
    digitCount = 0;
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
        digitCount++;
        if (digitCount >= 5)
        {
            if (averageTemperatureValue.average() > 30 && averageHumidityValue.average() > 70)
            {
                input = InAlarm;
                passwordCorrects++;
                break;
            }
            else
            {
                currentTime = millis();
                passwordCorrects++;
                task5.SetIntervalMillis(5000 - (currentTime - beginingTime));
                task5.Start();
                conditionRunway++;
            }
            digitCount = 0;
        }
    }
}


void funtionMonitorLuz()
{   
    beginingTime = millis();
    lcd.clear();
    digitCount = 0;
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
        digitCount++;
        if (digitCount >= 5)
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

AsyncTask initTask(50, true, []() { funtionInit(); });
AsyncTask monitorTHTask(500, true, []() { funtionMonitorTH(); });
AsyncTask monitorLightTask(200, true, []() { funtionMonitorLuz(); });
AsyncTask alarmTask(50, true, []() { funtionAlarm(); });
AsyncTask task5(5000, false, []() { funtionTimeOut5(); });
AsyncTask task3(3000, false, []() { funtionTimeOut6(); });
AsyncTask task6(6000, false, []() { funtionTimeOut7(); });

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


void outputInit()
{
    initTask.Stop();
}

void inputBlocked()
{
    assignColor(255, 0, 0);
    lcd.print("BLOQUEO");
    task5.SetIntervalMillis(5000);
    task5.Start();
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