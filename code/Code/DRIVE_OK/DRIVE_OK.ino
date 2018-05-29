#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Motor 1
int dir1PinA = 2;
int dir2PinA = 3;
int speedPinA = 9; // Needs to be a PWM pin to be able to control motor speed

// Motor 2
int dir1PinB = 4;
int dir2PinB = 5;
int speedPinB = 10; // Needs to be a PWM pin to be able to control motor speed

// Light
int blueLeftPin = 8;
int blueRightPin = 7;

//logic
boolean start = false;

void setup()
{
    Serial.begin(9600);
    lcd.begin();
   
    pinMode(dir1PinA,OUTPUT);
    pinMode(dir2PinA,OUTPUT);
    pinMode(speedPinA,OUTPUT);
    pinMode(dir1PinB,OUTPUT);
    pinMode(dir2PinB,OUTPUT);
    pinMode(speedPinB,OUTPUT);

    pinMode(blueLeftPin,OUTPUT);
    pinMode(blueRightPin,OUTPUT);

    intro();

    start = true;

}


void loop()
{

  while(start){

     LCD_start();
     FWD(1000);
     still(1000);
     BWD(1000);
     still(1000);
     RIGHT(1000);
     still(1000);
     LEFT(1000);

     start = false;
  }
   still(10);
   LCD_stop();
   turnOffAllLight();
   
 // demoOne();
 // delay(1000);
 

}

void FWD(int duration){

  analogWrite(speedPinA, 255);//Sets speed variable via PWM 
  digitalWrite(dir1PinA, HIGH);
  digitalWrite(dir2PinA, LOW);

  
  analogWrite(speedPinB, 255);
  digitalWrite(dir1PinB, HIGH);
  digitalWrite(dir2PinB, LOW);
  
  Serial.println("FWD");
  Serial.println("   ");
  
  lcd.setCursor(4, 0);                   
  lcd.print("Forward");  
  lcd.setCursor(0, 1);                   
  lcd.print("<<<<<<<  >>>>>>>"); 
  doubleLight(HIGH);
  delay(duration);
  doubleLight(LOW);
  lcd.clear();
}

void BWD(int duration){

  analogWrite(speedPinA, 255);//Sets speed variable via PWM 
  digitalWrite(dir1PinA, LOW);
  digitalWrite(dir2PinA, HIGH);

  
  analogWrite(speedPinB, 255);
  digitalWrite(dir1PinB, LOW);
  digitalWrite(dir2PinB, HIGH);
  
  Serial.println("BWD");
  Serial.println("   ");

  lcd.setCursor(4, 0);                   
  lcd.print("Backward");  
  lcd.setCursor(0, 1);                   
  lcd.print(">>>>>>>  <<<<<<<"); 
  doubleLight(HIGH);
  delay(duration);
  doubleLight(LOW);
  lcd.clear();
}

void still(int duration){
  
  analogWrite(speedPinA, 0);
  digitalWrite(dir1PinA, LOW);
  digitalWrite(dir2PinA, HIGH);
  
  analogWrite(speedPinB, 0);
  digitalWrite(dir1PinB, LOW);
  digitalWrite(dir2PinB, HIGH);

  Serial.println("still");
  Serial.println("   ");

  lcd.setCursor(6, 0);                   
  lcd.print("Still");  
  lcd.setCursor(0, 1);                   
  lcd.print("XXXXXXX  XXXXXXX"); 
  delay(duration);
  lcd.clear();
}



void RIGHT(int duration){

  analogWrite(speedPinA, 255);//Sets speed variable via PWM 
  digitalWrite(dir1PinA, HIGH);
  digitalWrite(dir2PinA, LOW);

  
  analogWrite(speedPinB, 255);
  digitalWrite(dir1PinB, LOW);
  digitalWrite(dir2PinB, HIGH);
  
  Serial.println("RIGHT");
  Serial.println("   ");

  lcd.setCursor(4, 0);                   
  lcd.print("SPIN RIGHT");  
  lcd.setCursor(0, 1);                   
  lcd.print("<<<<<<<         "); 
  rightLight(HIGH);
  delay(duration);
  rightLight(LOW);
  lcd.clear();
}

void LEFT(int duration){

  analogWrite(speedPinA, 255);//Sets speed variable via PWM 
  digitalWrite(dir1PinA, LOW);
  digitalWrite(dir2PinA, HIGH);

  
  analogWrite(speedPinB, 255);
  digitalWrite(dir1PinB, HIGH);
  digitalWrite(dir2PinB, LOW);
  
  Serial.println("LEFT");
  Serial.println("   ");

  lcd.setCursor(4, 0);                   
  lcd.print("SPIN LEFT");  
  lcd.setCursor(0, 1);                   
  lcd.print("         >>>>>>>"); 
  leftLight(HIGH);
  delay(duration);
  leftLight(LOW);
  lcd.clear();
}

// DISPLAY FUNCTION

void rightLight(int assign){
  digitalWrite(blueRightPin, assign);
}

void leftLight(int assign){
  digitalWrite(blueLeftPin, HIGH);
}

void doubleLight(int assign){
  digitalWrite(blueLeftPin, assign);
  digitalWrite(blueRightPin, assign);
}

void turnOffAllLight(){
  digitalWrite(blueLeftPin, LOW);
  digitalWrite(blueRightPin, LOW);
}

void intro(){
  lcd.setCursor(2, 0);
  lcd.print("System Init!"); 
  delay(1000);
  lcd.clear(); 
  lcd.setCursor(5, 0);                   
  lcd.print("Hello!");  
  lcd.setCursor(4, 1);                   
  lcd.print("PichayaP"); 
  delay(1000);
  lcd.clear();
}

void LCD_start(){
  lcd.setCursor(2, 0);
  lcd.print("Start Route"); 
  delay(1000);
  lcd.clear(); 
}
void LCD_stop(){
  lcd.setCursor(3, 0);
  lcd.print("Stop Route"); 
  delay(1000);
  lcd.clear(); 
}
