#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>

const char *ssid = "ESP_AP";
const char *password = "12345678";

WebServer szero(80);

void bejelentkezes() {
  if (szero.hasArg("ssid")) {
    String ssid = szero.arg("ssid");

    String html = R"rawliteral(
        <!DOCTYPE html>
        <html lang="hu">
        <head>
            <meta charset="UTF-8">
            <meta name="viewport" content="width=device-width, initial-scale=1.0">
            <title>WiFi csatlakozás</title>
        </head>
        <body>
            <h2>Csatlakozás a következő hálózathoz: )rawliteral" +
                  ssid + R"rawliteral(</h2>
            <form action="/csatlakozas" method="POST">
                <input type="hidden" name="ssid" value=")rawliteral" +
                  ssid + R"rawliteral(">
                <label for="password">Jelszó:</label><br>
                <input type="password" id="password" name="password" required><br><br>
                <input type="submit" value="Csatlakozás">
            </form>
        </body>
        </html>
        )rawliteral";

    szero.send(200, "text/html", html);
  } else {
    szero.send(400, "text/plain", "Hiányzik az SSID paraméter.");
  }
}

void csatlakozas() {
  if (szero.method() == HTTP_POST) {
    String ssid = szero.arg("ssid");
    String password = szero.arg("password");

    szero.send(200, "text/html",
               "<h2>Kapcsolódás folyamatban...</h2><p>Kérlek várj!</p>");

    WiFi.softAPdisconnect(true);
    WiFi.disconnect();
    delay(1000);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());

    int timeout = 10000;
    while (WiFi.status() != WL_CONNECTED && timeout > 0) {
      delay(500);
      Serial.print(".");
      timeout -= 500;
    }

    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("\nSikeres csatlakozás");
      Serial.print("IP cím: ");
      Serial.println(WiFi.localIP());

    } else {
      Serial.println("\nNem sikerült csatlakozni");
      WiFi.mode(WIFI_AP_STA);
      WiFi.softAP("ESP_AP", "12345678");
    }
  } else {
    szero.send(405, "text/plain", "Csak POST engedélyezett.");
  }
}

void wifilist() {
  String html = "<h2>Elérhető WiFi hálózatok:</h2>";
  int n = WiFi.scanNetworks();
  if (n == 0) {
    html += "<p>Nincs elérhető hálózat</p>";
  } else {
    for (int i = 0; i < n; i++) {
      String ssid = WiFi.SSID(i);
      html += "<p><a href=\"/connect?ssid=" + ssid + "\">" + ssid + "</a> (" +
              String(WiFi.RSSI(i)) + ")</p>";
    }
  }
  szero.send(200, "text/html", html);
}

void handleRoot() {
  String html = R"rawliteral(
    <!DOCTYPE html>
    <html lang="hu">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>ESP32 AP Webserver</title>
        <script>
          
            function wifilist() {
                fetch("/wifilist")
                    .then(res => res.text())
                    .then(html => {
                        document.getElementById("wifilist").innerHTML = html;
                    });
            }
        </script>
    </head>
    <body>
        <h1>ESP32 Webvezérlés</h1>

       

       
        <div id="wifilist"><em>Kattints a frissítéshez</em></div>
        <button onclick="wifilist()">WiFi Lista Frissítése</button>


    </body>
    </html>
    )rawliteral";

  szero.send(200, "text/html", html);
}

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ssid, password);
  Serial.println("Access Point elindult: ESP_AP");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());

  szero.on("/", handleRoot);
  szero.on("/wifilist", wifilist);
  szero.on("/connect", bejelentkezes);
  szero.on("/csatlakozas", HTTP_POST, csatlakozas);

  szero.begin();
  Serial.println("Webserver elindult.");
}

void loop() { szero.handleClient(); }
