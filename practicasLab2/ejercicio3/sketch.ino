// Definición de pines para botones
const int BOTON_A = 2;
const int BOTON_B = 3;

// Definición de pines para LEDs
const int LED_VERDE = 8;
const int LED_ROJO = 9;

void setup() {
  // Configuración de entradas
  pinMode(BOTON_A, INPUT);
  pinMode(BOTON_B, INPUT);
  
  // Configuración de salidas
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_ROJO, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Simulador Wokwi: Sistema de dos colores listo");
}

void loop() {
  // Lógica para el Botón A y LED Verde
  if (digitalRead(BOTON_A) == HIGH) {
    digitalWrite(LED_VERDE, HIGH);
    Serial.println("Boton A presionado: LED VERDE encendido");
  } else {
    digitalWrite(LED_VERDE, LOW);
  }

  // Lógica para el Botón B y LED Rojo
  if (digitalRead(BOTON_B) == HIGH) {
    digitalWrite(LED_ROJO, HIGH);
    Serial.println("Boton B presionado: LED ROJO encendido");
  } else {
    digitalWrite(LED_ROJO, LOW);
  }
  
  delay(10); // Estabilidad en la simulación
}