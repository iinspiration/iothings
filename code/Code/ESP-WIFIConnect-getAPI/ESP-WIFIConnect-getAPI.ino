#include <DHT.h>
#include <DHT_U.h>


#include <Arduino.h>
#include <string> 

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>
#include <Arduino_JSON.h>

ESP8266WiFiMulti WiFiMulti;


const int analogInPin = A0;
int sensorValue = 0; 

const int intervalTime = 60000;
const String getUrl = "http://b301-119-76-30-101.ngrok.io/api/iot/get";
const String postUrl = "http://b301-119-76-30-101.ngrok.io/api/iot/post";

void setup() {

  Serial.begin(115200);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("Initiate Connection %d...\n", t);
    Serial.flush();
    delay(5000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("photocanvas.in.th_2G", "pichaya7831");

}

void loop() {

  if ((WiFiMulti.run() == WL_CONNECTED)) {
   
//    const String response = getDataFromServer(getUrl);
    const String response = postDataToServer(postUrl);
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

  delay(intervalTime);
}


String getTime(){
      configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
      time_t now = time(nullptr);
      while (now < 8 * 3600 * 2) {
        delay(500);
        now = time(nullptr);
      }
      struct tm timeinfo;
      gmtime_r(&now, &timeinfo);
      return(asctime(&timeinfo));
}

String getTimeStamp(){
      configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
      time_t now = time(nullptr);
      while (now < 8 * 3600 * 2) {
        delay(500);
        now = time(nullptr);
      }
      struct tm timeinfo;
      gmtime_r(&now, &timeinfo);
      char buffer[35];
      return(itoa(now, buffer, 10));
}

String getKnobValue(){
    int sensorValue = analogRead(analogInPin);
    char buffer[35];
    return(itoa(sensorValue, buffer, 10)); 
}

String postDataToServer(String url){
  
    WiFiClient client;

    HTTPClient http;

    Serial.print("[HTTP] begin..\n");
    if (http.begin(client, url)) {  // HTTP


      Serial.printf("[HTTP] POST...%s\n",url.c_str());
      // start connection and send HTTP header
      http.addHeader("Content-Type", "application/json");
      String knob = getKnobValue();
      String temp = "99c";
      String humid = "44%";
      String timeStamp = getTimeStamp();
      String httpRequestData = "{\"data\": { \"knob\":\""+ knob +"\",\"temp\":\""+ temp +"\",\"humid\":\""+ humid +"\",\"timeStamp\":\""+ timeStamp +"\"}}";
      Serial.println(httpRequestData);
      int httpCode = http.POST(httpRequestData);

      if (httpCode > 0) {
        Serial.printf("[HTTP] POST... code: %d\n", httpCode);
        if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
          String payload = http.getString();
          return(payload);
        }
      } else { 
        Serial.printf("[HTTP] POST... failed, error: %s\n", http.errorToString(httpCode).c_str());
      }

      http.end();
    } else {
      Serial.printf("[HTTP} Unable to connect\n");
    }
  return "none";
}

String getDataFromServer(String url){
  
    WiFiClient client;

    HTTPClient http;

    Serial.print("[HTTP] begin..\n");
    if (http.begin(client, url)) {  // HTTP


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
