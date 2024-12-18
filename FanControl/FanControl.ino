
#define CLK 11
#define DIO 12

#include "GyverTM1637.h"
GyverTM1637 disp(CLK, DIO);


#define CLKE 8
#define DT 9
#define SW 10

#include "GyverEncoder.h"
//Encoder enc1(CLK, DT);      // для работы без кнопки
//Encoder enc1(CLKE, DT, SW);  // для работы c кнопкой
Encoder enc1(CLKE, DT, SW, TYPE1);  // для работы c кнопкой и сразу выбираем тип
//Encoder enc1(CLK, DT, ENC_NO_BUTTON, TYPE2);  // для работы без кнопки и сразу выбираем тип

// Варианты инициализации:
// Encoder enc;                  // не привязан к пину
// Encoder enc(пин CLK, пин DT);        // энкодер без кнопки (ускоренный опрос)
// Encoder enc(пин CLK, пин DT, пин SW);    // энкодер с кнопкой
// Encoder enc(пин CLK, пин DT, пин SW, тип); // энкодер с кнопкой и указанием типа
// Encoder enc(пин CLK, пин DT, ENC_NO_BUTTON, тип);  // энкодер без кнопкой и с указанием типа

boolean flag;
int value = 10;

// ---------------------------
void setup() {
  Serial.begin(9600);

 // enc1.setFastTimeout(40);    // таймаут на скорость isFastR. По умолч. 50

  disp.clear();
  disp.brightness(0);  // яркость, 0 - 7 (минимум - максимум)

  disp.point( false);
  disp.displayInt(value);

  //-----------------------------------
  initPWM();
 // pinMode(2, INPUT); // кол-во оборотов
 // pinMode(2, INPUT);
  pinMode(3, OUTPUT); // pwm
  mega328_PWM20kHz_D3(30);

 // attachInterrupt(0, rpm, CHANGE);

}

volatile long val = 0;
int counter = 0;      // замена i
uint32_t timer = 0;   // переменная таймера
#define T_500MLS 500  // период переключения


void rpm() {
  val++;
}

void loop() {

  // обязательная функция отработки. Должна постоянно опрашиваться
  enc1.tick();

  if (enc1.isRight()) value = (value + 10) ;    // если был поворот направо, увеличиваем на 1
  if (enc1.isLeft()) value = (value - 10) ;     // если был поворот налево, уменьшаем на 1

  if (enc1.isRightH()) value += 5;    // если было удержание + поворот направо, увеличиваем на 5
  if (enc1.isLeftH()) value -= 5;     // если было удержание + поворот налево, уменьшаем на 5

  if (enc1.isFastR()) value += 50;    // если был быстрый поворот направо, увеличиваем на 10
  if (enc1.isFastL()) value -= 50;    // если был быстрый поворот налево, уменьшаем на 10

  if (enc1.isTurn()) {                // если был совершён поворот (индикатор поворота в любую сторону)
    Serial.println(value);            // выводим значение при повороте
    disp.displayInt( value);    // выводим время

    mega328_PWM20kHz_D3(constrain(value, 0, 255));
    delay(50);
  }


  if (millis() - timer >= T_500MLS) { // таймер на millis()
    timer = millis(); // сброс

    long r = val;
    // val = digitalRead(2);
    // disp.displayInt(i);
    Serial.println("r:[" + (String)r + "], rpm:[" + (r * 60) / 2 + "]");
    // delay(200);
    val = 0;
  }
}



void initPWM() {
  // инициализация ШИМ 20 кГц для mega328
  TCCR2A = 0b10100011;
  TCCR2B = 0b00001010;
  OCR2A = 99;
}
void mega328_PWM20kHz_D3(uint8_t duty) {
  if (duty == 0) {
    bitClear(TCCR2A, COM2B1);
    bitClear(PORTD, 3);
  } else {
    bitSet(TCCR2A, COM2B1);
    OCR2B = map(duty, 0, 255, 0, 99);
  }
}
