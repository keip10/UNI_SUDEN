#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Pines del teclado 4x4
const byte FILAS[4] = {9, 8, 7, 6};
const byte COLUMNAS[4] = {5, 4, 3, 2};

// Lectura de teclas simplificada
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
  display.println(F(" CYBER-UNI & GAMES "));
  display.println(F("-----------------------"));
  display.setCursor(0, 18);
  display.println(F("[1] Test Reflejos"));
  display.println(F("[2] Esquivar Paredes"));
  display.println(F("[3] Ojos"));
  display.setCursor(0, 52);
  display.println(F("Elige una opcion..."));
  display.display();
}

// --- ANIMACIÓN DE OJOS / MASCOTITA ---
void dibujarOjos(int desplazamientoX, bool parpadeo) {
  display.clearDisplay();
  
  if (parpadeo) {
    // Ojos cerrados (Líneas)
    display.fillRect(25 + desplazamientoX, 28, 26, 4, SH110X_WHITE);
    display.fillRect(77 + desplazamientoX, 28, 26, 4, SH110X_WHITE);
  } else {
    // Ojos abiertos (Círculos exteriores)
    display.fillCircle(38 + desplazamientoX, 30, 15, SH110X_WHITE);
    display.fillCircle(90 + desplazamientoX, 30, 15, SH110X_WHITE);
    
    // Pupilas (Círculos negros)
    display.fillCircle(38 + desplazamientoX, 30, 6, SH110X_BLACK);
    display.fillCircle(90 + desplazamientoX, 30, 6, SH110X_BLACK);
  }
  
  display.display();
}

void opcionOjos() {
  esperarSoltarTecla();
  
  // Ciclo de animación de ojos (mira a los lados y parpadea)
  for (int i = 0; i < 2; i++) {
    dibujarOjos(0, false);   // Ojos al centro
    delay(1200);
    dibujarOjos(-8, false);  // Mira izquierda
    delay(800);
    dibujarOjos(8, false);   // Mira derecha
    delay(800);
    dibujarOjos(0, true);    // Parpadeo
    delay(300);
    dibujarOjos(0, false);   // Abre ojos
    delay(1000);
  }
  
  menuPrincipal();
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

// --- JUEGO 2: ESQUIVAR PAREDES (FLUIDO Y MAS LENTO) ---
void juegoEsquivar() {
  esperarSoltarTecla();
  int jugadorY = 28;
  int paredX = 120;
  int huecoY = random(5, 30);
  int puntos = 0;
  bool juegoActivo = true;

  while (juegoActivo) {
    char tecla = leerTeclas();

    // Movimiento más suave del jugador
    if ((tecla == '1' || tecla == '2' || tecla == '3' || tecla == 'A') && jugadorY > 0) {
      jugadorY -= 3;
    }
    if ((tecla == '4' || tecla == '5' || tecla == '6' || tecla == 'B' || tecla == '7' || tecla == '8' || tecla == '9' || tecla == 'C' || tecla == '0' || tecla == '*') && jugadorY < 58) {
      jugadorY += 3;
    }

    // Avance de velocidad controlado para evitar tirones en pantalla (máximo 5px)
    int velocidadPared = 2 + (puntos / 4); 
    if (velocidadPared > 5) velocidadPared = 5; 

    paredX -= velocidadPared;

    // Reinicio de pared al salir de pantalla
    if (paredX < -8) {
      paredX = 120;
      huecoY = random(5, 30);
      puntos++;
    }

    // Comprobación de Colisión
    if (paredX <= 16 && paredX >= 6) {
      if (jugadorY < huecoY || (jugadorY + 6) > (huecoY + 22)) {
        juegoActivo = false;
      }
    }

    // Renderizado OLED
    display.clearDisplay();
    display.fillRect(10, jugadorY, 6, 6, SH110X_WHITE);                         // Jugador
    display.fillRect(paredX, 0, 8, huecoY, SH110X_WHITE);                        // Pared Superior
    display.fillRect(paredX, huecoY + 22, 8, 64 - (huecoY + 22), SH110X_WHITE); // Pared Inferior

    display.setTextSize(1);
    display.setCursor(0, 0);
    display.print(puntos);
    display.display();

    // Retardo para refresco estable
    delay(30);
  }

  // Fin del juego
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

void loop() {
  char tecla = leerTeclas();

  if (tecla == '1') {
    juegoReflejos();
  } 
  else if (tecla == '2' || tecla == 'B') {
    juegoEsquivar();
  }
  else if (tecla == '3' || tecla == 'C') {
    opcionOjos();
  }
}