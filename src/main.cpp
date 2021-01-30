
#define TCP_MSS whatever
#define LWIP_IPV6 whatever
#define LWIP_FEATURES whatever
#define LWIP_OPEN_SRC whatever

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define STASSID "mohamed"
#define STAPSK  "mohamed1998"

const uint16_t port = 8090;
const char * host = "192.168.1.83";

void setup()
{
  Serial.begin(115200);

  WiFi.begin(STASSID, STAPSK);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() 
{
  HTTPClient http ;
  WiFiClient client;

 if (!client.connect(host, port)) {

        Serial.println("Connection to host failed");

        delay(1000);
        return;
    }

    Serial.println("Connected to server successful!");

    client.print("Hello from ESP32!");

    Serial.println("Disconnecting...");
    client.stop();

    delay(10000);
}