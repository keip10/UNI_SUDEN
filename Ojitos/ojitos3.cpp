#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Keypad.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Teclado 4x4
const byte FILAS = 4;
const byte COLUMNAS = 4;
char teclas[FILAS][COLUMNAS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte pinesFilas[FILAS] = {9, 8, 7, 6};
byte pinesColumnas[COLUMNAS] = {5, 4, 3, 2};
Keypad teclado = Keypad(makeKeymap(teclas), pinesFilas, pinesColumnas, FILAS, COLUMNAS);

unsigned long ultimoParpadeo = 0;
int modoActual = 0; // 0: Robot, 1: Terminal

void setup() {
  Wire.begin();
  display.begin(0x3C, true);
  dibujarCaraCentrada();
}

// --- DIBUJO DE OJOS Y ANIMACIONES ---
void dibujarCaraCentrada() {
  display.clearDisplay();
  display.fillCircle(40, 32, 12, SH110X_WHITE); // Ojo Izquierdo
  display.fillCircle(88, 32, 12, SH110X_WHITE); // Ojo Derecho
  display.fillRect(54, 48, 20, 4, SH110X_WHITE);  // Boca
  display.display();
}

void parpadear() {
  // Pestañeo: cerrar ojos
  display.clearDisplay();
  display.fillRect(28, 30, 24, 4, SH110X_WHITE);
  display.fillRect(76, 30, 24, 4, SH110X_WHITE);
  display.fillRect(54, 48, 20, 4, SH110X_WHITE);
  display.display();
  delay(150);
  dibujarCaraCentrada();
}

void mirarIzquierda() {
  display.clearDisplay();
  display.fillCircle(30, 32, 12, SH110X_WHITE);
  display.fillCircle(78, 32, 12, SH110X_WHITE);
  display.fillRect(54, 48, 20, 4, SH110X_WHITE);
  display.display();
}

void mirarDerecha() {
  display.clearDisplay();
  display.fillCircle(50, 32, 12, SH110X_WHITE);
  display.fillCircle(98, 32, 12, SH110X_WHITE);
  display.fillRect(54, 48, 20, 4, SH110X_WHITE);
  display.display();
}

// --- MODO TERMINAL HACKER ---
void animacionHack() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  
  display.setCursor(0, 0);
  display.println(F("> SCANNING SYSTEM..."));
  display.display();
  delay(400);

  // Barra de carga animada
  for (int i = 0; i <= 100; i += 20) {
    display.setCursor(0, 20);
    display.print(F("LOADING: "));
    display.print(i);
    display.println(F("%"));
    display.fillRect(0, 35, (i * 1.2), 8, SH110X_WHITE);
    display.display();
    delay(200);
  }

  display.setCursor(0, 50);
  display.println(F("[ACCESS GRANTED]"));
  display.display();
  delay(1500);
  modoActual = 0;
  dibujarCaraCentrada();
}

void loop() {
  char tecla = teclado.getKey();

  // Control manual por teclado
  if (tecla) {
    if (tecla == '1') {
      mirarIzquierda();
    } else if (tecla == '3') {
      mirarDerecha();
    } else if (tecla == 'A') {
      animacionHack();
    } else {
      dibujarCaraCentrada();
    }
  }

  // Animación automática de parpadeo cada 4 segundos
  if (modoActual == 0 && millis() - ultimoParpadeo > 4000) {
    parpadear();
    ultimoParpadeo = millis();
  }
}