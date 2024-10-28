#include <LedControl.h>


// Módulos propios
#include "RelojArena.h"
#include "PatronesLed.h"

// Definición de pines y constantes de las matrices y el joystick
const int matrizA = 0; // Parte superior del reloj
const int matrizB = 1; // Parte inferior del reloj
const int DIN = 11;
const int CLK = 13;
const int CS = 10;
const int numMatrices = 2;

const int vertPin = A0;   // Eje y del joystick
const int horzPin = A1;   // Eje x del joystick
const int switchPin = 9; // Switch del joystick (pulsador)

// Declarar el objeto LedControl
LedControl lc = LedControl(DIN, CLK, CS, numMatrices); // Dos matrices conectadas

// Array para registrar qué leds de la matriz B
// están encendidos (1) o apagados (0) en cada momento
int ledsMatrizB[8][8];

// Contador de granos de arena en reposo
int totalGranosCaidos;

// Variables de tiempo
unsigned long tiempoUltimaCaida;
const unsigned long intervalo = 1000;  // Intervalo entre caida de partículas (1 segundo)

// Variables para detener o iniciar el circuito
bool encendido = false;
int ultimoEstadoSwitch = HIGH; // Estado anterior del pulsador en joystick

// Variable para guardar la última acción del joystick
enum Direccion {ARRIBA, ABAJO, IZQUIERDA, DERECHA, CENTRO};
enum Accion { REINICIAR, PAUSAR, REANUDAR, IMAGEN, NONE };
Accion comandoAccion = NONE; // Inicialmente no realiza ninguna acción

// Declaración de función para detectar movimiento del joystick
Direccion movimientoJoystick(int x, int y); // función retorna un dato tipo Dirección



void setup() {
  // Pulsador del joystick : es una entrada
  pinMode(switchPin, INPUT_PULLUP);
  // (Las entradas analógicas del joystick, A0 y A1, no requieren inicialización)

  // Inicializar cada matriz
  lc.shutdown(matrizA, false);  // Despertar la matriz 1
  lc.shutdown(matrizB, false);  // Despertar la matriz 2
  
  lc.setIntensity(matrizA, 8);  // Ajustar brillo para la matriz 1 (0 a 15)
  lc.setIntensity(matrizB, 8);  // Ajustar brillo para la matriz 2 (0 a 15)

  randomSeed(analogRead(0));  // Para generar números aleatorios
  
}




void loop() {

  // DETECTAR SI SE HA PRESIONADO EL SWITCH DEL JOYSTICK
  int estadoSwitch = digitalRead(switchPin);

  // Detectar si el pulsador fue presionado y luego soltado
  if (estadoSwitch == LOW && ultimoEstadoSwitch == HIGH) {
      // Alternar el estado de ejecución
      encendido = !encendido;

      // EJECUTAR PROCESOS DE ENCENDIDO Y APAGADO
      if(encendido){ // Se ha activado el interruptor
        lc.shutdown(matrizA, false);  // Apagar la matriz 1
        lc.shutdown(matrizB, false);  // Apagar la matriz 2
        resetRelojArena();
        comandoAccion = REINICIAR;
      } else{ // Se ha desactivado el interruptor
        lc.clearDisplay(matrizA);
        lc.clearDisplay(matrizB);
        lc.shutdown(matrizA, true);  // Apagar la matriz 1
        lc.shutdown(matrizB, true);  // Apagar la matriz 2
        comandoAccion = NONE;
      }
      delay(200); // Pequeño retardo para evitar rebotes
  }

  // Guardar el estado actual del pulsador para comparar en la siguiente iteración
  ultimoEstadoSwitch = estadoSwitch;




  // EJECUCION DEL PROGRAMA
  if (encendido) {

    int valorHorizontal = analogRead(horzPin); // del joystick
    int valorVertical = analogRead(vertPin);   // del joystick

    // Determinar la dirección del joystick y establecer la acción correspondiente
    if (ARRIBA == movimientoJoystick(valorHorizontal, valorVertical)) {
        comandoAccion = REINICIAR;
    } else if (IZQUIERDA == movimientoJoystick(valorHorizontal, valorVertical)) {
        comandoAccion = PAUSAR;
    } else if (DERECHA == movimientoJoystick(valorHorizontal, valorVertical) && comandoAccion != IMAGEN) {
        comandoAccion = REANUDAR;
    } else if (ABAJO == movimientoJoystick(valorHorizontal, valorVertical)) {
        comandoAccion = IMAGEN;
        for (int fila = 0; fila < 8; fila++) {
          lc.setRow(matrizA, fila, patronCorazon[fila]);  // Aplica el patrón a cada fila
          lc.setRow(matrizB, fila, patronCorazon[fila]);  // Aplica el patrón a cada fila
        }
    }


    switch (comandoAccion) {
      case REINICIAR:
          resetRelojArena();
          comandoAccion = REANUDAR;
          break;

      case PAUSAR:
          // No se realizan acciones al pausar
          break;

      case REANUDAR:
          correrRelojArena();
          break;
      
      case IMAGEN:
          // Se ha cargado la imagen en la matriz. 
          // No se realizan más acciones hasta que se cambie la dirección del joystick.
          break;
    }        


  }  

  delay(100); // Pequeño retardo para facilitar la lectura y evitar cambios rápidos
  
}



Direccion movimientoJoystick(int x, int y){
  int rango = 200;

  if (y > (512 + rango)) {
      return ABAJO;
  } else if (y < (512 - rango)) {
      return ARRIBA;
  } else if (x < (512 - rango)) {
      return IZQUIERDA;
  } else if (x > (512 + rango)) {
      return DERECHA;
  }
  return CENTRO;
}
