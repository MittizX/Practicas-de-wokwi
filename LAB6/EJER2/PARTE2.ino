// Lab06_Servo_Potenciometro.ino
#include <Servo.h>
Servo mi_servo;
const int PIN_SERVO = 11;
const int PIN_POT = A0;
int angulo_anterior = -1; // Para detectar cambios
void setup() {
 mi_servo.attach(PIN_SERVO); // Asociar servo al pin
 mi_servo.write(90); // Posicion inicial: centro
 Serial.begin(9600);
 Serial.println('Servo listo. Gira el potenciometro.');
 Serial.println('Pot_ADC\tAngulo');
}
void loop() {
 int pot_val = analogRead(PIN_POT); // 0-1023
 int angulo = map(pot_val, 0, 1023, 0, 180); // 0-180 grados
 // Solo mover si el angulo cambio (evita vibracion por ruido ADC)
 if (abs(angulo - angulo_anterior) >= 2) {
 mi_servo.write(angulo);
 angulo_anterior = angulo;
 Serial.print(pot_val); Serial.print('\t');
 Serial.println(angulo);
 }
 delay(20);
}