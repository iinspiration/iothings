

//USE DOIT ESP32 DEV Kit V1 Board
#define BLYNK_TEMPLATE_ID "TMPL2OaGAXTP"
#define BLYNK_DEVICE_NAME "NodeMCU01"
#define BLYNK_AUTH_TOKEN "XfoG-PdCl5fpXjGdm-7zXIdh3BYgt0DI"
#define BLYNK_PRINT Serial

#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <DHT_U.h>


#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>


#define LED 2
#define PUMP 23
#define BTN 15

#define DHTPIN 4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

#define MOISTPIN 34

#define LIGHTPIN 5

#define LED 2


int tempValue = 0;

const char* ssid = "photocanvas.in.th_2G";
const char* password = "pichaya7831";

const int intervalTime = 15 * 1000;

const String deviceName = "NodeMCU01";
const String getUrl = "http://ec2-54-251-164-224.ap-southeast-1.compute.amazonaws.com:3000/api/iot/get";
const String postUrl = "http://ec2-54-251-164-224.ap-southeast-1.compute.amazonaws.com:3000/api/iot/post";


unsigned long lastTime = 0;


char auth[] = BLYNK_AUTH_TOKEN;

BlynkTimer timer;

float  moiPercentage = 50.0;
int pMoist = 0;
const int dry = 4095;
const int wet = 0;

// This function is called every time the Virtual Pin 0 state changes
//BLYNK_WRITE(V11)
//{
//  // Set incoming value from pin V0 to a variable
//  int value = param.asInt();
//
//  // Update state
////  Blynk.virtualWrite(V1, value);
//  digitalWrite(LED,value);
//  digitalWrite(13,value);
//  Serial.println(value);
//}


void dhtSensor()
{

//  String timeStamp = getTimeStamp();
  float humidity = dht.readHumidity();
  float temp = dht.readTemperature();
//
//  Blynk.virtualWrite(V0, temp);
//  Blynk.virtualWrite(V1, humidity);
//  Blynk.virtualWrite(V2, millis() / 1000);

  Blynk.virtualWrite(V3, temp);
  Blynk.virtualWrite(V4, humidity);
  Blynk.virtualWrite(V5, millis() / 1000);

//  Blynk.virtualWrite(V6, temp);
//  Blynk.virtualWrite(V7, humidity);
//  Blynk.virtualWrite(V8, millis() / 1000);

  
}

void moistSensor()
{
  analogReadResolution(12);
  int moiture = 0;
  moiture = analogRead(MOISTPIN);
  pMoist = map(moiture,wet,dry,100,00);
  if(pMoist > 0){
//    Blynk.virtualWrite(V9, pMoist);
    Blynk.virtualWrite(V10, pMoist);
   }
  
  
}


void setup() {
  Serial.begin(115200); 
  pinMode(LED,OUTPUT);
  pinMode(13,OUTPUT);
  Blynk.begin(auth, ssid, password);
  Blynk.notify("NodeMCU03 online");
  timer.setInterval(1000, dhtSensor);
  timer.setInterval(1200, moistSensor);
}

void loop() {
  Blynk.run();
  timer.run();
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

String postDataToServer(String url,String deviceName){
  
    WiFiClient client;

    HTTPClient http;

    int wifi_rssi = WiFi.RSSI();
    String timeStamp = getTimeStamp();
    float humidity = dht.readHumidity();
    float temp = dht.readTemperature();

//    int moisturePercentage = ( 100 - ( (analogRead(MOI) / 1023.00) * 100 ) );
    
    Serial.printf("[Humidity] read Humidity %f\n", humidity);
    Serial.printf("[Temp] read Temp %f\n", temp);
//    Serial.printf("[Moisture] read moisturePercentage %d\n", moisturePercentage);
    
    if (http.begin(client, url)) {  // HTTP


      Serial.printf("[HTTP] POST...%s\n",url.c_str());
      
      http.addHeader("Content-Type", "application/json");

      String httpRequestData = "{\"data\": { \"device\":\""+ deviceName +"\",\"wifi_rssi\":\""+ deviceName +"\",\"temp\":\""+ String(temp) +"\",\"humidity\":\""+ String(humidity) +"\",\"timeStamp\":\""+ timeStamp +"\"}}";
      Serial.println(httpRequestData);
      int httpCode = http.POST(httpRequestData);

      // httpCode will be negative on error
      if (httpCode > 0) {
        // HTTP header has been send and Server response header has been handled
        Serial.printf("[HTTP] POST... code: %d\n", httpCode);

        // file found at server
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

String getDataFromServer(String url,String deviceName){
  
    WiFiClient client;

    HTTPClient http;

    float humidity = dht.readHumidity();
    float temp = dht.readTemperature();

//    int moisturePercentage = ( 100 - ( (analogRead(MOI) / 1023.00) * 100 ) );
    
    Serial.printf("[Humidity] read Humidity %f\n", humidity);
    Serial.printf("[Temp] read Temp %f\n", temp);
//    Serial.printf("[Moisture] read moisturePercentage %d\n", moisturePercentage);
    
    Serial.print("[HTTP] begin..\n");
    if (http.begin(client, url + "?device="+ deviceName + "&humidity="+ humidity + "&temp="+ temp)) {  // HTTP


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
