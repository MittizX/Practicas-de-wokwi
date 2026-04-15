#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <Adafruit_BMP280.h>
#include <SD.h>
#include <SPI.h>

// ===================== PINES =====================
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
#define SDA_PIN 21
#define SCL_PIN 22

#define DHTPIN 4
#define DHTTYPE DHT22

#define SD_CS_PIN 5       // Chip Select de la SD

#define BUZZER_PIN 25
#define BTN_PIN 34        // Botón (INPUT con pull-up externo recomendado)

#define LED_VERDE 26
#define LED_ROJO  27
#define LED_AZUL  14

// ===================== OBJETOS =====================
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
DHT dht(DHTPIN, DHTTYPE);
Adafruit_BMP280 bmp;

// ===================== FSM =====================
enum Estado { ESTADO_MONITOR, ESTADO_ALERTA, ESTADO_LOG };
Estado estadoActual = ESTADO_MONITOR;

// ===================== TIEMPOS =====================
unsigned long tiempoAnterior    = 0;
const long intervaloGuardado    = 5000;   // guardar CSV cada 5 s
const long intervaloLectura     = 2000;   // leer sensores cada 2 s

unsigned long tiempoBoton       = 0;
const long tiempoLongPress      = 3000;   // 3 s pulsado → ESTADO_LOG

unsigned long tiempoParpadeo    = 0;
const long intervaloParpadeo    = 400;
bool ledRojoEstado              = false;

unsigned long tiempoUltimaLectura = 0;

// ===================== DATOS =====================
float humedad   = 0;
float tempDHT   = 0;
float presion   = 0;
long  totalRegistros = 0;

bool btnPresionado  = false;
bool enLongPress    = false;

const char* nombreArchivo = "/datos.csv";

// ===================== PROTOTIPOS =====================
void leerSensores();
void guardarCSV();
void contarRegistros();
void mostrarMonitor();
void mostrarAlerta();
void mostrarLog();
void manejarLEDs();
void manejarBuzzer();
void manejarBoton();
void apagarLEDs();

// ========================================================
void setup() {
  Serial.begin(115200);
  Wire.begin(SDA_PIN, SCL_PIN);

  // Pines
  pinMode(BTN_PIN,    INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_VERDE,  OUTPUT);
  pinMode(LED_ROJO,   OUTPUT);
  pinMode(LED_AZUL,   OUTPUT);

  // OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("Error OLED");
    while (true);
  }
  display.clearDisplay();
  display.setTextColor(WHITE);

  // DHT22
  dht.begin();

  // BMP280
  if (!bmp.begin(0x76)) {
    Serial.println("No se encontró BMP280");
    while (true);
  }

  // SD
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("Error SD");
    // No bloqueamos; el sistema sigue sin SD
  } else {
    // Escribir cabecera si el archivo es nuevo
    if (!SD.exists(nombreArchivo)) {
      File f = SD.open(nombreArchivo, FILE_WRITE);
      if (f) {
        f.println("timestamp_ms,temperatura_C,humedad_%,presion_hPa");
        f.close();
      }
    }
    contarRegistros();
  }

  Serial.println("Sistema iniciado");
}

// ========================================================
void loop() {
  unsigned long ahora = millis();

  // --- Lectura de sensores periódica ---
  if (ahora - tiempoUltimaLectura >= intervaloLectura) {
    tiempoUltimaLectura = ahora;
    leerSensores();
  }

  // --- Guardar CSV cada 5 s ---
  if (ahora - tiempoAnterior >= intervaloGuardado) {
    tiempoAnterior = ahora;
    guardarCSV();
  }

  // --- Manejar botón ---
  manejarBoton();

  // --- Determinar estado (solo si no estamos en LOG) ---
  if (estadoActual != ESTADO_LOG) {
    if (tempDHT > 35.0 || humedad > 85.0) {
      estadoActual = ESTADO_ALERTA;
    } else {
      estadoActual = ESTADO_MONITOR;
    }
  }

  // --- OLED según estado ---
  switch (estadoActual) {
    case ESTADO_MONITOR: mostrarMonitor(); break;
    case ESTADO_ALERTA:  mostrarAlerta();  break;
    case ESTADO_LOG:     mostrarLog();     break;
  }

  // --- LEDs y buzzer ---
  manejarLEDs();
  manejarBuzzer();

  delay(50); // pequeño delay para estabilidad
}

// ========================================================
void leerSensores() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float p = bmp.readPressure() / 100.0;

  if (!isnan(h)) humedad = h;
  if (!isnan(t)) tempDHT = t;
  presion = p;

  Serial.printf("[Sensor] T:%.1f°C  H:%.1f%%  P:%.1fhPa\n", tempDHT, humedad, presion);
}

// ========================================================
void guardarCSV() {
  if (isnan(humedad) || isnan(tempDHT)) return;

File f = SD.open(nombreArchivo, FILE_WRITE | O_APPEND);
  if (!f) {
    Serial.println("No se pudo abrir CSV");
    return;
  }
  f.printf("%lu,%.2f,%.2f,%.2f\n", millis(), tempDHT, humedad, presion);
  f.close();
  totalRegistros++;
  Serial.printf("[SD] Registro #%ld guardado\n", totalRegistros);
}

// ========================================================
void contarRegistros() {
  File f = SD.open(nombreArchivo, FILE_READ);
  if (!f) return;
  totalRegistros = -1; // descontar cabecera
  while (f.available()) {
    if (f.read() == '\n') totalRegistros++;
  }
  f.close();
  if (totalRegistros < 0) totalRegistros = 0;
}

// ========================================================
void mostrarMonitor() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("== MONITOR ==");

  display.setCursor(0, 14);
  display.print("T: "); display.print(tempDHT, 1); display.println(" C");

  display.setCursor(0, 26);
  display.print("H: "); display.print(humedad, 1); display.println(" %");

  display.setCursor(0, 38);
  display.print("P: "); display.print(presion, 1); display.println(" hPa");

  display.setCursor(0, 52);
  display.print("Reg: "); display.print(totalRegistros);
  display.display();
}

// ========================================================
void mostrarAlerta() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("!! ALERTA !!");

  if (tempDHT > 35.0) {
    display.setCursor(0, 14);
    display.print("TEMP ALTA: ");
    display.print(tempDHT, 1);
    display.println(" C");
  }
  if (humedad > 85.0) {
    display.setCursor(0, 26);
    display.print("HUM ALTA: ");
    display.print(humedad, 1);
    display.println(" %");
  }
  display.setCursor(0, 42);
  display.print("P: "); display.print(presion, 1); display.println(" hPa");
  display.display();
}

// ========================================================
void mostrarLog() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("== LOG SD ==");

  display.setCursor(0, 20);
  display.print("Registros:");

  display.setTextSize(2);
  display.setCursor(0, 36);
  display.print(totalRegistros);
  display.display();
}

// ========================================================
void manejarLEDs() {
  unsigned long ahora = millis();
  apagarLEDs();

  switch (estadoActual) {
    case ESTADO_MONITOR:
      digitalWrite(LED_VERDE, HIGH);
      break;

    case ESTADO_ALERTA:
      // Parpadeo rojo
      if (ahora - tiempoParpadeo >= intervaloParpadeo) {
        tiempoParpadeo = ahora;
        ledRojoEstado = !ledRojoEstado;
      }
      digitalWrite(LED_ROJO, ledRojoEstado ? HIGH : LOW);
      break;

    case ESTADO_LOG:
      digitalWrite(LED_AZUL, HIGH);
      break;
  }
}

// ========================================================
void manejarBuzzer() {
  if (estadoActual == ESTADO_ALERTA) {
    // Tono intermitente
    unsigned long ahora = millis();
    static unsigned long tBuzzer = 0;
    static bool buzzerOn = false;

    if (ahora - tBuzzer >= 500) {
      tBuzzer = ahora;
      buzzerOn = !buzzerOn;
      if (buzzerOn) tone(BUZZER_PIN, 1000);
      else           noTone(BUZZER_PIN);
    }
  } else {
    noTone(BUZZER_PIN);
  }
}

// ========================================================
void manejarBoton() {
  bool btnActual = (digitalRead(BTN_PIN) == LOW); // activo en LOW

  if (btnActual && !btnPresionado) {
    // Flanco de bajada: empieza a contar
    btnPresionado = true;
    enLongPress   = false;
    tiempoBoton   = millis();
  }

  if (btnActual && btnPresionado) {
    // Mantenido: verificar long press
    if (!enLongPress && (millis() - tiempoBoton >= tiempoLongPress)) {
      enLongPress   = true;
      estadoActual  = ESTADO_LOG;
    }
  }

  if (!btnActual && btnPresionado) {
    // Flanco de subida: soltar botón
    btnPresionado = false;
    if (enLongPress) {
      // Volver a MONITOR (o ALERTA si condiciones se mantienen)
      estadoActual = ESTADO_MONITOR;
      enLongPress  = false;
    }
  }
}

// ========================================================
void apagarLEDs() {
  digitalWrite(LED_VERDE, LOW);
  digitalWrite(LED_ROJO,  LOW);
  digitalWrite(LED_AZUL,  LOW);
}