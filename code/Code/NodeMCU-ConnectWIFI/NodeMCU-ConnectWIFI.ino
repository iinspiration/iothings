/*
  Rui Santos
  Complete project details at Complete project details at https://RandomNerdTutorials.com/esp32-http-get-post-arduino/

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files.

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
*/

#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

#define LED 2
#define PUMP 23
#define BTN 15
int buttonState = 0;

const char* ssid = "photocanvas.in.th_2G";
const char* password = "pichaya7831";

const int intervalTime = 60000;

const String deviceName = "NodeMCU01";
const String getUrl = "http://b301-119-76-30-101.ngrok.io/api/iot/get";
const String postUrl = "http://b301-119-76-30-101.ngrok.io/api/iot/post";


unsigned long lastTime = 0;
unsigned long timerDelay = 1000;

void setup() {
  Serial.begin(115200); 
  pinMode(LED,OUTPUT);
  pinMode(PUMP,OUTPUT);
  pinMode(BTN, INPUT);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
 
  digitalWrite(LED,HIGH);
}

void loop() {
  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay) {
    
    digitalWrite(LED,HIGH);
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){

      
      const String response = getDataFromServer(getUrl,deviceName);
      JSONVar jsonObject = JSON.parse(response);
      JSONVar keys = jsonObject.keys();
  
      Serial.println("Response From API : " + response);
  
      for (int i = 0; i < keys.length(); i++) {
        JSONVar value = jsonObject[keys[i]];
        Serial.print(keys[i]);
        Serial.print(" : ");
        Serial.println(value);
      }

      
    }
    else {
      Serial.println("WiFi Disconnected");
    }

    
    lastTime = millis();
  }
  
  digitalWrite(LED,LOW);
}

String getDataFromServer(String url,String deviceName){
  
    WiFiClient client;

    HTTPClient http;
    
    Serial.print("[HTTP] begin..\n");
    if (http.begin(client, url + "?device="+ deviceName + "&btnState=" + buttonState)) {  // HTTP


      Serial.printf("[HTTP] GET...%s\n",url.c_str());
      // start connection and send HTTP header
      int httpCode = http.GET();

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] GET... code: %d\n", httpCode);

        // file found at server
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
//          Serial.println(payload);
          return(payload);
        }
      } else { 
        Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  return "none";
}
