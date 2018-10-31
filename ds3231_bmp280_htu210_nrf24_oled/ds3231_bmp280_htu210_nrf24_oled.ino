// Часы DS3231
#include <DS3231.h>
DS3231  rtc(SDA, SCL);

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
  rtc.begin(); // Initialize the rtc object

  //  Serial.begin(9600);
  if (!bmp.begin()) {
    //  Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
}



void loop() {

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

  // Time
  String time = rtc.getTimeStr();
  String date = rtc.getDateStr();

  myOLED.setFont(MediumNumbers);
  myOLED.print(time, CENTER, 0);
  myOLED.update();

  delay(1000);
}
