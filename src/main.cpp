#include <Arduino.h>
#include <WebServer.h>
#include <WiFi.h>

const char *ssid = "ESP_AP";
const char *password = "12345678";

WebServer szero(80);

// Weboldal functionok gombok szöveg mezők stb...........
//--------------------------------------------------
void kapcs() { // egy void
    digitalWrite(2, HIGH);
    Serial.println("bekapcsolt");
    szero.send(200, "text/plain", "Bekapcs");
}

void kikapcs() {
    digitalWrite(2, LOW);
    Serial.println("kikapcsolt");
    szero.send(200, "text/plain", "Kikapcs");
}

void testkapcs() {
    digitalWrite(2, HIGH);
    Serial.println("test");
    szero.send(200, "text/plain", "test");
}

void inputszoveg() {
    if (szero.hasArg("adat")) {
        String bejovo = szero.arg("adat"); // szöveg beolvasása
        Serial.print("Bejött szöveg:");
        Serial.println(bejovo);
        szero.send(200, "text/plain", "Szöveg beolvasva");
        if (bejovo == "kapcs") { // ha a szöveg bejött akkor kapcsolja be
            digitalWrite(2, HIGH);
            Serial.println("bekapcsolt");
        }
    } else {
        szero.send(400, "text/plain", "Nincs adat");
    }
}

//--------------------------------------------------

void handleRoot() {
    // szero.send(200, "text/html", "<h1>Üdv az ESP32-n!</h1><p>Itt lesz majd a
    // WiFi választó oldal</p>");

    //-----------------------------------------------------------
    // ebben kehet megadni a weboldal kódját !!!!
    // 1.gombok beolvasása ✅
    //  -Létre kell hozni egy voidot
    //  -Utána a a setup ban a szerverre küldjük  (server.on stb....)
    //  -Utoljára a html kódban össze kötjük
    // 2.szöveg beolvasás
    // 3. beérkező adatok elmentése és kiírása
    //-----------------------------------------------------------
    String html = R"rawliteral(       
 <!DOCTYPE html>
<html lang="hu">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP_AP</title>
    <script>
    
    function ledkapcs(){            //a html hez való hozzáadás
  fetch("/bekapcs");
    }

      function ledkikapcs(){
  fetch("/kikapcs");
    }

    function test(){
    fetch("/test");
}

    function inputszoveg(){ 
  let szoveg = document.getElementById("szoveg").value; //szöveg beolvasása;
  fetch("/inputszoveg?adat=" +encodeURIComponent(szoveg));  



    }

    </script>
</head>
<body>

<h1>AP Tanulása</h1>
<button onclick = "ledkapcs()">BE</button>
<button onclick = "ledkikapcs()">KI</button>
<button onclick = "test()">TEST GOMB</button>


    <h2>Írj be valamit</h2>
<input type="text" id="szoveg" placeholder="Írj be valamit...">
<button onclick="inputszoveg()">Küldés</button>

    
</body>
</html>

 
  
  
  )rawliteral";
    szero.send(200, "text/html", html);
    // szero.on("/led_kapcs",kapcs);
}

void setup() {
    Serial.begin(115200);
    pinMode(2, OUTPUT);

    // Access Point indítása
    WiFi.softAP(ssid, password);

    Serial.println("Access Point létrehozva!");
    Serial.print("IP címe: ");
    Serial.println(WiFi.softAPIP());
    szero.on("/", handleRoot);
    szero.on("/bekapcs", kapcs); // szerverre való küldés
    szero.on("/kikapcs", kikapcs);
    szero.on("/test", testkapcs);
    szero.on("/inputszoveg", inputszoveg); // szöveg beolvasás

    szero.begin();
    Serial.printf("a webserver elindult");
}

void loop() {
    szero.handleClient();
    // A loop függvényben hívjuk meg a szerver kezelőjét

    // ide jöhet később pl. web server, eszközlista, stb.
}
