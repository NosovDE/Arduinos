
#include <OLED_I2C.h>         // Подключение библиотеки для дисплея

#include <DS3231.h>


OLED  myOLED(SDA, SCL, 8);
extern uint8_t MegaNumbers[]; // Подключение больших шрифтов
extern uint8_t SmallFont[];   // Подключение маленьких шрифтов
//#include <DS1307.h>           // Подключение библиотеки для часового модуля
DS3231 rtc(SDA, SCL);
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
  myOLED.print(rtc.getDOWStr(), CENTER, 0);   // Отображение дня недели
  String stringOne = rtc.getTimeStr();
  myOLED.setFont(MegaNumbers);
  myOLED.print(stringOne.substring(0, 2), 4, 12);  // Отображение часов
  myOLED.print("/", 51, 12);                       // Отображение двоеточия
  myOLED.print(stringOne.substring(3, 5), 75, 12); // Отображение минут
  myOLED.setFont(SmallFont);
  myOLED.print(rtc.getDateStr(), CENTER, 57);      // Отображение даты
  myOLED.update();
  delay(500);
  myOLED.setFont(MegaNumbers);                     // Скрытие двоеточия
  myOLED.print("-", 51, 12);
  myOLED.update();
  delay(500);
}