#include <SPI.h>
#include <SD.h>

const int PIN_CS_SD = 10;

void setup() {
  Serial.begin(9600);
  
  // Cambiamos ' ' por " "
  Serial.print("Inicializando SD...");
  
  if (!SD.begin(PIN_CS_SD)) {
    Serial.println(" ERROR. Verifica conexiones y formato FAT32.");
    return;
  }
  Serial.println(" OK");

  // Listar archivos
  File root = SD.open("/");
  Serial.println("Archivos en la SD:");
  
  while (true) {
    File entry = root.openNextFile();
    if (!entry) break;
    Serial.print(" ");
    Serial.print(entry.name());
    Serial.print(" ");
    Serial.println(entry.size());
    entry.close();
  }

  // Crear archivo
  File archivo = SD.open(" .TXT", FILE_WRITE);
  if (archivo) {
    archivo.println("Lab 08 - Prueba de escritura SD");
    archivo.println("Sistema embebido OK");
    archivo.println("EQUIPO: ");
    archivo.println("BADILLO NAVARRO EDGAR SANTIAGO ");
    archivo.println("SANCHEZ GARCIA JOSE ARTURO ");
    archivo.println("ZAMITTIZ CAMACHO IRVING FERNANDO");
    archivo.println("ZAPATA ECHVERRIA JUAN DE DIOS");
    archivo.close();
    Serial.println("Archivo EQUIPO.TXT creado correctamente.");
  }
}

void loop() {}