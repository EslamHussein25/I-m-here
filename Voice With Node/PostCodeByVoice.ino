#define TCP_MSS whatever
#define LWIP_IPV6 whatever
#define LWIP_FEATURES whatever
#define LWIP_OPEN_SRC whatever

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "VoiceRecognitionV3.h"
int RX = D2 ;
int TX = D3 ;
VR myVR(TX,RX);    // 2:RX 3:TX, you can choose your favourite pins.
uint8_t records[7]; // save record
uint8_t buf[64];
int ret_1;
#define onRecord    (0)
#define offRecord   (1) 

#define STASSID "Eslam"
#define STAPSK  "cat#01119897972 "

int fristcheck = 0 ; 
String string;
int httpcode;
HTTPClient http ;
WiFiClient client;

void setup()
{
  Serial.begin(115200);
    myVR.begin(9600);

      if(myVR.clear() == 0)
  {
    Serial.println("Recognizer cleared.");
  }
  else
  {
    Serial.println("Not find VoiceRecognitionModule.");
    Serial.println("Please check connection and restart Arduino.");
    while(1);
  }
  
  if(myVR.load((uint8_t)onRecord) >= 0)
  {
    Serial.println("onRecord loaded");
  }
  
  if(myVR.load((uint8_t)offRecord) >= 0)
  {
    Serial.println("offRecord loaded");
  }

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


// Our Site 
 http.begin("http://ardu-api.herokuapp.com/message/");
 http.addHeader("Content-Type","application/json");

 if (fristcheck == 0)
 {
  
        string = "{\"name\":\"First Name\",\"message\":\"First Message\"}";
        //String data = "{"name":"Mohamed","message":"Hello world"}";
       //http.addHeader("Authorization","basic bW9oYW1lZDptb2hhbWVk") ;
      
         httpcode  = http.POST(string);
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
        delay(5000);
        fristcheck++;
 }

  ret_1 = myVR.recognize(buf, 50);
  
  if(ret_1>0)
  {
    switch(buf[1])
    {
        case 0:
          string = "{\"name\":\"Drink\",\"message\":\"0\"}";
          httpcode  = http.POST(string);
          Serial.println(httpcode);
          http.end();
          delay(10000);
        break;
        
        case 1:
          string = "{\"name\":\"Eat\",\"message\":\"1\"}";
          httpcode  = http.POST(string);
          Serial.println(httpcode);
          http.end();
          delay(10000);
        break;
        
        case 2 : 
          string = "{\"name\":\"Pain\",\"message\":\"2\"}";
          httpcode  = http.POST(string);
          Serial.println(httpcode);
          http.end();
          delay(10000);         
        break;
    }
 printVR(buf);
}
}





void printVR(uint8_t *buf)
{
  Serial.println("VR Index\tGroup\tRecordNum\tSignature");

  Serial.print(buf[2], DEC);
  Serial.print("\t\t");

  if(buf[0] == 0xFF)
  {
    Serial.print("NONE");
  }
  else if(buf[0]&0x80)
  {
    Serial.print("UG ");
    Serial.print(buf[0]&(~0x80), DEC);
  }
  else
  {
    Serial.print("SG ");
    Serial.print(buf[0], DEC);
  }
  Serial.print("\t");

  Serial.print(buf[1], DEC);
  Serial.print("\t\t");
  
  if(buf[3]>0)
  {
    printSignature(buf+4, buf[3]);
  }
  else
  {
    Serial.print("NONE");
  }
  Serial.println("\r\n");
}




void printSignature(uint8_t *buf, int len)
{
  int i;
  for(i=0; i<len; i++){
    if(buf[i]>0x19 && buf[i]<0x7F){
      Serial.write(buf[i]);
    }
    else{
      Serial.print("[");
      Serial.print(buf[i], HEX);
      Serial.print("]");
    }
  }
}
