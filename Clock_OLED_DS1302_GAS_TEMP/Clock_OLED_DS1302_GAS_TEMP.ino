#include <DS1302.h>
#include <OLED_I2C.h> 
OLED  myOLED(SDA, SCL, 8); 
extern uint8_t MegaNumbers[];
extern uint8_t RusFont[];
extern uint8_t SmallFont[];
 

DS1302 rtc(8, 7 , 6);// (RST, DAT, CLK)

Time t;
 
void setup() {
  myOLED.begin();
  rtc.halt(false);
  //rtc.setDOW(SUNDAY);
// rtc.setTime(13, 31, 0);
 // rtc.setDate(28, 10, 2018);
}
 
void loop() {
  myOLED.setFont(RusFont);
  t = rtc.getTime();
  switch (t.dow)
  {
    case 1:     myOLED.print("GJYTLTKMYBR", CENTER, 0);    break;
    case 2:     myOLED.print("DNJHYBR", CENTER, 0);        break;
    case 3:     myOLED.print("CHTLF", CENTER, 0);          break;
    case 4:     myOLED.print("XTNDTHU", CENTER, 0);        break;
    case 5:     myOLED.print("GZNYBWF", CENTER, 0);        break;
    case 6:     myOLED.print("CE<<JNF", CENTER, 0);        break;
    case 7:     myOLED.print("DJCRHTCTYMT", CENTER, 0);    break;
  }
  String stringOne = rtc.getTimeStr();
  myOLED.setFont(MegaNumbers);
  myOLED.print(stringOne.substring(0, 2), 4, 12);
  myOLED.print("/", 51, 12);
  myOLED.print(stringOne.substring(3, 5), 75, 12);
  myOLED.setFont(RusFont);
  switch (t.mon)
  {
    case 1:
      myOLED.print(String(t.date), 30, 57);
      myOLED.print("ZYDFHZ", CENTER, 57);
      myOLED.print(String(t.year), 88, 57);
      break;
    case 2:
      myOLED.print(String(t.date), 26, 57);
      myOLED.print("ATDHFKZ", CENTER, 57);
      myOLED.print(String(t.year), 92, 57);
      break;
    case 3:
      myOLED.print(String(t.date), 30, 57);
      myOLED.print("VFHNF", CENTER, 57);
      myOLED.print(String(t.year), 88, 57);
      break;
    case 4:
      myOLED.print(String(t.date), 30, 57);
      myOLED.print("FGHTKZ", CENTER, 57);
      myOLED.print(String(t.year), 88, 57);
      break;
    case 5:
      myOLED.print(String(t.date), 36, 57);
      myOLED.print("VFZ", CENTER, 57);
      myOLED.print(String(t.year), 82, 57);
      break;
    case 6:
      myOLED.print(String(t.date), 35, 57);
      myOLED.print("B>YZ", CENTER, 57);
      myOLED.print(String(t.year), 81, 57);
      break;
    case 7:
      myOLED.print(String(t.date), 35, 57);
      myOLED.print("B>KZ", CENTER, 57);
      myOLED.print(String(t.year), 81, 57);
      break;
    case 8:
      myOLED.print(String(t.date), 28, 57);
      myOLED.print("FDUECNF", CENTER, 57);
      myOLED.print(String(t.year), 90, 57);
      break;
    case 9:
      myOLED.print(String(t.date), 24, 57);
      myOLED.print("CTYNZ<HZ", CENTER, 57);
      myOLED.print(String(t.year), 94, 57);
      break;
    case 10:
      myOLED.print(String(t.date), 26, 57);
      myOLED.print("JRNZ<HZ", CENTER, 57);
      myOLED.print(String(t.year), 92, 57);
      break;
    case 11:
      myOLED.print(String(t.date), 28, 57);
      myOLED.print("YJZ<HZ", CENTER, 57);
      myOLED.print(String(t.year), 90, 57);
      break;
    case 12:
      myOLED.print(String(t.date), 26, 57);
      myOLED.print("LTRF<HZ", CENTER, 57);
      myOLED.print(String(t.year), 92, 57);
      break;
  }
  myOLED.update();
  delay(500);
  myOLED.setFont(MegaNumbers);
  myOLED.print("-", 51, 12);
  myOLED.update();
  delay(500);
}
