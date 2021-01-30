
#define TCP_MSS whatever
#define LWIP_IPV6 whatever
#define LWIP_FEATURES whatever
#define LWIP_OPEN_SRC whatever

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

#define STASSID "mohamed"
#define STAPSK  "mohamed1998"

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

// Our Site 
 http.begin("http://ardu-api.herokuapp.com/message/");
 http.addHeader("Content-Type","application/json");
 String string = "{\"name\":\"First Name\",\"message\":\"First Message\"}";
//  String data = "{"name":"Mohamed","message":"Hello world"}";
 //http.addHeader("Authorization","basic bW9oYW1lZDptb2hhbWVk") ;

  int httpcode  = http.POST(string);
  Serial.println(httpcode);
  if(httpcode > 0)
  {
    String payload  = http.getString();
    Serial.println("the response is : ");
    Serial.println(payload);
  }
  else 
  {
    Serial.print("Connection fail");
  }
  http.end();
  delay(10000);

}