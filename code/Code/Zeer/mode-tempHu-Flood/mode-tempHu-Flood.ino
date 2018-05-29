#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

//===============================================================
// Global Variables & Constants
//===============================================================

LiquidCrystal_I2C lcd(0x27, 16, 2);

// Coresystem
const int modePin = 5; // Active HIGH, held low by 4.7K
 
int mode = 1; // Selector State (Initial state = ALL OFF)
int val = 0; // Pin 13 HIGH/LOW Status
int push = 0; // Pin 13 HIGH/LOW Status
int butState = 0; // Last Button State
int modeState = 0; // Last Mode State
boolean debug = 1; // 1 = Print Serial Enabled / 0 = disabled

// DHT
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

//flood
const int wlPin = 0;

//Alarm
const int buzzerPin =7;
const int greenPin =6;
const int redPin =5;


//===============================================================
// SETUP
//===============================================================
void setup () {
 Serial.begin(9600);
 pinMode(modePin, INPUT);
 pinMode(wlPin, INPUT);

 pinMode(buzzerPin, OUTPUT);
 pinMode(greenPin, OUTPUT);
 pinMode(redPin, OUTPUT);

 dht.begin();
 
 lcd.begin(); 

 intro();
}
 
//===============================================================
// Main Loop
//===============================================================
void loop() {
 
 push = analogRead(modePin);
 if(push < 500){ val = 0;}else{val = 1;}

  
 // If we see a change in button state, increment mode value
 if (val != butState && val == 0){
 mode++;
 Serial.print("Pushed");       // prints a label
 Serial.print("\n");
 lcd.clear(); 
 }

 
 
 butState = val; // Keep track of most recent button state
 

   switch ( mode ) {
    
   case 2:

   showState();
   
   lcd.setCursor(0, 0);
   lcd.print("TempAlarm"); 
   delay(1000);

    while(1){
      float hu = dht.readHumidity();
      float te = dht.readTemperature();
      lcd.clear(); 
      lcd.setCursor(0, 0);
      lcd.print("Humidity:");
      lcd.print(hu);
      lcd.print("%");
      lcd.setCursor(0, 1);
      lcd.print("Temp:");
      lcd.print(te);
      lcd.print("*C");
      if (isnan(te) || isnan(hu)) {
        Serial.println("Failed to read from DHT");
      } else {
        Serial.print("Humidity: "); 
        Serial.print(hu);
        Serial.print(" %\t");
        Serial.print("Temperature: "); 
        Serial.print(te);
        Serial.println(" *C");
      }

      if(te>50){
        digitalWrite(buzzerPin, HIGH); 
      }else{
        digitalWrite(buzzerPin, LOW);
        }
      delay(100);
      push = analogRead(modePin);
      if(push < 500){ Serial.print("push!! "); mode++;lcd.clear();  break;}
      }
    
   break;
   
   case 3:

   showState();

   lcd.setCursor(0, 0);
   lcd.print("FloodAlarm"); 
   delay(1000);delay(1000);

  while(1){

      float wl = analogRead(wlPin);
      if(wl>100){
        digitalWrite(buzzerPin, HIGH); 
      }else{
        digitalWrite(buzzerPin, LOW);
        }
      
  
      lcd.clear(); 
      lcd.setCursor(0, 0);
      lcd.print("WaterStatus: ");
      if(wl<100){lcd.print("N");}else{lcd.print("F");}
      lcd.setCursor(0, 1);
      lcd.print("Value:");
      lcd.print(wl);


        Serial.print("WaterLevel: "); 
        Serial.print(wl);
        Serial.print(" \n");

      delay(100);
      push = analogRead(modePin);
      if(push < 500){ Serial.print("push!! "); mode++;lcd.clear();  break;}
      }
  
   break;
   
   default:
   mode = 1;

   showState();
   lcd.setCursor(0, 0);
   lcd.print("SelectMode"); 
  
    
   break;
   
   } // end switch

 modeState = mode; // Keep track of mode recent mode value
 delay(100); // slow the loop just a bit for debounce
}
 
//===============================================================
// Subroutine
//===============================================================
void showState() {
 Serial.print("Mode: ");
 Serial.println(mode);
}


void intro(){
  lcd.begin(); 
  lcd.setCursor(2, 0);
  lcd.print("System Init!"); 
  delay(1000);
  lcd.clear(); 
  lcd.setCursor(5, 0);                   
  lcd.print("Sensor");  
  lcd.setCursor(3, 1);                   
  lcd.print("initiated!"); 
  delay(1000);
  lcd.clear();
}





