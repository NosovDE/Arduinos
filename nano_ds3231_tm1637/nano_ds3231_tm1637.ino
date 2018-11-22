/*
   Пример вывода на дисплей с регистром TM1637
   показывает все возможности библиотеки GyverTM1637
   AlexGyver Technologies http://alexgyver.ru/
*/

#define CLK 2
#define DIO 3

#include "GyverTM1637.h"
GyverTM1637 disp(CLK, DIO);



#include <DS3231.h>

// Init the DS3231 using the hardware interface
DS3231  rtc(SDA, SCL);
// Init a Time-data structure
Time  t;

uint32_t Now, clocktimer;
boolean flag;

void setup() {
  // Initialize the rtc object
  rtc.begin();
  // The following lines can be uncommented to set the date and time
 // rtc.setDOW(4);     // Set Day-of-Week to SUNDAY
 // rtc.setTime(22, 39, 0);     // Set the time to 12:00:00 (24hr format)
//  rtc.setDate(22, 11, 2018);   // Set the date to January 1st, 2014

  // Serial.begin(9600);
  disp.clear();
  disp.brightness(7);  // яркость, 0 - 7 (минимум - максимум)

}

void loop() {

  // Get data from the DS3231
  t = rtc.getTime();



  //disp.point(t.sec % 2 == 0 ? true : false); // выкл/выкл точки
  disp.point( true); // выкл/выкл точки
  disp.displayClockScroll(t.hour, t.min, 70);    // выводим время

  delay(500);
  // twistClock(t.hour, t.min);

  //normClock(t.hour, t.min);
  disp.point(false); // выкл/выкл точки
  disp.displayClockScroll(t.hour, t.min, 70);    // выводим время
  delay(500);
}

void twistClock(byte hrs , byte mins) {
  // byte hrs = 21, mins = 55;
  uint32_t tmr;
  Now = millis();
  while (millis () - Now < 10000) {   // каждые 10 секунд
    if (millis() - tmr > 500) {       // каждые полсекунды
      tmr = millis();
      flag = !flag;
      disp.point(flag);   // выкл/выкл точки

      if (flag) {
        // ***** часы! ****
        mins ++;
        if (mins > 59) {
          mins = 0;
          hrs++;
          if (hrs > 24) hrs = 0;
        }
        // ***** часы! ****
        disp.displayClockScroll(hrs, mins, 35);    // выводим время
      }
    }
  }
  disp.point(0);   // выкл точки
}

void scrolls() {
  // прокрутка массив ЦИФР
  byte digs[4] = {3, 5, 7, 1};
  disp.scroll(digs, 100);     // скорость прокрутки 100
  delay(1000);

  // прокрутка прицельно (ячейка, ЦИФРА, скорость)
  disp.scroll(0, 8, 200);
  delay(1000);

  disp.clear();
  delay(1000);
  for (byte i = 0; i < 10; i++) {
    disp.scroll(3, i, 50);
    delay(400);
  }

  // прокрутка массива БАЙТ
  byte troll[4] = {_t, _r, _o, _l};
  disp.scrollByte(troll, 100);
  delay(1000);

  // прицельная прокрутка БАЙТА (ячейка, байт, скорость)
  disp.scrollByte(2, _G, 50);
  delay(1000);
}

void bytes() {
  // выводим байты из массива
  byte troll[4] = {_t, _r, _o, _l};
  disp.displayByte(troll);
  delay(1000);

  // выводим байты напрямую (4 в скобках)
  disp.displayByte(_L, _O, _L, _empty);
  delay(1000);

  // выводим байты "прицельно"
  disp.displayByte(3, _O);    // 3 ячейка, буква О
  delay(1000);

  // выводим цифры из массива
  byte hell[4] = {6, 6, 6, 6};
  disp.display(hell);
  delay(1000);

  // выводим цифры напрямую (4 в скобках)
  disp.display(1, 2, 3, 4);
  delay(1000);

  // выводим цифры "прицельно"
  disp.display(0, 9);    // 0 ячейка, цифра 9
  delay(1000);
}

void fadeBlink() {
  // пишем HELL
  disp.displayByte(_H, _E, _L, _L);

  Now = millis();
  while (millis () - Now < 3000) {    // 3 секунды
    for (int i = 7; i > 0; i--) {
      disp.brightness(i);   // меняем яркость
      delay(40);
    }
    for (int i = 0; i < 8; i++) {
      disp.brightness(i);   // меняем яркость
      delay(40);
    }
  }
}

void scrollClock(byte hrs , byte mins) {
  //  byte hrs = 15, mins = 0;
  uint32_t tmr;
  Now = millis();
  while (millis () - Now < 10000) {   // каждые 10 секунд
    if (millis() - tmr > 500) {       // каждые полсекунды
      tmr = millis();
      flag = !flag;
      disp.point(flag);   // выкл/выкл точки

      if (flag) {
        // ***** часы! ****
        mins ++;
        if (mins > 59) {
          mins = 0;
          hrs++;
          if (hrs > 24) hrs = 0;
        }
        // ***** часы! ****
        disp.displayClockScroll(hrs, mins, 70);    // выводим время
      }
    }
  }
  disp.point(0);   // выкл точки
}

void normClock(byte hrs , byte mins ) {
  //byte hrs = 15, mins = 0;
  uint32_t tmr;
  Now = millis();
  while (millis () - Now < 10000) {   // каждые 10 секунд
    if (millis() - tmr > 500) {       // каждые полсекунды
      tmr = millis();
      flag = !flag;
      disp.point(flag);   // выкл/выкл точки

      // ***** часы! ****
      mins ++;
      if (mins > 59) {
        mins = 0;
        hrs++;
        if (hrs > 24) hrs = 0;
      }
      // ***** часы! ****
      disp.displayClock(hrs, mins);   // выводим время функцией часов
    }
  }
  disp.point(0);   // выкл точки
}


