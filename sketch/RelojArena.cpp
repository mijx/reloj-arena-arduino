#include <LedControl.h>
#include "RelojArena.h"
#include "PatronesLed.h"


// FUNCION PRINCIPAL
void correrRelojArena(){
  unsigned long tiempoActual = millis();

  // CAIDA DE UN NUEVO GRANO DE ARENA: CADA SEGUNDO
  if (tiempoActual - tiempoUltimaCaida >= intervalo && totalGranosCaidos <60) {
    tiempoUltimaCaida = tiempoActual;
     
    // MOVIMIENTO DE LA ARENA EN MATRIZ B
    quitarParticulaEnMatrizA();

    ledsMatrizB[0][0]=1; // Cuando comienza a caer, se ubica la particula en el origen

    totalGranosCaidos++;
  }
    
  // MOVIMIENTO DE LA ARENA EN MATRIZ B
  actualizarMatrizB();

}


// FUNCIONES SOBRE LA MATRIZ A
void quitarParticulaEnMatrizA(){
  lc.setRow(matrizA, modificacionMatrizA[totalGranosCaidos][0], modificacionMatrizA[totalGranosCaidos][1]);

}


// FUNCIONESS SOBRE MATRIZ B
void actualizarMatrizB(){
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      if(ledsMatrizB[x][y]==1 && moverParticulaEnMatrizB(x,y)){
          // Ya se ha desplazado el led que cambió de posición
          return; // Salir de la función porque los demás leds permanecen quietos
      }
    }
  }
}


bool moverParticulaEnMatrizB(int x, int y) {
  bool izq = izqLibre(x, y);
  bool der = derLibre(x, y);

  int nuevoX;
  int nuevoY;

  if (abajoLibre(x,y)) { // Descender
    nuevoX = x+1;
    nuevoY = y+1;
  } else if (izq && der) { // Si ambas direcciones están libres, elegir aleatoriamente
      if (random(0, 2) == 0){// Va a la izquierda
          nuevoX = x;
          nuevoY = y+1;
      } else{ // Va a la derecha
          nuevoX = x+1;
          nuevoY = y;
      }
  } else if (izq) { // Mover a la izquierda
      nuevoX = x;
      nuevoY = y+1;
  } else if (der) { // Mover a la derecha
      nuevoX = x+1;
      nuevoY = y;
  } else {
      return false; // Para no ejecutar el cambio de posición
  }

  cambioPosicionLedEnB(x, y, nuevoX, nuevoY);
  return true;

}



void resetRelojArena(){
  lc.clearDisplay(matrizA);     // Limpiar la matriz 1
  lc.clearDisplay(matrizB);     // Limpiar la matriz 2

  // Inicializamos el array de la matriz A según el patrón de leds
  for (int fila = 0; fila < 8; fila++) {
    lc.setRow(matrizA, fila, estadoInicialMatrizA[fila]);
  }

  // Inicializamos el array de la matriz B en 0 (todos los LEDs apagados)
  for (int x = 0; x < 8; x++) {
    for (int y = 0; y < 8; y++) {
      ledsMatrizB[x][y] = 0;
    }
  }

  totalGranosCaidos = 0; // Ningún grano de arena ha caido aun
  tiempoUltimaCaida = millis(); // Inicializar tiempo cuando comience a correr de nuevo

}


// FUNCIONES VERIFICADORAS EN MATRIZ B
bool abajoLibre(int x, int y) {
  // Si la partícula aún no llega al borde y si el led debajo está apagado
  return (y < 7) && (x<7)&&(ledsMatrizB[x+1][y + 1] == 0);
}

bool izqLibre(int x, int y) {
  return (x>=0)&&(y+1<=7)&&(ledsMatrizB[x][y+1] == 0);
}

bool derLibre(int x, int y) {
  return (x+1<=7)&&(y<=7)&&(ledsMatrizB[x+1][y] == 0);
}


// FUNCION PARA EDITAR LA POSICIÓN DE UN LED EN LA MATRIZ B
void cambioPosicionLedEnB(int x, int y, int nuevoX, int nuevoY){
  lc.setLed(matrizB, x, y, false);
  ledsMatrizB[x][y] = 0;
  lc.setLed(matrizB, nuevoX, nuevoY, true);
  ledsMatrizB[nuevoX][nuevoY] = 1;
}