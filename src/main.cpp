#include <Arduino.h>
#include <WiFi.h>

const char *ssid = "ESP32-Hotspot";
const char *password = "12345678";

void setup() {
  Serial.begin(115200);

  // Access Point indítása
  WiFi.softAP(ssid, password);

  Serial.println("Access Point létrehozva!");
  Serial.print("IP címe: ");
  Serial.println(WiFi.softAPIP());
}

void loop() {
  // ide jöhet később pl. web server, eszközlista, stb.
}
