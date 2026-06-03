#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

//1 Konfiguracja połączenia z tą samą siecią WiFi co serwer MAIN
const char* ssid = "JanRouterIIIŁączycielSieci";
const char* password = "Innominepatris";

ESP8266WebServer server(80);

//2 Adresacja sieciowa:
// vip - adres głównego serwera przejmowany po awarii MAIN
// backup_ip - własny adres serwera BACKUP w trybie czuwania
IPAddress vip(172,16,54,100);
IPAddress backup_ip(172,16,54,101);
IPAddress gateway(172,16,54,233);
IPAddress subnet(255,255,255,0);

bool isMaster = false;

unsigned long lastSeen = 0;
const unsigned long TIMEOUT = 5000;

void handleRoot()
{
  server.send(200, "text/html",
              isMaster ? "<h1>BACKUP (MASTER)</h1>"
                       : "<h1>BACKUP (STANDBY)</h1>");
}
//3 Mechanizm komunikacji między usługami.
// BACKUP wysyła zapytanie HTTP do MAIN i sprawdza
// odpowiedź z endpointu /heartbeat.
bool checkMain()
{
  WiFiClient client;

  if (client.connect(vip, 80))
  {
    client.print(
      "GET /heartbeat HTTP/1.1\r\n"
      "Connection: close\r\n\r\n"
    );

    delay(50);

    String r = client.readString();
    return r.indexOf("ALIVE") != -1;
  }

  return false;
}

//4 Mechanizm failover.
// Po wykryciu awarii MAIN serwer BACKUP przejmuje
// adres IP głównej usługi i staje się aktywnym serwer
void takeOver()
{
  Serial.println("TAKEOVER MAIN IP");

  WiFi.softAPdisconnect(true);

  WiFi.config(vip, gateway, subnet);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(200);
  }

  server.begin();
  isMaster = true;
}

//5 Mechanizm powrotu do trybu czuwania.
// Gdy MAIN ponownie stanie się dostępny,
// BACKUP oddaje rolę głównego serwera.
void backToStandby()
{
  Serial.println("BACK TO STANDBY");

  WiFi.config(backup_ip, gateway, subnet);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(200);
  }

  isMaster = false;
}

void setup()
{
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);
  WiFi.config(backup_ip, gateway, subnet);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
  }

  server.on("/", handleRoot);
  server.begin();

  Serial.println("BACKUP READY");
}

void loop()
{
  server.handleClient();

  if (checkMain())
  {
    lastSeen = millis();

    if (isMaster)
      backToStandby();
  }
  else
  {
    if (millis() - lastSeen > TIMEOUT)
    {
      if (!isMaster)
        takeOver();
    }
  }
}