//-------------------------
// EEPROM
//-------------------------
#include "EEManager.h"

struct Settings {
  uint8_t version = 1;
  uint8_t intensity = 0;
};
Settings settings;  // переменная, с которой мы работаем в программе
EEManager memory(settings);

//-------------------------
// Matrix 7219
//-------------------------
#include "Adafruit_GFX.h"
#include "Max72xxPanel.h"

#define MAX_CS  10 // CS to this pin
#define MAX_DIN 11 // DIN to MOSI(11) hardware SPI
#define MAX_CLK 13 // CLK to SCK(13)  hardware SPI

int numberOfHorizontalDisplays = 1;
int numberOfVerticalDisplays = 8; // Кол-во сегментов

Max72xxPanel matrix = Max72xxPanel(MAX_CS, numberOfHorizontalDisplays, numberOfVerticalDisplays);

String tape;
int spacer = 1;
int width = 5 + spacer; // The font width is 5 pixels


//-------------------------
// RTC DS3231
//-------------------------
#include "microDS3231.h"
MicroDS3231 rtc;
DateTime date;


//-------------------------
// Датчик температуры, влажности, давления BME280
//-------------------------
#include "GyverBME280.h"
GyverBME280 bme;

//-------------------------
// Энкодер
//-------------------------
#define EN_CLK 8
#define EN_DT 9
#define EN_SW 7

#include "GyverEncoder.h"
Encoder enc(EN_CLK, EN_DT, EN_SW, TYPE2); // тип энкодера TYPE1 одношаговый, TYPE2 двухшаговый. Если ваш энкодер работает странно, смените тип

//------------------------


uint32_t timer = 0;   // переменная таймера
#define T_500MLS 500  // период переключения
boolean isEdit = false;
boolean isMinute = false;
boolean isHour = false;


void setup() {
  // Serial.begin(9600);
  memory.begin(0, 'b');

  // Часы DS3032
  if (rtc.lostPower()) {  //  при потере питания
    rtc.setTime(COMPILE_TIME);  // установить время компиляции
  }

  // rtc.setTime(0, 50, 11, 16, 12, 2024);  // установить время компиляции

  // Serial.println("Time: " + rtc.getTimeString());
  // Serial.println("Date: " + rtc.getDateString());

  // Матрицы MAX79
  matrix.setIntensity(settings.intensity); // Use a value between 0 and 15 for brightness
  matrix.setRotation(1);    // The same hold for the last display
  matrix.fillScreen(LOW);  // Задание цвета фона экрана (вне букв) LOW - темный, HIGH - светлый


  displayText("__:__WAIT ");


  // Модуль погоды BME280
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

  // Обработка энкодера
  encoderTick();
  // Обработка eeprom
  //memory.tick();

  if (millis() - timer >= T_500MLS) { // таймер на millis()
    timer = millis(); // сброс

    // получаем все данные в структуру
    DateTime now = rtc.getTime();
    boolean blink = now.second % 2;

    if (isEdit) {
      tape = (isHour  ? printDigits(date.hour) : "  ")
             + (blink ? ":" : " ")
             + (isMinute  ? printDigits(date.minute) : "  ")
             + printExtra(now);

    } else {
      tape = printDigits(now.hour)
             + (blink ? ":" : " ")
             + printDigits(now.minute)
             + printExtra(now);

    }

    displayText(tape);
  }
}

String printExtra(DateTime now) {
  int8_t period = now.second % 15;

  if (period >= 0 && period < 5)  {
    return  "+" + (String)(int)bme.readTemperature() + "@C ";
  } else if (period >= 5 && period < 10)  {
    return  (String)(int)bme.readHumidity() + "%  ";
  }
  return  (String)(int)pressureToMmHg(bme.readPressure()) + "mm  ";
}

// =======================================================================

// =======================================================================
void displayText(String text) {

  for (int i = 0; i < text.length(); i++) {

    int letter = (matrix.width()) - i * (width);
    int x = (matrix.width() + 1) - letter;
    int y = (matrix.height() - 8) / 2; // Центрируем текст по Вертикали
    if (i > 4) x = x + 3;
    matrix.drawChar(x, y, text[i], HIGH, LOW, 1);

    matrix.write(); // Вывод на дисплей

  }

}
String sp() {


  unsigned char n = '\0xB0';
  char m[2] = { '0', '\0' };
  m[0] = n;
  return String(m);
}

//-------------------------------------------
String utf8rus(String source)
{
  int i, k;
  String target;
  unsigned char n;
  char m[2] = { '0', '\0' };

  k = source.length();
  i = 0;

  while (i < k) {
    n = source[i];
    i++;

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
    m[0] = n;
    target = target + String(m);
  }
  return target;
}

String printDigits(int digits)
{
  if (digits < 10) {
    return "0" + (String)digits;
  }
  return (String) digits;
}


void encoderTick() {
  enc.tick();   // работаем с энкодером
  // *********** КЛИК ПО ЭНКОДЕРУ **********
  if (enc.isClick()) {        // клик по энкодеру

    if (!isEdit) {
      isEdit = isMinute = true;
      date = rtc.getTime();

    } else if (!isHour) {
      isHour = true;
      isMinute = !isHour;
      date.second = 0;
      rtc.setTime(date);

    } else {
      isEdit = isHour = false;
      date.second = 0;
      rtc.setTime(date);
    }

  }

  if (isEdit) {

    if (enc.isRight()) {
      if (isHour) {
        date.hour++;
        if (date.hour >= 24) {
          date.hour = 0;
        }
      }
      if (isMinute)  {
        date.minute++;
        if (date.minute > 59) {
          date.minute = 0;
        }
      }
    }

    if (enc.isLeft()) {
      if (isHour) {
        if (date.hour == 0) {
          date.hour = 23;
        } else {
          date.hour--;
        }
      }
      if (isMinute) {
        if (date.minute == 0) {
          date.minute = 59;
        } else {
          date.minute--;
        }
      }
    }
  } else {
    if (enc.isRight()) {
      settings.intensity++;
      if (settings.intensity >= 15) settings.intensity = 15;
      matrix.setIntensity(settings.intensity);
      memory.updateNow();
      
    }
    if (enc.isLeft()) {
      settings.intensity--;
      if (settings.intensity <= 0) settings.intensity = 0;
      matrix.setIntensity(settings.intensity);
      memory.updateNow();
      
    }
  }

  // *********** УДЕРЖАНИЕ ЭНКОДЕРА **********
  if (enc.isHolded()) {       // кнопка удержана
    //Serial.println("Encoder holded");
  }

}
