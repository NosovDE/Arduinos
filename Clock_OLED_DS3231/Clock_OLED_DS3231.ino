#include <Wire.h>
//#include <OLED_I2C.h>         // Подключение библиотеки для дисплея
//#include "DS3231.h"

#include <GyverOLED.h>



//#define SDA  4
//#define SCL  5

OLED  myOLED(SDA, SCL, 8);
extern uint8_t MegaNumbers[]; // Подключение больших шрифтов
extern uint8_t SmallFont[];   // Подключение маленьких шрифтов

 
DS3231 rtc;
RTCDateTime dt;

void setup() {
  myOLED.begin();
  rtc.begin();
  //rtc.setDOW(SUNDAY);         // Настройка дня недели
  //rtc.setTime(11, 11, 0);      // Настройка времени
  //rtc.setDate(27, 10, 2018);  // Настройка даты

  //rtc.setDateTime(__DATE__, __TIME__);
}
void loop() {
  myOLED.setFont(SmallFont);
  myOLED.print(rtc.dateFormat("w", dt), CENTER, 0);   // Отображение дня недели
    // 
  String stringOne = rtc.dateFormat("H:i:s", dt);
  myOLED.setFont(MegaNumbers);
  myOLED.print(stringOne.substring(0, 2), 4, 12);  // Отображение часов
  myOLED.print("/", 51, 12);                       // Отображение двоеточия
  myOLED.print(stringOne.substring(3, 5), 75, 12); // Отображение минут
  myOLED.setFont(SmallFont);

  String date = rtc.dateFormat("d-m-Y", dt);
  myOLED.print(date, CENTER, 57);      // Отображение даты
  myOLED.update();
 
  delay(500);
  
  myOLED.setFont(MegaNumbers);                     // Скрытие двоеточия
  myOLED.print("-", 51, 12);
  myOLED.update();
  
  delay(500);
}
