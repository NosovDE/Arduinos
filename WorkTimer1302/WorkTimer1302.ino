// Часы DS1302
#include "DS1302.h"

#define RST 8        // часы 1302
#define DAT 7        // часы 1302
#define CLK 6        // часы 1302  

DS1302 rtc(RST, DAT , CLK); // (RST, DAT, CLK)

//Массив, содержащий время компиляции
//char compileTime[] = __TIME__;
//char compileDate[] = __DATE__;
Time t;

// Дисплей TM1637
#include "GyverTM1637.h"

#define CLK 11        // дисплей
#define DIO 12        // дисплей

GyverTM1637 disp(CLK, DIO);
int8_t hrs = 0, mins = 0;


void setup() {
  // Установка времени
  rtc.halt(false); //запуск часов
  rtc.writeProtect(false); //снять защиту от записи

  rtc.setDOW(7);

  // rtc.setTime(getInt(__TIME__, 0), getInt(__TIME__, 3), getInt(__TIME__, 6));

  rtc.setTime(hrs, mins, 0);
  rtc.setDate(21, 01, 2010);

  disp.clear();
  disp.brightness(0);  // яркость, 0 - 7 (минимум - максимум)
}


void loop() {

  t = rtc.getTime();

  mins = t.min;
  hrs = t.hour;

  displayTime(0);
  delay(500);

  displayTime(1);
  delay(500);
}

/**
   Отобразить время на дисплее.
   point - отображать двоеточие
*/
void displayTime(byte point) {
  disp.point(point);
  disp.displayClock(hrs, mins);

}
