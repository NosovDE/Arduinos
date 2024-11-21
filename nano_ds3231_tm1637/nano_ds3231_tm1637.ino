/*
   Пример вывода на дисплей с регистром TM1637
   показывает все возможности библиотеки GyverTM1637
   AlexGyver Technologies http://alexgyver.ru/
*/

#define CLK 12
#define DIO 11

#include "GyverTM1637.h"
GyverTM1637 disp(CLK, DIO);



#include "RTClib.h"
RTC_DS3231 rtc;


uint32_t Now, clocktimer;
boolean flag;

void setup() {
  // Initialize the rtc object
  rtc.begin();
  // The following lines can be uncommented to set the date and time
  // rtc.setDOW(4);     // Set Day-of-Week to SUNDAY
  // rtc.setTime(20, 54, 0);     // Set the time to 12:00:00 (24hr format)
  //rtc.setDate(23, 11, 2018);   // Set the date to January 1st, 2014
  // rtc.adjust(DateTime(2018, 23, 11, 21, 55, 0));
 rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  // Serial.begin(9600);
  disp.clear();
  disp.brightness(3);  // яркость, 0 - 7 (минимум - максимум)

}

void loop() {
  // Get data from the DS3231
  DateTime now = rtc.now();

  //disp.point(t.sec % 2 == 0 ? true : false); // выкл/выкл точки
  disp.point( true); // выкл/выкл точки
  disp.displayClockScroll(now.hour(), now.minute(), 70);    // выводим время

  delay(500);
  // twistClock(t.hour, t.min);

  //normClock(t.hour, t.min);
  disp.point(false); // выкл/выкл точки
  disp.displayClockScroll(now.hour(), now.minute(), 70);    // выводим время
  delay(500);
}
