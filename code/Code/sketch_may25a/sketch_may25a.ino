#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h> 
Servo myservo;

// Set the LCD address to 0x27 in PCF8574 by NXP and Set to 0x3F in PCF8574A by Ti
LiquidCrystal_I2C lcd(0x27, 16, 2); // จอกว้าง 16 ตัวอักษร 2 บรรทัด รหัสประจำตัว 0x3F

float initialTemp;
float temp;
float value;
int tempPin = 0;
int swPin = 1;

int redPin = 5;
int bluePin = 6;

int jStickCenterPin = 2;
int jStickXPin = 3;
int jStickYPin = 4;

int trigger = 1;
int jStickXState=0;

void setup() {

  Serial.begin(9600);
  pinMode(redPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(jStickCenterPin, INPUT);  
  pinMode(jStickXPin, INPUT);  
  pinMode(jStickYPin, INPUT);  

  myservo.attach(9);
  //myservo.write(0); 
  
  intro();

  initialTemp = analogRead(tempPin);
  initialTemp = initialTemp * 0.48828125;
  
}

void loop() {

  jStickXState = digitalRead(jStickXPin);
  Serial.println(jStickXState);


  while(1){
     //readTemp(tempPin);
     readDegree(swPin);
    }


 lcd.setCursor(6, 0);
 lcd.print("HOME"); 
 lcd.setCursor(0, 1);                                        //กำหนดตำแหน่ง Cursor
 lcd.print("initTMP:"); 
 lcd.print(initialTemp); 
 lcd.print("*C"); 
  
}


void readTemp(int tempPin){
    temp = analogRead(tempPin);
    temp = temp * 0.48828125;
    lcd.setCursor(0, 0);
    lcd.print("RoomTemperature"); 
    lcd.setCursor(3, 1);
    lcd.print(temp); 
    lcd.print("*C"); 
    if(temp>initialTemp){
        digitalWrite(redPin,HIGH);
        digitalWrite(bluePin,LOW);
        lcd.print(" ++"); 
      }else if(temp<initialTemp){
        digitalWrite(bluePin,HIGH);
        digitalWrite(redPin,LOW);
        lcd.print(" --"); 
      } 
    delay(100);
    lcd.clear();
    
  }
void readDegree(int Pin){
    value = analogRead(Pin);
    value = map(value, 0, 1023, 0, 179);
    myservo.write(value);
    lcd.setCursor(0, 0);
    lcd.print("Servo Degree:"); 
    lcd.setCursor(3, 1);
    lcd.print(value);
    
    delay(10);
    lcd.clear();
  }
void intro(){
  lcd.begin(); 
  lcd.setCursor(5, 0);
  lcd.print("Ready?"); 
  delay(500);
  lcd.clear(); 
  
  lcd.setCursor(6, 1);                                        //กำหนดตำแหน่ง Cursor
  lcd.print("GO!!");  
  delay(500);
  lcd.clear();
}


