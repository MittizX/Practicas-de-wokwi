const int PIN_LED = 13;
int contador = 0;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(9600);

  Serial.println("=== Ejercicio 2: SOS ===");
}

void loop() {
  contador++;
  Serial.print("Repeticion #");
  Serial.println(contador);

  // 3 cortos (200 ms)
  for (int i = 0; i < 3; i++) {
    digitalWrite(PIN_LED, HIGH);
    Serial.println("CORTO");
    delay(200);

    digitalWrite(PIN_LED, LOW);
    delay(200);
  }

  // 3 largos (600 ms)
  for (int i = 0; i < 3; i++) {
    digitalWrite(PIN_LED, HIGH);
    Serial.println("LARGO");
    delay(600);

    digitalWrite(PIN_LED, LOW);
    delay(200);
  }

  // 3 cortos (200 ms)
  for (int i = 0; i < 3; i++) {
    digitalWrite(PIN_LED, HIGH);
    Serial.println("CORTO");
    delay(200);

    digitalWrite(PIN_LED, LOW);
    delay(200);
  }

  // Pausa de 2 segundos entre repeticiones
  delay(2000);
}