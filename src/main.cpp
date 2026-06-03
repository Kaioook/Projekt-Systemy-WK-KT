#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

//1
const char* ssid = "JanRouterIIIŁączycielSieci";
const char* password = "Innominepatris";

ESP8266WebServer server(80);

//2
IPAddress ip(172,16,54,100);
IPAddress gateway(172,16,54,233);
IPAddress subnet(255,255,255,0);

void handleRoot()
{
  server.send(200, "text/html", "<h1>MAIN SERVER</h1>");
}

//3
void handleHeartbeat()
{
  server.send(200, "text/plain", "ALIVE");
}

void setup()
{
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  WiFi.config(ip, gateway, subnet);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.println("MAIN connecting...");
  }

  server.on("/", handleRoot);
  server.on("/heartbeat", handleHeartbeat);
  server.begin();

  Serial.println("MAIN READY");
}

void loop()
{
  server.handleClient();
}