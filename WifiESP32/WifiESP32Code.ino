// Librerías necesarias
#include <WiFi.h>
#include <WebServer.h>

// Definir credenciales del Access Point
const char* ssid = "Art_Byte";
const char* password = "JuanOrjuela";

// Definir credenciales de autenticación
const char* authUser = "admin";
const char* authPass = "1234";

// Definir pines de los LEDs y PWM
const int led1 = 2;
const int led2 = 4;
const int pwmPin = 5;

// Crear el servidor web en el puerto 80
WebServer server(80);

// Página HTML optimizada para móvil con botones grandes
const char htmlPage[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <title>ESP32 Web Server</title>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <style>
        body { font-family: Arial; text-align: center; padding: 20px; background: #f0f0f0; }
        h2 { color: #333; }
        .btn { display: inline-block; padding: 15px 25px; margin: 10px; font-size: 20px;
               text-decoration: none; color: white; background: #007BFF; border-radius: 8px; }
        .btn.off { background: #dc3545; }
        .slider { width: 80%; margin: 20px auto; }
    </style>
</head>
<body>
    <h2>Control de LEDs</h2>
    <a class="btn" href="/led1/on">Encender LED 1</a>
    <a class="btn off" href="/led1/off">Apagar LED 1</a><br>
    <a class="btn" href="/led2/on">Encender LED 2</a>
    <a class="btn off" href="/led2/off">Apagar LED 2</a>

    <h2>Brillo PWM</h2>
    <form action="/pwm" method="GET">
        <input class="slider" type="range" name="value" min="0" max="255" onchange="this.form.submit()">
    </form>
</body>
</html>
)rawliteral";

void handleRoot() {
    if (!server.authenticate(authUser, authPass)) {
        return server.requestAuthentication();
    }
    server.send(200, "text/html", htmlPage);
}

void handleLED1On() {
    digitalWrite(led1, HIGH);
    Serial.println("LED 1 Encendido");
    server.send(200, "text/plain", "LED 1 Encendido");
}

void handleLED1Off() {
    digitalWrite(led1, LOW);
    Serial.println("LED 1 Apagado");
    server.send(200, "text/plain", "LED 1 Apagado");
}

void handleLED2On() {
    digitalWrite(led2, HIGH);
    Serial.println("LED 2 Encendido");
    server.send(200, "text/plain", "LED 2 Encendido");
}

void handleLED2Off() {
    digitalWrite(led2, LOW);
    Serial.println("LED 2 Apagado");
    server.send(200, "text/plain", "LED 2 Apagado");
}

void handlePWM() {
    if (!server.authenticate(authUser, authPass)) {
        return server.requestAuthentication();
    }
    if (server.hasArg("value")) {
        int pwmValue = server.arg("value").toInt();
        analogWrite(pwmPin, pwmValue); // Reemplazo de ledcWrite por analogWrite
        Serial.println("PWM Ajustado: " + String(pwmValue));
        server.send(200, "text/plain", "PWM Ajustado: " + String(pwmValue));
    } else {
        server.send(400, "text/plain", "Falta el parámetro de valor");
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(led1, OUTPUT);
    pinMode(led2, OUTPUT);
    pinMode(pwmPin, OUTPUT);

    // Configurar el ESP32 como AP
    WiFi.softAP(ssid, password);
    Serial.println("Punto de acceso iniciado");
    Serial.print("Dirección IP: ");
    Serial.println(WiFi.softAPIP());

    // Definir rutas del servidor
    server.on("/", handleRoot);
    server.on("/led1/on", handleLED1On);
    server.on("/led1/off", handleLED1Off);
    server.on("/led2/on", handleLED2On);
    server.on("/led2/off", handleLED2Off);
    server.on("/pwm", handlePWM);

    // Iniciar servidor
    server.begin();
    Serial.println("Servidor web iniciado");
}

void loop() {
    server.handleClient();
}