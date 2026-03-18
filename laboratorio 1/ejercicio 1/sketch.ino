const int PIN_LED = 13;
int contador = 0;

void setup() {
  pinMode(PIN_LED, OUTPUT);
  Serial.begin(9600);

  Serial.println("=== Lab 01: Blink Iniciado ===");
}

void loop() {
  contador++;

  digitalWrite(PIN_LED, HIGH);
  Serial.print("[#");
  Serial.print(contador);
  Serial.println("] LED -> ENCENDIDO");
  delay(200);

  digitalWrite(PIN_LED, LOW);
  Serial.print("[#");
  Serial.print(contador);
  Serial.println("] LED -> APAGADO");
  delay(800);
}