#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Pines del teclado 4x4
const byte FILAS[4] = {9, 8, 7, 6};
const byte COLUMNAS[4] = {5, 4, 3, 2};

void setup() {
  Wire.begin();
  display.begin(0x3C, true);

  for (int i = 0; i < 4; i++) {
    pinMode(FILAS[i], OUTPUT);
    digitalWrite(FILAS[i], HIGH);
    pinMode(COLUMNAS[i], INPUT_PULLUP);
  }

  menuPrincipal();
}

// Lectura de teclas simplificada y anti-rebote
char leerTeclas() {
  char mapaTeclas[4][4] = {
    {'1', '2', '3', 'A'},
    {'4', '5', '6', 'B'},
    {'7', '8', '9', 'C'},
    {'*', '0', '#', 'D'}
  };

  for (int f = 0; f < 4; f++) {
    digitalWrite(FILAS[f], LOW);
    for (int c = 0; c < 4; c++) {
      if (digitalRead(COLUMNAS[c]) == LOW) {
        delay(10);
        digitalWrite(FILAS[f], HIGH);
        return mapaTeclas[f][c];
      }
    }
    digitalWrite(FILAS[f], HIGH);
  }
  return 0;
}

void esperarSoltarTecla() {
  while (leerTeclas() != 0) {
    delay(10);
  }
}

void menuPrincipal() {
  esperarSoltarTecla();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(10, 0);
  display.println(F("== CYBER-GAMES =="));
  display.println(F("------------------"));
  display.setCursor(0, 20);
  display.println(F("[1] Test Reflejos"));
  display.println(F("[2 o B] Esquivar"));
  display.setCursor(0, 50);
  display.println(F("Presiona 1, 2 o B..."));
  display.display();
}

// --- JUEGO 1: TEST DE REFLEJOS ---
void juegoReflejos() {
  esperarSoltarTecla();
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(10, 5);
  display.println(F("PREPARATE..."));
  display.setCursor(10, 25);
  display.println(F("Presiona CUALQUIER"));
  display.println(F("tecla al ver 'YA'"));
  display.display();

  delay(random(2000, 4000));

  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(20, 22);
  display.println(F("!! YA !!"));
  display.display();

  unsigned long tiempoInicio = millis();
  
  while (leerTeclas() == 0);

  unsigned long tiempoReaccion = millis() - tiempoInicio;

  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(10, 10);
  display.println(F("TU TIEMPO:"));
  display.setTextSize(2);
  display.setCursor(10, 30);
  display.print(tiempoReaccion);
  display.println(F(" ms"));
  display.display();

  delay(2000);
  menuPrincipal();
}

// --- JUEGO 2: ESQUIVAR PAREDES (CON VELOCIDAD INCREMENTAL) ---
void juegoEsquivar() {
  esperarSoltarTecla();
  int jugadorY = 28;
  int paredX = 120;
  int huecoY = random(5, 30);
  int puntos = 0;
  bool juegoActivo = true;

  while (juegoActivo) {
    char tecla = leerTeclas();

    // Mover arriba: Fila superior (1, 2, 3, A)
    if ((tecla == '1' || tecla == '2' || tecla == '3' || tecla == 'A') && jugadorY > 0) {
      jugadorY -= 4;
    }
    // Mover abajo: Resto de teclas
    if ((tecla == '4' || tecla == '5' || tecla == '6' || tecla == 'B' || tecla == '7' || tecla == '8' || tecla == '9' || tecla == 'C' || tecla == '0' || tecla == '*') && jugadorY < 58) {
      jugadorY += 4;
    }

    // Aumento de velocidad progresivo:
    // A más puntos, mayor avance de la pared (velocidadPared)
    int velocidadPared = 3 + (puntos / 3); 
    if (velocidadPared > 8) velocidadPared = 8; // Límite máximo para mantener el juego jugable

    paredX -= velocidadPared;

    if (paredX < -8) {
      paredX = 120;
      huecoY = random(5, 30);
      puntos++;
    }

    // Comprobación de Choque
    if (paredX <= 16 && paredX >= 6) {
      if (jugadorY < huecoY || (jugadorY + 6) > (huecoY + 22)) {
        juegoActivo = false;
      }
    }

    // Dibujar en pantalla OLED
    display.clearDisplay();
    display.fillRect(10, jugadorY, 6, 6, SH110X_WHITE);                         // Jugador
    display.fillRect(paredX, 0, 8, huecoY, SH110X_WHITE);                        // Pared Arriba
    display.fillRect(paredX, huecoY + 22, 8, 64 - (huecoY + 22), SH110X_WHITE); // Pared Abajo

    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print(puntos);
    display.display();

    // A medida que aumentan los puntos, disminuye el tiempo de refresco (mínimo 10ms)
    int retardoBucle = 25 - (puntos / 2);
    if (retardoBucle < 10) retardoBucle = 10;

    delay(retardoBucle);
  }

  // Pantalla de Game Over
  display.clearDisplay();
  display.setTextSize(2);
  display.setCursor(10, 15);
  display.println(F("GAME OVER"));
  display.setTextSize(1);
  display.setCursor(10, 40);
  display.print(F("PUNTOS: "));
  display.print(puntos);
  display.display();

  delay(2000);
  menuPrincipal();
}

void loop() {
  char tecla = leerTeclas();

  if (tecla == '1') {
    juegoReflejos();
  } 
  else if (tecla == '2' || tecla == 'B' || tecla == '4' || tecla == '5') {
    juegoEsquivar();
  }
}