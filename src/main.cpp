#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char *ssid = "ESP32-Hotspot";
const char *password = "12345678";

WebServer szero(80);

void handleRoot(){
  //szero.send(200, "text/html", "<h1>Üdv az ESP32-n!</h1><p>Itt lesz majd a WiFi választó oldal</p>");

  //-----------------------------------------------------------
  //ebben kehet megadni a weboldal kódját !!!!
  //1.gombok beolvasása
  //2.szöveg beolvasás
  //3. beérkező adatok elmentése és kiírása
  //-----------------------------------------------------------
  String html = R"rawliteral(       
  <h1>Szia!</h1>
  
  )rawliteral" ;
  szero.send(200,"text/html",html);

}

void setup() {
  Serial.begin(115200);

  // Access Point indítása
  WiFi.softAP(ssid, password);

  Serial.println("Access Point létrehozva!");
  Serial.print("IP címe: ");
  Serial.println(WiFi.softAPIP());
  szero.on("/",handleRoot);
  szero.begin();
  Serial.printf("a webserver elindult");
}

void loop() {
szero.handleClient();

  // ide jöhet később pl. web server, eszközlista, stb.
}
