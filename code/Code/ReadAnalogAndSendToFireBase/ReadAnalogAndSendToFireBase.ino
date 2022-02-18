//
// Copyright 2015 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

// FirebaseDemo_ESP8266 is a sample that demo the different functions
// of the FirebaseArduino API.

#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <Servo.h>
Servo myservo;
// Set these to run example.
#define DEVICENAME "NodeMCUV2_01"
#define FIREBASE_HOST "nodemcuthings.firebaseio.com"
#define FIREBASE_AUTH "kSavdvK0yfm8dcsRtbc3iP4Mx3zP3nM6cmGGIcAG"
#define WIFI_SSID "photocanvas.in.th_2G"
#define WIFI_PASSWORD "pichaya7831"



const int analogInPin = A0;
int sensorValue = 0;
int readValue = 0;
int estDistance = 0;
void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  Firebase.setString("Devicename", DEVICENAME);
  Firebase.setString("ConnectedOn", WIFI_SSID);
  
  myservo.attach(2); //D4
  myservo.write(0);
}

int n = 0;
int dB = 0;
void loop() {
  
  readValue = analogRead(analogInPin);
  sensorValue = map(readValue, 0, 1023, 0, 180);
  dB = WiFi.RSSI();
  estDistance = map(dB, -50, -25, 200, 0);

  myservo.write(sensorValue);
  
  
  Firebase.setInt("ConnectionStrength",dB);
  Firebase.setInt("analogval", sensorValue);

  Serial.print("set: analogval [");
  Serial.print(sensorValue);
  Serial.print("] \t ConnectionStrength [");
  Serial.print(dB);
  Serial.print("] \t estDistance [");
  Serial.print(estDistance);
  Serial.print("] \n");
  digitalWrite(LED_BUILTIN, LOW);
  //delay(sensorValue);
  digitalWrite(LED_BUILTIN, HIGH); 
}
