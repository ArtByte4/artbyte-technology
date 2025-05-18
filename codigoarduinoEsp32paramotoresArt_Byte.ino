// Proyecto: Art_Byte - Firmware para ESP32
// Control de dos motores DC vía interfaz web

#include <WiFi.h>
#include <WebServer.h>

// Pines de motor
#define IN1 14
#define IN2 27
#define ENA 26
#define IN3 25
#define IN4 33
#define ENB 32

// Sensores
#define LIMIT1 18
#define LIMIT2 19
#define ENCODER1 34
#define ENCODER2 35

// Wi-Fi
const char* ssid = "Art_Byte_AP";
const char* password = "Tattoo";

WebServer server(80);

int speed1 = 128;
int speed2 = 128;
int encoder1_val = 0;
int encoder2_val = 0;

void setup() {
  Serial.begin(115200);

  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  
  pinMode(LIMIT1, INPUT_PULLUP);
  pinMode(LIMIT2, INPUT_PULLUP);
  pinMode(ENCODER1, INPUT);
  pinMode(ENCODER2, INPUT);

  analogWrite(ENA, speed1);
  analogWrite(ENB, speed2);

  WiFi.softAP(ssid, password);
  Serial.println("WiFi AP iniciado");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  server.on("/", []() {
    server.send(200, "text/plain", "ESP32 listo para Art_Byte");
  });

  server.on("/m1/forward", []() {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    analogWrite(ENA, speed1);
    server.send(200);
  });

  server.on("/m1/backward", []() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    analogWrite(ENA, speed1);
    server.send(200);
  });

  server.on("/m2/forward", []() {
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    analogWrite(ENB, speed2);
    server.send(200);
  });

  server.on("/m2/backward", []() {
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    analogWrite(ENB, speed2);
    server.send(200);
  });

  server.on("/m1/speed", []() {
    if (server.hasArg("value")) {
      speed1 = map(server.arg("value").toInt(), 1, 100, 25, 255);
      analogWrite(ENA, speed1);
    }
    server.send(200);
  });

  server.on("/m2/speed", []() {
    if (server.hasArg("value")) {
      speed2 = map(server.arg("value").toInt(), 1, 100, 25, 255);
      analogWrite(ENB, speed2);
    }
    server.send(200);
  });

  server.on("/sensor/m1", []() {
    encoder1_val = analogRead(ENCODER1);
    bool limit1 = digitalRead(LIMIT1) == LOW;
    String json = "{";
    json += "\"encoder\":" + String(encoder1_val);
    json += ",\"limit\":\"" + String(limit1 ? "ACTIVADO" : "NO ACTIVADO") + "\"}";
    server.send(200, "application/json", json);
  });

  server.on("/sensor/m2", []() {
    encoder2_val = analogRead(ENCODER2);
    bool limit2 = digitalRead(LIMIT2) == LOW;
    String json = "{";
    json += "\"encoder\":" + String(encoder2_val);
    json += ",\"limit\":\"" + String(limit2 ? "ACTIVADO" : "NO ACTIVADO") + "\"}";
    server.send(200, "application/json", json);
  });

  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop() {
  server.handleClient();
}