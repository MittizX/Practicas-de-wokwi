#include <Wire.h>
#include <Adafruit_BMP085.h>   // ← ESTA ES LA CORRECTA PARA BMP180
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C

Adafruit_BMP085 bmp;
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void setup() {
  Serial.begin(9600);

  // Inicializar OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("Error OLED");
    while (true);
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(15, 25);
  display.println("Iniciando...");
  display.display();

  // Inicializar BMP180
  if (!bmp.begin()) {
    Serial.println("Error BMP180");
    display.clearDisplay();
    display.setCursor(0, 25);
    display.println("ERROR BMP180");
    display.display();
    while (true);
  }

  Serial.println("Sistema listo");
}

void loop() {
  float temp = bmp.readTemperature();      // °C
  float pres = bmp.readPressure() / 100.0; // hPa
  float alt  = bmp.readAltitude();         // metros

  display.clearDisplay();

  // Título
  display.setTextSize(1);
  display.setCursor(30, 0);
  display.println("BMP180");

  // Temperatura grande
  display.setTextSize(2);
  display.setCursor(0, 15);
  display.print(temp, 1);
  display.println(" C");

  // Presión
  display.setTextSize(1);
  display.setCursor(0, 40);
  display.print("Pres: ");
  display.print(pres, 1);
  display.println(" hPa");

  // Altitud
  display.setCursor(0, 55);
  display.print("Alt: ");
  display.print(alt, 0);
  display.println(" m");

  display.display();

  // Serial
  Serial.print("Temp: ");
  Serial.print(temp);
  Serial.print(" C | Pres: ");
  Serial.print(pres);
  Serial.print(" hPa | Alt: ");
  Serial.print(alt);
  Serial.println(" m");

  delay(1000);
}