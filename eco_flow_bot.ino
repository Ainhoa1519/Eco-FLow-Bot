/* CREDENCIALES DE BLYNK  */
#define BLYNK_TEMPLATE_ID "TMPL2zY8z_qW3"
#define BLYNK_TEMPLATE_NAME "Eco Flow Bot"
#define BLYNK_AUTH_TOKEN "ZiVtXL3-4g55BsMnegpmWWdO0jYJPA92"

/* Librerías necesarias para ESP32 y Blynk */
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

/* WIFI 
char ssid[] = "";
char pass[] = "";
*/

int nivelBateria = 100;
const int pinLedFisico = 8;    // LED de Batería
const int pinMotorFisico = 5;  // LED de Retrolavado
bool estadoParpadeo = false;

BlynkTimer timer; 

void actualizarSensores() {
  // si hay batería ->  FUNCIONA TODO
  if (nivelBateria > 0) {
    //Simulación de Turbidez
    int turbidez = random(10, 25); // Valores normales de agua
    Blynk.virtualWrite(V0, turbidez);

    // Simulación de Caudal 
    // Genera un valor entre 20.0 y 25.0
    float caudal = 20.0 + (random(0, 500) / 100.0); 
    Blynk.virtualWrite(V1, caudal);

    nivelBateria--; // Baja la batería
  } 
  // si no hay batería -> SENSORES A CERO
  else {
    Blynk.virtualWrite(V0, 0);
    Blynk.virtualWrite(V1, 0);
    digitalWrite(pinMotorFisico, LOW); // seguridad -> si no hay luz, no hay motor
  }

  // Actualizar widget de batería
  Blynk.virtualWrite(V3, nivelBateria);

  // Lógica del LED de encendido (Pin 8)
  if (nivelBateria > 20) {
    digitalWrite(pinLedFisico, HIGH);
    Blynk.virtualWrite(V4, 255);
  } else if (nivelBateria <= 20 && nivelBateria > 0) {
    estadoParpadeo = !estadoParpadeo;
    digitalWrite(pinLedFisico, estadoParpadeo ? HIGH : LOW);
    Blynk.virtualWrite(V4, estadoParpadeo ? 255 : 0);
  } else {
    digitalWrite(pinLedFisico, LOW);
    Blynk.virtualWrite(V4, 0);
  }
}

// BOTÓN DE RETROLAVADO
BLYNK_WRITE(V2) {
  int valorBoton = param.asInt(); 
  if (nivelBateria > 0) { // Solo funciona si hay batería
    if (valorBoton == 1) {
      digitalWrite(pinMotorFisico, HIGH);
      Serial.println("Motor ON");
    } else {
      digitalWrite(pinMotorFisico, LOW);
      Serial.println("Motor OFF");
    }
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(pinLedFisico, OUTPUT);
  pinMode(pinMotorFisico, OUTPUT);
  
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  timer.setInterval(1000L, actualizarSensores);
}

void loop() {
  Blynk.run();
  timer.run();
}
