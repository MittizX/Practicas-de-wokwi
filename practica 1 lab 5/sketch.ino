#include "DHT.h"

#define PIN_DHT 7
#define TIPO_DHT DHT22

#define LED_VERDE 8
#define LED_ROJO 9
#define BUZZER 6

DHT dht(PIN_DHT, TIPO_DHT);

// Umbrales
const float TEMP_ALTA = 30.0;
const float TEMP_BAJA = 10.0;
const float HUM_ALTA  = 80.0;

void setup() {
  Serial.begin(9600);
  
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  dht.begin();

  Serial.println("=== LAB 05: DHT22 ===");
}

void loop() {
  float humedad = dht.readHumidity();
  float tempC = dht.readTemperature();
  float tempF = dht.readTemperature(true);

  // Verificar lectura
  if (isnan(humedad) || isnan(tempC)) {
    Serial.println("Error leyendo DHT22");
    return;
  }

  // Índice de calor
  float indiceCalor = dht.computeHeatIndex(tempC, humedad, false);

  // Mostrar datos
  Serial.print("Temp: ");
  Serial.print(tempC);
  Serial.print(" °C | ");

  Serial.print(tempF);
  Serial.print(" °F | Humedad: ");
  Serial.print(humedad);
  Serial.print(" % | Heat Index: ");
  Serial.println(indiceCalor);

  // Lógica de alertas
  if (tempC > TEMP_ALTA) {
    digitalWrite(LED_ROJO, HIGH);
    digitalWrite(LED_VERDE, LOW);
    tone(BUZZER, 1000, 200);
    
  } else if (tempC < TEMP_BAJA) {
    digitalWrite(LED_ROJO, HIGH);
    digitalWrite(LED_VERDE, LOW);
    tone(BUZZER, 500, 200);

  } else {
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_ROJO, LOW);
    noTone(BUZZER);
  }

  delay(2000); // IMPORTANTE: DHT22 necesita 2s
}