#include <Adafruit_NeoPixel.h>
#define PIN 8 // номер порта к которому подключен модуль
#define count_led 1 // количество светодиодов 
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(1, 8, NEO_GRB + NEO_KHZ800);

/*
  #include "Ai_WS2811.h"
  #define DATA_PIN 8
  #define NUM_LEDS 1
  Ai_WS2811 ws2811;
*/


#include <Wire.h>
#include <BH1750.h>
BH1750 lightMeter;



// Часы DS3231
//#include <DS3231.h>
//DS3231  rtc(SDA, SCL);

/*
  Код для двуцветной LED мигалки  с широтно импульсной модуляцией (ШИМ=PWM)
  БЕЗ ИСПОЛЬЗОВАНИЯ ФУНКЦИИ    delay!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
*/

#define ledPin1  5            // PIN СИНИЙ LED     
#define ledPin2  3            // PIN КРАСНЫЙ LED
#define pot1 1                // PIN ПОТЕНЦЕОМЕТРА ДЛИННЫ ВСПЫШКИ
#define pot2 2                // PIN ПОТЕНЦЕОМЕТРА ДЛИННЫ ЦИКЛА

boolean BlinkPeriod = 0;                  // Триггер периода вспышки за цикл
boolean OFF_LED_Period = 0;   // Триггер периода запрета вспышки за цикл
int pos = 1;                             // Переменная смены цвета и количеста вспышек (счетчик)
long t1;                                  // Время вспышки  (длинна вспышки)
long t2;                                 // Время  цикла
long time;
int count;                           // Счетчик импульсов

// Барометр
//#include <Adafruit_Sensor.h>
#include "Adafruit_BMP280.h"
Adafruit_BMP280 bmp; // I2C
float pressure;    //To store the barometric pressure (Pa)
float temperature;  //To store the temperature (oC)
int altimeter;    //To store the altimeter (m) (you can also use it as a float variable)

// Барометр другой
//#include <iarduino_Pressure_BMP.h>                          // Подключаем библиотеку iarduino_Pressure_BMP для работы с датчиками атмосферного давления BMP180 или BMP280.
//iarduino_Pressure_BMP bmp;                               // Объявляем объект sensor для работы с датчиком давления, используя функции и методы библиотеки iarduino_Pressure_BMP.



// OLED экран
#include <OLED_I2C.h>
OLED  myOLED(SDA, SCL, 8);
//extern uint8_t MegaNumbers[];
//extern uint8_t RusFont[];
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
//extern uint8_t BigNumbers[];


void setup() {
  //sensor.begin();

  myOLED.begin();
  // rtc.begin(); // Initialize the rtc object

  //  Serial.begin(9600);
  if (!bmp.begin()) {
    //  Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }

  // два вывода на выход
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);


  lightMeter.begin();
  //ws2811.init(DATA_PIN);
  pixels.begin();
  // pixels.show(); // Устанавливаем все светодиоды в состояние "Выключено"
}

 uint16_t lux = 32000;
void loop() {
  for (int i = 0; lux<200 && i < 3; i++) {
    pixels.setPixelColor(0, pixels.Color(0, 0, 255)); // Назначаем для первого светодиода цвет "Зеленый"
    pixels.show();
    delay(50);
    pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Назначаем для первого светодиода цвет "Зеленый"
    pixels.show();
    delay(50);
    pixels.setPixelColor(0, pixels.Color(0, 0, 255)); // Назначаем для первого светодиода цвет "Зеленый"
    pixels.show();
    delay(100);

    pixels.setPixelColor(0, pixels.Color(255, 0, 0)); // Назначаем для первого светодиода цвет "Зеленый"
    pixels.show();
    delay(50);
    pixels.setPixelColor(0, pixels.Color(0, 0, 0)); // Назначаем для первого светодиода цвет "Зеленый"
    pixels.show();
    delay(50);
    pixels.setPixelColor(0, pixels.Color(255, 0, 0)); // Назначаем для первого светодиода цвет "Зеленый"
    pixels.show();
    delay(100);


  }
  pixels.setPixelColor(0, pixels.Color(0, 0, 0)); 
  pixels.show();
  /*
    for (int i = 0; i < 100; i++) {
    ws2811.setColor(0x0, 0x0, 0xFF);
    sendLEDs();
    _delay_ms(100);

    ws2811.setColor(0, 0xFF, 0x0);
    sendLEDs();
    _delay_ms(100);

    }
    /*
    ws2811.setColor(a,b,a);
    sendLEDs();
    _delay_ms(1000);

    ws2811.setColor(b,a,a);
    sendLEDs();
    _delay_ms(1000);
  */
  //Read values from the sensor:
  pressure = bmp.readPressure();
  temperature = bmp.readTemperature();
  altimeter = bmp.readAltitude (1050.35); //Change the "1050.35" to your city current barrometric pressure (https://www.wunderground.com)
  /*
    if (!bmp.read(2)) {
      pressure = bmp.pressure;
      temperature = bmp.temperature;
      altimeter = bmp.altitude;
    }
  */

  myOLED.setFont(SmallFont);
  myOLED.print("P: " + (String)(pressure / 133.3224) + " mm.r.s" , CENTER, 35);
  myOLED.print("T: " + (String)temperature + " C" , CENTER, 45);
  myOLED.print("A: " + (String)altimeter + " m" , CENTER, 55);

   lux = lightMeter.readLightLevel();

  myOLED.print("light: " + (String)lux + " lux" , CENTER, 20);


  // Time
  // String time = rtc.getTimeStr();
  //String date = rtc.getDateStr();

  // myOLED.setFont(MediumNumbers);
  // myOLED.print(time, CENTER, 0);
  myOLED.update();

  int potpos1 = analogRead(pot1); //Читает показания потенцеометра №1
  int potpos2 = analogRead(pot2); //Читает показания потенцеометра №2
  t1 = map(potpos1, 0, 1023, 0, 300); //Переводит показания потенцеометра №1 во время
  t2 = map(potpos2, 0, 1023, 0, 300); //Переводит показания потенцеометра №2 во время


  //ГЕНЕРАТОР ПРЯМОУГОЛЬНЫХ ИМПУЛЬСОВ С ПЕРЕМЕННОЙ СКВАЖНОСТЬЮ
  if ((BlinkPeriod == 0) && (OFF_LED_Period == 0))
  {
    time = millis();
    BlinkPeriod = 1;
    count++;                                        // Счетчик импульсов вспышки
    if (count > 5) count = 1;           // 5 - позиций счетчика
    OFF_LED_Period = 0;
  }
  if ( BlinkPeriod && (millis() - time) > t1)     //Время Вспышки
  {
    BlinkPeriod = 0;
    OFF_LED_Period = 1;
  }
  if (OFF_LED_Period && (millis() - time) > t2) //Время Цикла
  {
    OFF_LED_Period = 0;
  }

  //ОПРЕДЕЛЕНИЕ МИГАЮЩЕГО LED (КРАСНЫЙ - СИНИЙ)
  if ((count == 1) || (count == 2)) // ДВЕ ВСПЫШКИ
  {
    pos = 1; // КРАСНЫЙ LED
  }
  if (count == 3)                        // ПАУЗА МЕЖДУ СМЕНОЙ ЦВЕТА
  {
    pos = 2; // Одна позиция счетчика на паузу
  }
  if ((count == 4) || (count == 5)) // ДВЕ ВСПЫШКИ
  {
    pos = 3; // СИНИЙ LED
  }

  //ВЫХОДНОЙ СИГНАЛ
  if ((BlinkPeriod == 1) && (pos == 1)) // Мигает КРАСНЫЙ LED
  {
    digitalWrite(ledPin1, LOW);
  }
  else
  {
    digitalWrite(ledPin1, HIGH);
  }

  if ((BlinkPeriod == 1) && (pos == 3)) // Мигает СИНИЙ LED
  {
    digitalWrite (ledPin2, LOW);
  }
  else
  {
    digitalWrite (ledPin2, HIGH);
  }

  // delay(10); // Ждем
  // delay(1000);
}

void sendLEDs()
{
  cli();
  for (byte i = 0; i < 1; ++i) {
    //    ws2811.send();
  }
  sei();
}
