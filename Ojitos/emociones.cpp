#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Keypad.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// Inicializar la pantalla OLED SH1106
Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Configuración del Teclado Matricial 4x4
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

void setup() {
  Wire.begin();
  display.begin(0x3C, true);
  
  // Mostrar cara feliz por defecto al encender
  caraFeliz();
}

// 1. CARA FELIZ (Tecla 1)
void caraFeliz() {
  display.clearDisplay();
  // Ojos
  display.fillCircle(40, 28, 9, SH110X_WHITE);
  display.fillCircle(88, 28, 9, SH110X_WHITE);
  // Boca sonriente (curva hacia arriba)
  display.drawLine(45, 48, 55, 54, SH110X_WHITE);
  display.drawLine(55, 54, 73, 54, SH110X_WHITE);
  display.drawLine(73, 54, 83, 48, SH110X_WHITE);
  display.display();
}

// 2. CARA TRISTE (Tecla 2)
void caraTriste() {
  display.clearDisplay();
  // Ojos
  display.fillCircle(40, 30, 9, SH110X_WHITE);
  display.fillCircle(88, 30, 9, SH110X_WHITE);
  // Cejas tristes (caídas hacia afuera)
  display.drawLine(25, 18, 50, 14, SH110X_WHITE);
  display.drawLine(103, 18, 78, 14, SH110X_WHITE);
  // Boca triste (curva hacia abajo)
  display.drawLine(45, 54, 55, 48, SH110X_WHITE);
  display.drawLine(55, 48, 73, 48, SH110X_WHITE);
  display.drawLine(73, 48, 83, 54, SH110X_WHITE);
  display.display();
}

// 3. CARA ENOJADA (Tecla 3)
void caraEnojada() {
  display.clearDisplay();
  // Ojos
  display.fillCircle(40, 32, 10, SH110X_WHITE);
  display.fillCircle(88, 32, 10, SH110X_WHITE);
  // Cejas enojadas (inclinadas hacia el centro)
  display.drawLine(25, 15, 52, 24, SH110X_WHITE);
  display.drawLine(25, 16, 52, 25, SH110X_WHITE);
  display.drawLine(103, 15, 76, 24, SH110X_WHITE);
  display.drawLine(103, 16, 76, 25, SH110X_WHITE);
  // Boca enojada/recta
  display.drawLine(50, 50, 78, 50, SH110X_WHITE);
  display.drawLine(50, 51, 78, 51, SH110X_WHITE);
  display.display();
}

void loop() {
  char tecla = teclado.getKey();

  if (tecla) {
    if (tecla == '1') {
      caraFeliz();
    } 
    else if (tecla == '2') {
      caraTriste();
    } 
    else if (tecla == '3') {
      caraEnojada();
    }
  }
}