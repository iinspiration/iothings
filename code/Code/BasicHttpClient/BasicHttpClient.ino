/**
   BasicHTTPClient.ino

    Created on: 24.05.2015

*/

#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <ESP8266HTTPClient.h>

#include <WiFiClient.h>

ESP8266WiFiMulti WiFiMulti;

const int intervalTime = 5000;
const String url = "http://b301-119-76-30-101.ngrok.io/api/iot/get";

void setup() {

  Serial.begin(115200);
  // Serial.setDebugOutput(true);

  Serial.println();
  Serial.println();
  Serial.println();

  for (uint8_t t = 4; t > 0; t--) {
    Serial.printf("[SETUP] WAIT %d...\n", t);
    Serial.flush();
    delay(1000);
  }

  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("photocanvas.in.th_2G", "pichaya7831");

}

String getTime(){
      // Set time via NTP, as required for x.509 validation
      configTime(3 * 3600, 0, "pool.ntp.org", "time.nist.gov");
    
//      Serial.print("Waiting for NTP time sync: ");
      time_t now = time(nullptr);
      while (now < 8 * 3600 * 2) {
        delay(500);
//        Serial.print(".");
        now = time(nullptr);
      }
//      Serial.println("");
      struct tm timeinfo;
      gmtime_r(&now, &timeinfo);
//      Serial.print("Current time: ");
//      Serial.print(asctime(&timeinfo));
      return(asctime(&timeinfo));

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

void loop() {
  // wait for WiFi connection
  if ((WiFiMulti.run() == WL_CONNECTED)) {

    const String currentTime =getTime();
    const String response = getDataFromServer(url);
    Serial.println("["+ currentTime +"] Response From API : " + response);
    
  }

  delay(intervalTime);
}
