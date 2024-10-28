// Variables externas
extern const int matrizA;
extern const int matrizB;

extern unsigned long tiempoUltimaCaida;
extern const unsigned long intervalo;

extern int ledsMatrizB[8][8];

extern LedControl lc;

extern int totalGranosCaidos; 


// Declaración de funciones


void resetGlobalVariables();

void resetRelojArena();


// FUNCIONES VERIFICADORAS EN MATRIZ B
bool abajoLibre(int x, int y);
bool izqLibre(int x, int y);
bool derLibre(int x, int y);

// FUNCIONES SOBRE LA MATRIZ A
void quitarParticulaEnMatrizA();

// FUNCIONESS SOBRE MATRIZ B
void actualizarMatrizB();
bool moverParticulaEnMatrizB(int x, int y);
void cambioPosicionLedEnB(int x, int y, int nuevoX, int nuevoY);

// FUNCION PRINCIPAL
void correrRelojArena();