/*
Статья "Цифровой VU meter с OLED дисплеем. Arduino проект для начинающих":
http://www.techn0man1ac.info/arduino/diy-digital-vumeter-oled-and-arduino
Видео "Arduino проект для начинающих. VU meter с OLED дисплеем. Techn0man1ac #2 "
https://youtu.be/qoLsynwssLI
За основу взят вот этот проект:
https://forum.arduino.cc/index.php?topic=403234.0

By Techn0man1ac
*/

#include "display.c"
#include <OLED_I2C.h> // Library: OLED_I2C http://www.rinkydinkelectronics.com/library.php?id=79


OLED  myOLED(SDA, SCL, 8); // Arduino Nano SDA - pin A4, SCL - A5

#define analogInput 0 // В этой строчке задаётся аналоговый пин ардуино;
#define HighSens true // режим высокой чувствительности, достигается за счёт изменения опорного напряжения в 1.1 V,
//может быть как включено true, так и выключено — false, рекомендую включить;
#define FASTADC true // ускорение работы ADC, для Arduino Nano/Uno/Pro Mini рекомендуется включить(true );
#define Sensitivity 1024 // чувствительность, это значение не может быть больше 1024, если Ваш источник аудиосигнала
//выдаёт небольшое выходное напряжение, то можете попробовать в 2, 4 раза уменьшить это значение;
#define SampleWindow 50  // количество сэмплов, число в миллисекундах, чем оно меньше, тем шустрее дёргается стрелка,
//по умолчанию 50, но я для себя подобрал 15-20, больше уже ардуина не тянет.

// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

//extern uint8_t VUMeter[];

int hMeter = 65;                      // horizontal center for needle animation
int vMeter = 85;                      // vertical center for needle animation (outside of dislay limits)
int rMeter = 80;                      // length of needle animation or arch of needle travel

int sample;

void setup() {

  if (HighSens == true) {  // Включить(true)/выключить(false) режим высокой чувствительности
    analogReference(INTERNAL);  //an built-in reference, equal to 1.1 volts on the ATmega168 or ATmega328
    // and 2.56 volts on the ATmega8 (not available on the Arduino Mega)
    //INTERNAL1V1: a built-in 1.1V reference (Arduino Mega only)
  } else {
    analogReference(DEFAULT);
  }

#if FASTADC
  // set prescale to 16
  sbi(ADCSRA, ADPS2) ;
  cbi(ADCSRA, ADPS1) ;
  cbi(ADCSRA, ADPS0) ;
#endif

  myOLED.begin();          // needed for SH1106 display
  myOLED.clrScr();                                 // clears display from any library info displayed
}

void loop() {

  unsigned long startMillis = millis();                    // start of sample window
  unsigned int PeaktoPeak = 0;                             // peak-to-peak level
  unsigned int SignalMax = 0;
  unsigned int SignalMin = Sensitivity;

  while ( millis() - startMillis < SampleWindow ) {

    sample = analogRead(analogInput);
    sample = random(10, 900);

    if (sample < Sensitivity) {

      if (sample > SignalMax) {

        SignalMax = sample;                                // saves just the max levels
      }

      else if (sample < SignalMin) {

        SignalMin = sample;                                // saves just the min levels
      }
    }
  }

  PeaktoPeak = SignalMax - SignalMin;                      // max - min = peak-peak amplitude
  float MeterValue = PeaktoPeak * 330 / Sensitivity;              // convert volts to arrow information

  MeterValue = MeterValue - 34;                            // shifts needle to zero position
  //myOLED.clrScr();                                // refresh display for next step
  myOLED.drawBitmap(0, 0, VUMeter, 128, 64);       // draws background
  int a1 = (hMeter + (sin(MeterValue / 57.296) * rMeter)); // meter needle horizontal coordinate
  int a2 = (vMeter - (cos(MeterValue / 57.296) * rMeter)); // meter needle vertical coordinate
  myOLED.drawLine(a1, a2, hMeter, vMeter);         // draws needle
  myOLED.update();
}