#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <Keypad.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SH1106G display = Adafruit_SH1106G(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// Teclado Matricial 4x4
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

void setup() {
  Wire.begin();
  display.begin(0x3C, true);
  ojosAnimeNormal();
}

// --- DIBUJO DE OJO ANIME INDIVIDUAL ---
void dibujarOjoAnime(int x, int y, int offsetX, int offsetY) {
  // 1. Contorno/Base del ojo (Rectángulo redondeado blanco)
  display.fillRoundRect(x, y, 26, 36, 6, SH110X_WHITE);
  
  // 2. Pupila interior (Hueco negro en el centro, ajustable para mirar a lados)
  display.fillRoundRect(x + 6 + offsetX, y + 8 + offsetY, 14, 22, 4, SH110X_BLACK);
  
  // 3. Brillo principal superior (Punto blanco de luz anime)
  display.fillRect(x + 4 + offsetX, y + 4, 6, 6, SH110X_WHITE);
  
  // 4. Segundo brillo inferior más pequeño
  display.fillRect(x + 16 + offsetX, y + 24, 3, 3, SH110X_WHITE);
}

// --- EXPRESIONES ANIME ---
void ojosAnimeNormal() {
  display.clearDisplay();
  
  // Ojo Izquierdo y Derecho centrados
  dibujarOjoAnime(25, 12, 0, 0);
  dibujarOjoAnime(77, 12, 0, 0);
  
  // Boca pequeña kawaii (3 líneas cortas)
  display.drawLine(58, 52, 64, 55, SH110X_WHITE);
  display.drawLine(64, 55, 70, 52, SH110X_WHITE);
  
  display.display();
}

void ojosAnimeMirarLado(int direccion) { // -3 Izquierda, +3 Derecha
  display.clearDisplay();
  
  dibujarOjoAnime(25, 12, direccion, 0);
  dibujarOjoAnime(77, 12, direccion, 0);
  
  // Boca
  display.drawLine(58, 52, 70, 52, SH110X_WHITE);
  
  display.display();
}

void parpadeoAnime() {
  // Cierre de párpados estilo línea curva
  display.clearDisplay();
  
  display.drawLine(25, 30, 51, 30, SH110X_WHITE);
  display.drawLine(25, 31, 51, 31, SH110X_WHITE);
  
  display.drawLine(77, 30, 103, 30, SH110X_WHITE);
  display.drawLine(77, 31, 103, 31, SH110X_WHITE);
  
  display.drawLine(60, 50, 68, 50, SH110X_WHITE);
  
  display.display();
  delay(120);
  ojosAnimeNormal();
}

void modoTerminal() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  
  display.setCursor(0, 0);
  display.println(F(">[CYBERDECK OS]"));
  display.println(F("------------------"));
  display.println(F("ANIME CORE: LOADED"));
  display.println(F("STATUS: OVERCLOCKED"));
  
  for (int i = 0; i <= 100; i += 25) {
    display.fillRect(0, 48, (i * 1.2), 6, SH110X_WHITE);
    display.display();
    delay(180);
  }
  
  delay(1000);
  ojosAnimeNormal();
}

void loop() {
  char tecla = teclado.getKey();

  if (tecla) {
    switch (tecla) {
      case '1':
        ojosAnimeMirarLado(-3); // Mirar izquierda
        break;
      case '3':
        ojosAnimeMirarLado(3);  // Mirar derecha
        break;
      case '2':
        ojosAnimeNormal();     // Volver al centro
        break;
      case 'A':
        modoTerminal();        // Activar modo hacker
        break;
    }
  }

  // Parpadeo automático estilo anime cada 3.5 segundos
  if (millis() - ultimoParpadeo > 3500) {
    parpadeoAnime();
    ultimoParpadeo = millis();
  }
}