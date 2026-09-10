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

// Variables para animación suave
float pupilaX = 0;       
float pupilaY = 0;       
float destinoX = 0;      
float destinoY = 0;      

int radioOjo = 18;       // Ojo redondo grande (Radio 18px)
int aperturaParpado = 18; // Para controlar el parpadeo
unsigned long ultimoParpadeo = 0;
unsigned long ultimoMovimientoOjos = 0;

void setup() {
  Wire.begin();
  display.begin(0x3C, true);
}

// --- DIBUJO DE OJO REDONDO ANIME CON BRILLO ---
void renderizarOjoRedondo(int x, int y, float pX, float pY, int radioActual) {
  if (radioActual <= 1) return;

  // 1. Globo ocular redondo (Círculo blanco)
  display.fillCircle(x, y, radioActual, SH110X_WHITE);

  // 2. Pupila redonda negra (Interior)
  int radioPupila = radioActual / 2;
  if (radioPupila > 2) {
    display.fillCircle(x + (int)pX, y + (int)pY, radioPupila, SH110X_BLACK);
  }

  // 3. Brillo de luz superior (Círculo blanco pequeño)
  if (radioActual > 8) {
    display.fillCircle(x - 4 + (int)pX, y - 4 + (int)pY, 3, SH110X_WHITE);
  }

  // 4. Segundo brillo inferior
  if (radioActual > 14) {
    display.fillCircle(x + 5 + (int)pX, y + 5 + (int)pY, 1, SH110X_WHITE);
  }
}

// --- RENDERIZADO DE LA CARA ---
void renderizarEscena() {
  display.clearDisplay();

  // Ojo Izquierdo (X: 35) y Ojo Derecho (X: 93)
  renderizarOjoRedondo(35, 30, pupilaX, pupilaY, aperturaParpado);
  renderizarOjoRedondo(93, 30, pupilaX, pupilaY, aperturaParpado);

  // Boca curva
  if (aperturaParpado > 6) {
    display.drawLine(58, 52, 64, 55, SH110X_WHITE);
    display.drawLine(64, 55, 70, 52, SH110X_WHITE);
  } else {
    display.drawLine(60, 52, 68, 52, SH110X_WHITE);
  }

  display.display();
}

// --- PARPADEO FLUIDO ---
void animacionParpadeo() {
  for (int r = radioOjo; r >= 0; r -= 3) {
    aperturaParpado = r;
    renderizarEscena();
    delay(15);
  }
  for (int r = 0; r <= radioOjo; r += 3) {
    aperturaParpado = r;
    renderizarEscena();
    delay(15);
  }
}

void loop() {
  char tecla = teclado.getKey();

  if (tecla) {
    switch (tecla) {
      case '1': destinoX = -5; destinoY = 0; break;  // Izquierda
      case '3': destinoX = 5;  destinoY = 0; break;  // Derecha
      case '2': destinoX = 0;  destinoY = 0; break;  // Centro
      case '5': destinoX = 0;  destinoY = -4; break; // Arriba
      case '8': destinoX = 0;  destinoY = 4; break;  // Abajo
      case 'A': animacionParpadeo(); break;
    }
  }

  // Movimiento autónomo aleatorio
  if (millis() - ultimoMovimientoOjos > 3000) {
    destinoX = random(-5, 6);
    destinoY = random(-3, 4);
    ultimoMovimientoOjos = millis();
  }

  // Transición suave (Interpolación)
  pupilaX += (destinoX - pupilaX) * 0.25;
  pupilaY += (destinoY - pupilaY) * 0.25;

  // Parpadeo cada 4 segundos
  if (millis() - ultimoParpadeo > 4000) {
    animacionParpadeo();
    ultimoParpadeo = millis();
  } else {
    renderizarEscena();
  }

  delay(20);
}