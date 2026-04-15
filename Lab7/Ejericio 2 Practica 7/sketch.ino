int led = 9;
int buzzer = 8;

void setup() {
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // Encender LED y buzzer
  digitalWrite(led, HIGH);
  digitalWrite(buzzer, HIGH);
  Serial.println("LED ENCENDIDO - BUZZER SONANDO");
  delay(1000);

  // Apagar LED y buzzer
  digitalWrite(led, LOW);
  digitalWrite(buzzer, LOW);
  Serial.println("LED APAGADO - BUZZER APAGADO");
  delay(1000);
}