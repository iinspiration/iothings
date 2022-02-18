
//USE DOIT ESP32 DEV Kit V1 Board
#define BLYNK_TEMPLATE_ID "TMPL2OaGAXTP"
#define BLYNK_DEVICE_NAME "NodeMCU01"
#define BLYNK_AUTH_TOKEN "XfoG-PdCl5fpXjGdm-7zXIdh3BYgt0DI"

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
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

#define MOI A0

int buttonState = 0;
int tempValue = 0;

const char* ssid = "photocanvas.in.th_2G";
const char* password = "pichaya7831";

const int intervalTime = 15 * 1000;

const String deviceName = "NodeMCU01";
const String getUrl = "http://ec2-54-251-164-224.ap-southeast-1.compute.amazonaws.com:3000/api/iot/get";
const String postUrl = "http://ec2-54-251-164-224.ap-southeast-1.compute.amazonaws.com:3000/api/iot/post";


unsigned long lastTime = 0;
BlynkTimer timer;

char auth[] = BLYNK_AUTH_TOKEN;

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
  Serial.print("WiFi RSSI: ");
  Serial.println(WiFi.RSSI());
 
  digitalWrite(LED,HIGH);
  
  Blynk.begin(auth, ssid, password);
  Blynk.virtualWrite(V1, 55);
}

void loop() {
  //Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > intervalTime) {
    Blynk.virtualWrite(V0, millis());
    digitalWrite(LED,HIGH);
    //Check WiFi connection status
    if(WiFi.status()== WL_CONNECTED){

      
      const String response = postDataToServer(postUrl,deviceName);
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
