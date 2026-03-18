const int PIN_BOTON = 2;
const int PIN_RESET = 3;
const int PIN_LED = 8;

int contador = 0;

void setup() {
  pinMode(PIN_BOTON, INPUT);
  pinMode(PIN_RESET, INPUT);
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(9600);
  Serial.println("Ejercicio 2: Parpadeo por Conteo");
}

void loop() {
  // Lógica de Incremento
  if (digitalRead(PIN_BOTON) == HIGH) {
    contador++;
    Serial.print("Contador: ");
    Serial.println(contador);

    // Bucle para parpadear el LED el número de veces del contador
    for (int i = 0; i < contador; i++) {
      digitalWrite(PIN_LED, HIGH);
      delay(200); // Tiempo encendido
      digitalWrite(PIN_LED, LOW);
      delay(200); // Tiempo apagado
    }

    // Antirrebote: Esperar a que suelte el botón
    while(digitalRead(PIN_BOTON) == HIGH);
    delay(50);
  }

  // Lógica de Reset
  if (digitalRead(PIN_RESET) == HIGH) {
    contador = 0;
    Serial.println("CONTEO REINICIADO");
    
    // Feedback visual de reset (un parpadeo largo)
    digitalWrite(PIN_LED, LOW);
    
    while(digitalRead(PIN_RESET) == HIGH);
    delay(50);
  }
}