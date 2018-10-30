
#define CLK 7
#define DT 8
#define SW 9
#include "GyverEncoder.h"
Encoder enc1(CLK, DT, SW);
Encoder enc2(5, 6, 4);


// OLED экран
#include <OLED_I2C.h>
OLED  myOLED(SDA, SCL, 8);
extern uint8_t MegaNumbers[];
extern uint8_t RusFont[];
extern uint8_t SmallFont[];




void setup() {
  enc1.setType(TYPE2);
  enc2.setType(TYPE2);
 // enc1.setTickMode(AUTO);
 // enc2.setTickMode(AUTO);

  myOLED.begin();
  myOLED.setFont(SmallFont);
  myOLED.print("Left", LEFT, 1);
  myOLED.print("Right", RIGHT, 1);

  myOLED.setFont(MegaNumbers);
  myOLED.print("0", LEFT, 15);
  myOLED.print("0", RIGHT, 15);
  myOLED.update();

//  Serial.begin(9600);
//  Serial.println("TwoKnobs Encoder Test:");
}

long positionLeft  = 0;
long positionRight = 0;

void loop() {
  // обязательная функция отработки. Должна постоянно опрашиваться
  enc1.tick();
  enc2.tick();

//  if (enc1.isTurn()) {     // если был совершён поворот (индикатор поворота в любую сторону)
    // ваш код
//  }

  if (enc1.isRight())
  {
    //Serial.println("Right");         // если был поворот
    positionRight++;
    updateScreen();
  }
  if (enc1.isLeft())
  {
   // Serial.println("Left");
    positionRight--;
    updateScreen();
  }

if (enc2.isRight())
  {
    positionLeft++;
    updateScreen();
  }
  if (enc2.isLeft())
  {
    positionLeft--;
    updateScreen();
  }


/*
  if (enc1.isRightH()) Serial.println("Right holded"); // если было удержание + поворот
  if (enc1.isLeftH()) Serial.println("Left holded");

  if (enc1.isPress()) Serial.println("Press");         // нажатие на кнопку (+ дебаунс)
  if (enc1.isRelease()) Serial.println("Release");     // отпускание кнопки (+ дебаунс)
  if (enc1.isHolded()) Serial.println("Holded");       // если была удержана и энк не поворачивался
  //if (enc1.isHold()) Serial.println("Hold");         // возвращает состояние кнопки
  */
 
  
}

void updateScreen() {
 myOLED.setFont(MegaNumbers);
  myOLED.print(String(positionLeft), LEFT, 15);
  myOLED.print(String(positionRight), RIGHT, 15);
  myOLED.update();
}
