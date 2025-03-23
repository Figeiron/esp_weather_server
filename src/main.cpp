#include <Arduino.h>

#include <WiFi.h>
#include <WebServer.h>
#include <DHT.h>

#define DHT22_PIN 21
DHT dht22(DHT22_PIN, DHT22);

const char* ssid = "vasilev";
const char* password = "20111983";

WebServer server(80);

void handleRoot() {
    float humi = dht22.readHumidity();
    float tempC = dht22.readTemperature();
    float tempF = dht22.readTemperature(true);

    if (isnan(tempC) || isnan(tempF) || isnan(humi)) {
        server.send(500, "text/plain", "Failed to read from sensor!");
        return;
    }

    String html = "<html><head><meta http-equiv='refresh' content='5'>";
    html += "<title>WEATHER SERVER</title></head><body>";
    html += "<p><b>Humidity:</b> " + String(humi) + "%</p>";
    html += "<p><b>Temperature:</b> " + String(tempC) + "°C</p>";
    html += "</body></html>";

    server.send(200, "text/html", html);
}

void setup() {
    Serial.begin(9600);
    dht22.begin();

    WiFi.begin(ssid, password);
    Serial.print("Connecting to Wi-Fi...");
    
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.print(".");
    }
    Serial.println("\nConnected to Wi-Fi!");
    Serial.println(WiFi.localIP());

    server.on("/", handleRoot);
    server.begin();
}

void loop() {
    server.handleClient();
}
