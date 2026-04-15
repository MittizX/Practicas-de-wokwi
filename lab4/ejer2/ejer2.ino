// Lab04_LDR_Luz.ino

const int PIN_LDR = A1;
const int PIN_LED = 9;
const int PIN_BUZZER = 8;

const int LDR_OSCURO = 800;
const int LDR_LUZ = 100;

void setup() {
  Serial.begin(9600);

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  Serial.println("LDR_raw\tLuz%\tEstado");
}

void loop() {
  int lectura_ldr = analogRead(PIN_LDR);

  int luz_pct = map(lectura_ldr, LDR_OSCURO, LDR_LUZ, 0, 100);
  luz_pct = constrain(luz_pct, 0, 100);

  int brillo_led = map(luz_pct, 0, 100, 255, 0);
  analogWrite(PIN_LED, brillo_led);

  const char* estado;

  if (lectura_ldr > 55) {
      estado = "ILUMINADO - BUZZER OFF";
  noTone(PIN_BUZZER);

} else {

    estado = "MUY OSCURO - BUZZER ON";
  tone(PIN_BUZZER, 1000);
}

  Serial.print(lectura_ldr);
  Serial.print("\t");
  Serial.print(luz_pct);
  Serial.print("\t");
  Serial.println(estado);

  delay(300);
}