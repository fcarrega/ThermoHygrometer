// Dependencies
#include <dht11.h>
dht11 DHT11;
#define DHT11PIN 13 // DHT11 signal pin must be plugged to a digital input (pins 8 to 13 on Arduino UNO)

#include <RTClib.h>
#include <Wire.h>
RTC_DS1307 DS1307;

#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
byte temperatureChar[8] = { B00100, B01010, B01010, B01110, B01110, B11111, B11111, B01110 };
byte dropChar[8] = { B00100, B00100, B01010, B01010, B10001, B10001, B10001, B01110 };
byte degreeChar[8] = { B11100, B10100, B11100, B00000, B00000, B00000, B00000, B00000 };

// Custom print fuctions
void printOnLCD(int customChar, float value, int type){
  lcd.write(customChar);
  lcd.print(" ");
  lcd.print((float)value, 1);
  switch(type){
    case 1:
      lcd.write(3);
      lcd.print("C ");
      break;
    case 2:
      lcd.print("%");
      break;
  }
}

void printInteger(int integer){
  if (integer < 10)
  {
    lcd.print(0);
  }
  lcd.print(integer);
}

void printDate(int day, int month, int year,  int hour, int minute){
  printInteger(day);
  lcd.print("/");
  printInteger(month);
  lcd.print("/");
  lcd.print(year);
  lcd.print(" ");
  printInteger(hour);
  lcd.print(":");
  printInteger(minute);
}

// Setup
void setup(){  
  lcd.createChar(1, temperatureChar);
  lcd.createChar(2, dropChar);
  lcd.createChar(3, degreeChar);
  lcd.begin(16,2);
  
  Serial.begin(9600);
  Wire.begin();
  DS1307.begin();   // turn on the internal clock
}

// Loop
void loop(){
  // Read date from DS1307
  DateTime now = DS1307.now();
  
  // Read DHT11 sensor
  DHT11.read(DHT11PIN);

  // Clear LCD
  lcd.clear();

  // Write on first line
  lcd.setCursor(0, 0);
  printDate(now.day(), now.month(), now.year(), now.hour(), now.minute());
  
  // Write on second line
  lcd.setCursor(0, 1);
    
  // Display temperature
  printOnLCD(1, DHT11.temperature, 1);
  
  // Display humidity
  printOnLCD(2, DHT11.humidity, 2);
  
  // Wait
  delay(2000);
}
