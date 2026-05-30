//File not Found
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

//Konfiguracja Wifi
const char* ssid = "autko2137";
const char* password = "autko2137";
// Serwer sieciowy
ESP8266WebServer server(80);
// Aktualny stan diody
bool ledState = false;
bool isServer1 = true;
//
String token = "ABC123";
//
bool checkToken() 
{
 return server.hasArg("token") && server.arg("token") == token;
}

void handleRoot() {
 const String postForms = "<html>\
 <head>\
 <title>TEST SERVER</title>\
 <style>\
 body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }\
 </style>\
 </head>\
 <body>\
 <h1>Sterowanie Diodą LED</h1>\
 <p>Wciśnij przycisk, aby zmienić stan diody LED.</p>\
 <button onclick=\"toggleLED()\">Zmień stan LED</button>\
 <script>\
 function toggleLED() {\
 fetch('/toggle')\
 .then(response => {\
 if (response.ok) {\
 alert('Zmieniono stan diody LED!');\
} else {\
 alert('Wystąpił błąd!');\
 }\
 });\
 }\
 </script>\
 </body>\
 </html>";
 server.send(200, "text/html", postForms);
}
void handleNotFound() {
 String message = "File Not Found\n\n";
 server.send(404, "text/plain", message);
}
//
void handleSetToken() {
 if (!server.hasArg("value")) {
 server.send(400, "text/plain", "Brak parametru value");
 return;
 }
 if(checkToken()) {
 token = server.arg("value");
 server.send(200, "text/plain", "Token ustawiony");
 }
}
void handleLED(){
    if(ledState == false)
    {
      digitalWrite(LED_BUILTIN, LOW);
      ledState = true;
    }
    else
    {
      digitalWrite(LED_BUILTIN, HIGH);
      ledState = false;
    }
}
//CHECK SERVER
void checkServer()
{
  if(isServer1 == true)
  {
    Serial.print("Jestem Serverem");
    server.begin();
     server.on("/", handleRoot);
 server.onNotFound(handleNotFound);
 Serial.println("Serwer wystartował");
 server.on("/setToken", handleSetToken);
 server.on("/toggle", handleLED);
 digitalWrite(LED_BUILTIN, LOW);
      ledState = true;
  }
  
  
}

void setup() {
 // Inicjalizacja WiFi
 WiFi.softAP(ssid, password);
 IPAddress IP = WiFi.softAPIP();
 //
 pinMode(LED_BUILTIN, OUTPUT);
 Serial.begin(9600); // Ustawiony baud rate
 Serial.println();
 Serial.print("Aplikacja dostępna pod adresem: ");
 Serial.println(IP);
 // Konfiguracja serwera
checkServer();
 //jeżeli server2 == false dopiero odpal server1
 
}
void loop() {
 server.handleClient();
}
//Przykład wywołania: http://192.168.4.1/status?token=ABC123
//Ustawienie tokenu: http://192.168.4.1/setToken?token=ABC123&value=NOWY