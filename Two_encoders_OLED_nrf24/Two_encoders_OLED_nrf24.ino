#include "GyverEncoder.h"

Encoder enc1(5, 6, 4);
Encoder enc2(7, 8, 9);


// OLED экран
#include <OLED_I2C.h>
OLED  myOLED(SDA, SCL, 8);
//extern uint8_t MegaNumbers[];
extern uint8_t RusFont[];
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];

long positionLeft  = 0;
long positionRight = 0;



void setup() {
  enc1.setType(TYPE2);
  enc2.setType(TYPE2);
  // enc1.setTickMode(AUTO);
  // enc2.setTickMode(AUTO);

  myOLED.begin();
  updateScreen(positionLeft, positionRight);

  //  Serial.begin(9600);
  //  Serial.println("TwoKnobs Encoder Test:");
}


void loop() {
  // обязательная функция отработки. Должна постоянно опрашиваться
  enc1.tick();
  enc2.tick();


  if (enc1.isRight())
  {
    //Serial.println("Right");         // если был поворот
    positionLeft--;
    // updateScreen();
  }
  if (enc1.isLeft())
  {
    // Serial.println("Left");
    positionLeft++;
    //    updateScreen();
  }

  if (enc1.isRelease()) {
    positionLeft = 0;
    updateScreen(positionLeft, positionRight);
  }

  if (enc1.isTurn()) {     // если был совершён поворот (индикатор поворота в любую сторону)
    updateScreen(positionLeft, positionRight);
  }


  if (enc2.isRight())
  {
    positionRight--;
    //  updateScreen();
  }
  if (enc2.isLeft())
  {
    positionRight++;
    //   updateScreen();
  }
  if (enc2.isPress()) {
    positionRight = 0;
    updateScreen(positionLeft, positionRight);
  }

  if (enc2.isTurn()) {     // если был совершён поворот (индикатор поворота в любую сторону)
    updateScreen(positionLeft, positionRight);
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



void updateScreen(long left, long right) {
  myOLED.clrScr();

  // Левый квадрат
  myOLED.setFont(SmallFont);
  myOLED.print("Left", LEFT, 1);
  myOLED.setFont(RusFont);
  myOLED.print("Ntvgthfnehf", LEFT + 3, 53);
  myOLED.setFont(BigNumbers);
  myOLED.print(String(left), LEFT + 13, 20);

  myOLED.drawRoundRect(0, 16, 65, 63);
  myOLED.drawLine(0, 50, 65, 50);

  // Правый квадрат
  myOLED.setFont(SmallFont);
  myOLED.print("Right", RIGHT, 1);
  myOLED.setFont(RusFont);
  myOLED.print("Dkf;yjcnm",  70 , 53);
  myOLED.setFont(BigNumbers);
  myOLED.print(String(right), 80, 20);

  myOLED.drawRoundRect(67, 16, 127, 63);
  myOLED.drawLine(67, 50, 127, 50);


  myOLED.update();
}
