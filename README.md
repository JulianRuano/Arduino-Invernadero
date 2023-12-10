<h1 align="center">Proyecto de Invernadero Inteligente con Arduino</h1>

###

<p align="left">El sistema se basa en una máquina de estados finitos implementada con la biblioteca "StateMachineLib.h" en Arduino.</p>

###

<div align="left">
  <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/cplusplus/cplusplus-original.svg" height="40" alt="cplusplus logo"  />
  <img width="12" />
  <img src="https://cdn.jsdelivr.net/gh/devicons/devicon/icons/arduino/arduino-original.svg" height="40" alt="arduino logo"  />
</div>

###

<h2 align="left"></h2>

###

<div align="center">
  <img height="300" src="https://res.cloudinary.com/dilrruxyx/image/upload/v1702167451/simulador.png"  />
</div>

###

### Estado Inicial (init):

Se inicia en este estado.
Si se ingresa la clave incorrecta tres veces, pasa al estado "bloqueado".
Si se ingresa la clave correcta, pasa al estado "monitor de temperatura y humedad".

### Estado Bloqueado (blocked):

Si se ingresó incorrectamente la clave tres veces, el sistema se bloquea.
Después de 5 segundos en este estado, vuelve al estado inicial (init).

### Estado Monitor de Temperatura y Humedad (monitorTH):

Se accede desde el estado inicial al ingresar la clave correcta.
Si la temperatura es mayor a 30 grados y la humedad supera el 70%, pasa al estado de "alarma".
Cada 5 segundos, cambia al estado "monitor de luz".

### Estado Alarma (alarm):

Se activa si la temperatura es alta y la humedad es excesiva.
Después de 6 segundos en este estado, vuelve al estado de "monitor de temperatura y humedad".

### Estado Monitor de Luz (monitorLight):

Se accede desde el estado de "monitor de temperatura y humedad" cada 5 segundos.
Si la luz es menor a 20, se activa la "alarma".
Después de 6 segundos en este estado, vuelve al estado de "monitor de temperatura y humedad".

###

## Funciones Específicas de la Máquina de Estados:
## Transiciones:

Se han establecido transiciones entre los estados según las entradas proporcionadas (InBlocked, InMonitorTH, TimeOut5, InAlarm).
Acciones al Entrar en un Estado:

Se han definido funciones específicas para ejecutar al entrar en cada estado (inputInit, inputBlocked, inputMonitorTH, inputAlarm, inputMonitorLight).
Acciones al Salir de un Estado:

Se han definido funciones específicas para ejecutar al salir de cada estado (outputInit, outputMonitorTH, outputAlarm, outputMonitorLight).
Este diseño permite un control detallado del invernadero, gestionando eventos como bloqueo por clave incorrecta, 
monitoreo de temperatura, humedad y luz, y activación de alarmas en condiciones adversas.

###

<div align="center">
  <img height="300" src="https://res.cloudinary.com/dilrruxyx/image/upload/v1702164701/estados.png"  />
</div>

## Simulación en Wokwi

Simulación del proyecto en Wokwi:
[link](https://wokwi.com/arduino/projects/383594322323065857)

