/*
#include "GyverFilters.h"
// параметры: разброс измерения, разброс оценки, скорость изменения значений
// разброс измерения: шум измерений
// разброс оценки: подстраивается сам, можно поставить таким же как разброс измерения
// скорость изменения значений: 0.001-1, варьировать самому

//GKalman testFilter(40, 40, 0.5);

// также может быть объявлен как (разброс измерения, скорость изменения значений)
GKalman testFilter(400, 0.01);
*/
#include "GyverFilters.h"
GMedian3 testFilter;




// Часы DS1302
#include <DS1302.h>
DS1302 rtc(8, 7 , 6);// (RST, DAT, CLK)

// DHT датчик температуры и влажности
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 2     // what digital pin we're connected to
// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);
float hum = 0;
float tmp = 0;

// OLED экран
#include <OLED_I2C.h>
OLED  myOLED(SDA, SCL, 8);
extern uint8_t MegaNumbers[];
extern uint8_t RusFont[];
extern uint8_t SmallFont[];


// Датчик качества воздуха
#include <MQ135.h>
MQ135 gasSensor = MQ135(A0);



//Массив, содержащий время компиляции
char compileTime[] = __TIME__;
char compileDate[] = __DATE__;
Time t;
byte i = 0;


void setup() {
  // Serial.begin(9600);
  dht.begin();

  // float rzero = gasSensor.getRZero();
  //  Serial.println("rzero=" + (String)rzero);

  myOLED.begin();
  rtc.halt(false);
  rtc.setDOW(5);

  //Получаем число из строки, зная номер первого символа
  byte hour = getInt(compileTime, 0);
  byte minute = getInt(compileTime, 3);
  byte second = getInt(compileTime, 6);

  //Serial.println("hour=" + (String)hour);
  //Serial.println("minutes=" + (String)minute);
  //Serial.println("second=" + (String)second);

  //Serial.println("compileDate=" + (String)compileDate);

  byte day = getInt(compileDate, 4);
  int year = getInt(compileDate, 7);

  //Serial.println("day=" + (String)day);
  //Serial.println("year=" + (String)year);

  rtc.setTime(getInt(__TIME__, 0), getInt(__TIME__, 3), getInt(__TIME__, 6));
  rtc.setDate(02, 11, 2018);
}

void loop() {
  myOLED.setFont(RusFont);
  t = rtc.getTime();
  switch (t.dow)
  {
    case 1:     myOLED.print("GJYTLTKMYBR", RIGHT, 0);    break;
    case 2:     myOLED.print("DNJHYBR", RIGHT, 0);        break;
    case 3:     myOLED.print("CHTLF", RIGHT, 0);          break;
    case 4:     myOLED.print("XTNDTHU", RIGHT, 0);        break;
    case 5:     myOLED.print("GZNYBWF", RIGHT, 0);        break;
    case 6:     myOLED.print("CE<<JNF", RIGHT, 0);        break;
    case 7:     myOLED.print("DJCRHTCTYMT", RIGHT, 0);    break;
  }
  String stringOne = rtc.getTimeStr();

  myOLED.setFont(SmallFont);
  myOLED.print(stringOne.substring(0, 2), 0, 0);
  myOLED.print(":", 13, 0);
  myOLED.print(stringOne.substring(3, 5), 18, 0);

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


  //  myOLED.setFont(SmallFont);
  //myOLED.print("ppm" , 100, 45);

  if (i % 2 == 0) {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    hum = dht.readHumidity();
    // Read temperature as Celsius (the default)
    tmp = dht.readTemperature();
    i = 0;

    // меряем газ
    //float ppm = gasSensor.getCorrectedPPM(tmp, hum);
    int ppm = testFilter.filtered((int)gasSensor.getPPM());
    
    // Датчик газа
    myOLED.setFont(SmallFont);
    myOLED.print(" CO2", LEFT, 18);
    myOLED.print(String(ppm) + "ppm " , 50, 18);
  }
  i++;

  myOLED.setFont(SmallFont);

  // Разделитель в часах
  myOLED.print(" ", 13, 0);
  // Рамка
  myOLED.drawRoundRect(0, 11, 127, 53 );

  // Датчик температуры и влажности
  myOLED.setFont(RusFont);
  myOLED.print("Dkf;yjcnm   " + (isnan(hum) ? "--" : String(round(hum))) + "%" , 5, 29);
  myOLED.print("Ntvgthfnehf   " + (isnan(tmp) ? "--" : String(round(tmp))) + " C" , 5, 40);

  myOLED.update();
  delay(500);
}

//Содержимое функции объяснено ниже
char getInt(const char* string, int startIndex) {
  return int(string[startIndex] - '0') * 10 + int(string[startIndex + 1]) - '0';
}
