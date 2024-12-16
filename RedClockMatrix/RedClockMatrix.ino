
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Max72xxPanel.h>
#include <Wire.h>



int pinCS = 10; // Attach CS to this pin, DIN to MOSI(11) and CLK to SCK(13) (cf http://arduino.cc/en/Reference/SPI )
int numberOfHorizontalDisplays = 1;
int numberOfVerticalDisplays = 8; // Кол-во сегментов

Max72xxPanel matrix = Max72xxPanel(pinCS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

int refresh = 0;
String tape = "21:00";
const String format1 = "H:i ";
const String format2 = "H i ";
int wait = 90; // In milliseconds

int spacer = 1;
int width = 5 + spacer; // The font width is 5 pixels

#include "microDS3231.h"
MicroDS3231 rtc;




#include "GyverBME280.h"
GyverBME280 bme;


int counter = 0;      // замена i
uint32_t timer = 0;   // переменная таймера
#define T_PERIOD 500  // период переключения




void setup() {
  //Serial.begin(9600);

  if (rtc.lostPower()) {  //  при потере питания
    rtc.setTime(COMPILE_TIME);  // установить время компиляции
  }

  //rtc.setTime(SEC, MIN, HOUR, DAY, MONTH, YEAR); // устанвока времени вручную


  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2022, 3, 22, 15, 52, 0));
  }
  //clock.begin();
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // Set sketch compiling time
  //  clock.setDateTime(__DATE__, __TIME__);
  //clock.setDateTime("2017-12-10", "18:24");

  matrix.setIntensity(1); // Use a value between 0 and 15 for brightness

  // Adjust to your own needs
  //  matrix.setPosition(0, 0, 0); // The first display is at <0, 0>
  //  matrix.setPosition(1, 1, 0); // The second display is at <1, 0>
  //  matrix.setPosition(2, 2, 0); // The third display is at <2, 0>
  //  matrix.setPosition(3, 3, 0); // And the last display is at <3, 0>
  //  ...
  //  matrix.setRotation(0, 2);    // The first display is position upside down
  matrix.setRotation(1);    // The same hold for the last display

  // matrix.fillScreen(0);
  DisplayText("__:__WAIT ");

  bme.setMode(FORCED_MODE);                             // Перед инициализацией говорим датчику работать в принудительном режиме
  bme.begin();                                          // Больше настройки не нужны  - инициализируем датчик

  while (millis() < 10000) {                            // Работа с датчиком в принудительном режиме в течении первых 10 секунд
    bme.oneMeasurement();                               // Просим датчик проснуться и сделать одно преобразование
    while (bme.isMeasuring());                          // Ждем окончания преобразования

    //  Serial.print("Temperature: ");
    //  Serial.print(bme.readTemperature());                // Читаем и выводим температуру
    //  Serial.println(" *C");

    //  Serial.print("Humidity: ");
    //  Serial.print(bme.readHumidity());                   // Читаем и выводим влажность
    //   Serial.println(" %");

    //  Serial.print("Pressure: ");
    //  Serial.print(pressureToMmHg(bme.readPressure()));   // Читаем и выводим давление
    //  Serial.println(" mm Hg");
    //   Serial.println("");
    delay(1000);
  }

  bme.setMode(NORMAL_MODE);                             // Спустя 10 секунд переключаем датчик в обычный режим
  bme.begin();                                          // Переинициализируем датчик после изменения настроек - обязательная процедура

  //matrix.fillScreen(1);
}

void loop() {
  if (millis() - timer >= T_PERIOD) { // таймер на millis()
    timer = millis(); // сброс

    // получаем все данные в структуру
    DateTime now = rtc.getTime();

    tape = (String)now.hour
           + (now.second % 2 ? ":" : " ")
           + (String)(now.minute)

           +  getExtraInfo(now);

    DisplayText(tape);
  }


  /*
    if (dt.minute % 2) {
    matrix.fillScreen(LOW);
    ScrollText(utf8rus("  Темп:" + (String)temp + "C  Влаж:" + (String)hum + "%  "));
    // DisplayText((String)temp + deg + (String)hum + "% ");
    matrix.fillScreen(LOW);
    delay(2000);
    }
  */



  /*
    int i = width * tape.length() - width;//width * tape.length() + matrix.width() - 1 - spacer;

    //Serial.println("i:" + (String)i);

    matrix.fillScreen(LOW);

    int letter = i / width;
    int x = (matrix.width() - 0) - i % width;
    int y = (matrix.height() - 8) / 2; // center the text vertically

    //Serial.println("X:" + (String)x);
    // Serial.println("Y:" + (String)y);

    while ( x + width - spacer >= 0 && letter >= 0 ) {
    if ( letter < tape.length() ) {
      matrix.drawChar(x, y, tape[letter], HIGH, LOW, 1);
    }

    letter--;
    x -= width;
    }

    matrix.write(); // Send bitmap to display
  */

}

String getExtraInfo(DateTime now) {
  int8_t period = now.second % 15;

  if (period >= 0 && period < 5)  {
    return  (String)(int)bme.readTemperature() + "C  ";
  } else if (period >= 5 && period < 10)  {
    return  (String)(int)bme.readHumidity() + "%  ";
  }
  return  (String)(int)pressureToMmHg(bme.readPressure()) + "mm  ";
}

// =======================================================================
void ScrollText (String text) {

  for ( int i = 32 ; i < (width * text.length() + matrix.width() - 1 - spacer) - 32; i++ ) {
    if (refresh == 1) i = 0;
    refresh = 0;
    //  matrix.fillScreen(LOW);
    int letter = i / width;
    // int x =  1 - i % width;
    int x = (matrix.width() - 1) - i % width;
    int y = (matrix.height() - 8) / 2; // Центрируем текст по Вертикали

    while ( x + width - spacer >= 0 && letter >= 0 ) {
      if ( letter < text.length() ) {
        matrix.drawChar(x, y, text[letter], HIGH, LOW, 1);
      }
      letter--;
      x -= width;
    }
    matrix.write(); // Вывод на дисплей
    delay(wait);
  }
}
// =======================================================================
void DisplayText(String text) {

  for (int i = 0; i < text.length(); i++) {

    int letter = (matrix.width()) - i * (width);
    int x = (matrix.width() + 1) - letter;
    int y = (matrix.height() - 8) / 2; // Центрируем текст по Вертикали
    if (i > 4) x = x + 3;
    matrix.drawChar(x, y, text[i], HIGH, LOW, 1);

    matrix.write(); // Вывод на дисплей

  }

}
//-------------------------------------------
String utf8rus(String source)
{
  int i, k;
  String target;
  unsigned char n;
  char m[2] = { '0', '\0' };

  k = source.length(); i = 0;

  while (i < k) {
    n = source[i]; i++;

    if (n >= 0xC0) {
      switch (n) {
        case 0xD0: {
            n = source[i]; i++;
            if (n == 0x81) {
              n = 0xA8;
              break;
            }
            if (n >= 0x90 && n <= 0xBF) n = n + 0x30 - 1;
            break;
          }
        case 0xD1: {
            n = source[i]; i++;
            if (n == 0x91) {
              n = 0xB8;
              break;
            }
            if (n >= 0x80 && n <= 0x8F) n = n + 0x70 - 1;
            break;
          }
      }
    }
    m[0] = n; target = target + String(m);
  }
  return target;
}

String printDigits(int digits)
{
  // utility function for digital clock display: prints preceding colon and leading 0

  if (digits < 10) {
    return "0" + (String)digits;
  }
  return (String) digits;
}
