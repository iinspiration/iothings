#include <debug.h>
#include <MicroGear.h>
#include <AuthClient.h>
#include <MQTTClient.h>
#include <SHA1.h>
#include <PubSubClient.h>


#include <WiFi.h>
#include "DHT.h"

const char* ssid     = "photocanvas.in.th_2G";
const char* password = "pichaya7831";

#define APPID   "bee75429-957f-4f06-bf7e-e3a2d173823b"
#define KEY     "tmtJJnLKr18YtA1eDeNqxyx6i86vJ7oV"
#define SECRET  "StlXQ!IMUhPfl*hFj5Laa82psVJOgRf!"

#define ALIAS   "NodeMCU03"
#define TargetWeb "HTML_web"

#define D4 2   // TXD1
#define DHTPIN D4     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22

DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;
MicroGear microgear(client);

void onMsghandler(char *topic, uint8_t* msg, unsigned int msglen) 
{
    Serial.print("Incoming message --> ");
    msg[msglen] = '\0';
    Serial.println((char *)msg);
}


void onConnected(char *attribute, uint8_t* msg, unsigned int msglen) 
{
    Serial.println("Connected to NETPIE...");
    microgear.setAlias(ALIAS);
}

void setup() 
{
     /* Event listener */
    microgear.on(MESSAGE,onMsghandler);
    microgear.on(CONNECTED,onConnected);

    dht.begin();
    Serial.begin(115200);
    Serial.println("Starting...");

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) 
    {
       delay(250);
       Serial.print(".");
    }

    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    microgear.init(KEY,SECRET,ALIAS);
    microgear.connect(APPID);
}

void loop() 
{
    if (microgear.connected())
    {
       microgear.loop();
       Serial.println("connected");

       float Humidity = dht.readHumidity();
       float Temp = dht.readTemperature();  // Read temperature as Celsius (the default)
       String data = "/" + String(Humidity) + "/" + String(Temp);
       char msg[128];
       data.toCharArray(msg,data.length());
       Serial.println(msg);    

       microgear.chat(TargetWeb , msg);
    }
   else 
   {
    Serial.println("connection lost, reconnect...");
    microgear.connect(APPID);
   }
    delay(1500);
}
