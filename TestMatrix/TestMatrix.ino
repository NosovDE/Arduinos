#include <GyverMAX7219.h>
MAX7219 < 2, 1, 10 > matrix;   // одна матрица (1х1), пин CS на D5


void setup() {
  Serial.begin(9600);

  matrix.begin();       // запускаем
  matrix.setBright(3);  // яркость 0..15
  //matrix.setRotation(1);   // можно повернуть 0..3, по 90 град по часовой стрелке
  matrix.textDisplayMode(GFX_REPLACE);

  matrix.setTextBound(0, 16);
  matrix.clear();
}

void loop() {
  matrix.setCursor(0, 0);
  matrix.setTextBound(0, 16);
  matrix.println("22:34");
  //matrix.print("747mm ");
  matrix.update();
  delay(1000);

  matrix.setCursor(0, 0);
  // matrix.clear();
  matrix.println("00:00");
  // matrix.setCursor(1, 0);
  // matrix.print("555mm ");
  matrix.update();
  delay(1000);
  matrix.clear();
}
