#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>

const char *ssid = "ESP_AP";
const char *password = "12345678";

WebServer szero(80);

void kapcs() {
    digitalWrite(2, HIGH);
    Serial.println("LED bekapcsolva");
    szero.send(200, "text/plain", "LED BE");
}

void kikapcs() {
    digitalWrite(2, LOW);
    Serial.println("LED kikapcsolva");
    szero.send(200, "text/plain", "LED KI");
}

void testkapcs() {
    digitalWrite(2, HIGH);
    Serial.println("TEST GOMB megnyomva");
    szero.send(200, "text/plain", "TEST");
}

void inputszoveg() {
    if (szero.hasArg("adat")) {
        String bejovo = szero.arg("adat");
        Serial.print("Beérkezett szöveg: ");
        Serial.println(bejovo);
        szero.send(200, "text/plain", "Szöveg beolvasva");

        if (bejovo == "kapcs") {
            digitalWrite(2, HIGH);
            Serial.println("LED bekapcsolt a szöveg alapján");
        }
    } else {
        szero.send(400, "text/plain", "Nincs adat");
    }
}

void wifilist() {
    String html = "<h2>Elérhető WiFi hálózatok:</h2>";
    int n = WiFi.scanNetworks();
    if (n == 0) {
        html += "<p>Nincs elérhető hálózat</p>";
    } else {
        for (int i = 0; i < n; i++) {
            html += "<p>" + WiFi.SSID(i) + " (" + String(WiFi.RSSI(i)) + " dBm)</p>";
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
            function ledkapcs() {
                fetch("/bekapcs")
                    .then(res => res.text())
                    .then(txt => {
                        document.getElementById("ledstatus").innerText = txt;
                    });
            }

            function ledkikapcs() {
                fetch("/kikapcs")
                    .then(res => res.text())
                    .then(txt => {
                        document.getElementById("ledstatus").innerText = txt;
                    });
            }

            function test() {
                fetch("/test");
            }

            function inputszoveg() {
                let adat = document.getElementById("szoveginput").value;
                fetch("/inputszoveg?adat=" + encodeURIComponent(adat));
            }

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

        <button onclick="ledkapcs()">LED BE</button>
        <button onclick="ledkikapcs()">LED KI</button>
        <button onclick="test()">TEST</button>
        <p><strong>LED állapot:</strong> <span id="ledstatus">Ismeretlen</span></p>

        <h2>Szöveg küldése</h2>
        <input type="text" id="szoveginput">
        <button onclick="inputszoveg()">Küldés</button>

        <h2>Elérhető WiFi hálózatok</h2>
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
    szero.on("/bekapcs", kapcs);
    szero.on("/kikapcs", kikapcs);
    szero.on("/test", testkapcs);
    szero.on("/inputszoveg", inputszoveg);
    szero.on("/wifilist", wifilist);

    szero.begin();
    Serial.println("Webserver elindult.");
}

void loop() {
    szero.handleClient();
}
