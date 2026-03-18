// Definición de pines
const int PIN_BOTON = 2;   // Pulsador de incremento
const int PIN_RESET = 3;   // Nuevo pulsador de reset
const int PIN_LED = 8;     

int contador = 0; // Variable para almacenar el conteo

void setup() {
  pinMode(PIN_BOTON, INPUT);
  pinMode(PIN_RESET, INPUT); // Configuración del nuevo pin
  pinMode(PIN_LED, OUTPUT);
  
  Serial.begin(9600);
  Serial.println("Sistema de Conteo Iniciado");
}

void loop() {
  // Lógica para el botón de incremento (Pin 2)
  if (digitalRead(PIN_BOTON) == HIGH) {
    contador++;
    digitalWrite(PIN_LED, HIGH);
    Serial.print("Valor actual: ");
    Serial.println(contador);
    
    // Antirrebote simple para no contar de más
    while(digitalRead(PIN_BOTON) == HIGH); 
    delay(50);
  }

  // Lógica para el botón de RESET (Pin 3)
  if (digitalRead(PIN_RESET) == HIGH) {
    contador = 0; // Reiniciamos la variable
    digitalWrite(PIN_LED, LOW);
    
    // IMPORTANTE: Usamos comillas dobles "" para evitar el error de los números extraños
    Serial.println("CONTEO REINICIADO");
    
    // Esperar a que suelte el botón
    while(digitalRead(PIN_RESET) == HIGH);
    delay(50);
  }
  
  digitalWrite(PIN_LED, LOW);
}