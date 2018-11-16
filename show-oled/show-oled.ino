

// OLED экран
#include <OLED_I2C.h>
OLED  myOLED(SDA, SCL, 8);
//extern uint8_t MegaNumbers[];
//extern uint8_t RusFont[];
extern uint8_t SmallFont[];
extern uint8_t MediumNumbers[];
//extern uint8_t BigNumbers[];


#define WIDTH 127            // ширина матрицы
#define HEIGHT 63           // высота матрицы

void setup() {
  myOLED.begin();

}

void loop() {
  myOLED.clrScr();


  snow();
  myOLED.update();

  delay(1500);

}


void snow() {
  // сдвигаем всё вниз
  /*for (byte x = 0; x < WIDTH; x++) {
    for (byte y = 0; y < HEIGHT - 1; y++) {
      myOLED.setPixel(x, y);
    }
  }
*/
  for (byte x = 0; x < WIDTH; x++) {
    // заполняем случайно верхнюю строку
    // а также не даём двум блокам по вертикали вместе быть
    if ( (random(0, 10) == 0))
      myOLED.setPixel(x, HEIGHT - 1);
    else
      myOLED.setPixel(x, HEIGHT - 1);
  }
}
