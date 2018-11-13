// Подключаем библиотеки:                                       //
#include <Wire.h>                                               // Для работы с шиной I2C
#include <SparkFun_APDS9960.h>                                  // Для работы с датчиком APDS-9960
SparkFun_APDS9960 apds = SparkFun_APDS9960();                   // Определяем объект apds, экземпляр класса SparkFun_APDS9960
//
void setup() {                                                  //
  Serial.begin(9600);                                         // Инициируем передачу данных в монитор последовательного порта на скорости 9600 бит/сек
  //  Инициируем работу датчика:                                  //
  if (apds.init()) {                                          // Если инициализация прошла успешно, то ...
    Serial.println("Initialization OK!");                 // Выводим сообщение об успешной инициализации датчика
  } else {
    Serial.println("Initialization ERROR!"); // Иначе, выводим сообщение оо ошибке инициализации датчика
  }
  //
  //  Устанавливаем коэффициент усиления приёмника:               // Доступные значения: 1х, 2х, 4х, 8х (GGAIN_1X, GGAIN_2X, GGAIN_4X, GGAIN_8X). Чем выше коэффициент тем выше чувствительность
  if (apds.setGestureGain(GGAIN_2X)) {                        // Если установлен коэффициент усиления приёмника в режиме обнаружения жестов, то ...
    Serial.println("Set gain OK!");                       // Выводим сообщение об успешной установке коэффициента усиления приёмника
  } else {
    Serial.println("Set gain ERROR!"); // Иначе, выводим сообщение об ошибке при установке коэффициента усиления приёмника
  }
  // Прочитать установленный коэффициент усиления приёмника можно так: uint8_t i = apds.getGestureGain(); // в переменную i сохранится значение: GGAIN_1X, или GGAIN_2X, или GGAIN_4X, или GGAIN_8X
  //  Устанавливаем силу тока драйвера ИК-светодиода:             // Доступные значения: 100мА, 50мА, 25мА, 12.5мА (LED_DRIVE_100MA, LED_DRIVE_50MA, LED_DRIVE_25MA, LED_DRIVE_12_5MA). Чем выше сила тока, тем выше чувствительность.
  if (apds.setGestureLEDDrive(LED_DRIVE_100MA)) {             // Если устанавлена сила тока драйвера (яркость) ИК-светодиода для обнаружения жестов, то ...
    Serial.println("Set LED drive OK!");                  // Выводим сообщение об успешной установке силы тока драйвера
  } else {
    Serial.println("Set LED drive ERROR!"); // Иначе, выводим сообщение об ошибке при установке силы тока драйвера
  }
  // Прочитать установленную силу тока можно так: uint8_t i = apds.getGestureLEDDrive(); // в переменную i сохранится значение: LED_DRIVE_100MA, или LED_DRIVE_50MA, или LED_DRIVE_25MA, или LED_DRIVE_12_5MA
  //  Разрешаем режим обнаружение жестов:                         //
  if (apds.enableGestureSensor(false)) {                      // Если механизм обнаружения жестов (false - без прерываний на выходе INT) запущен, то ...
    Serial.println("Start gesture sensor OK!");           // Выводим сообщение об успешном запуске механизма обнаружения жестов
  } else {
    Serial.println("Start gesture sensor ERROR!"); // Иначе, выводим сообщение об ошибке запуска механизма обнаружения жестов
  }
  // Запретить работу механизма обнаружения жестов можно так: bool j = apds.disableGestureSensor(); // в переменную j сохранится результат выполнения функции (true/false)
  //  Ждём завершение инициализации и калибровки:                 //
  delay(500);                                                 //
}                                                               //
//
void loop() {                                                   //
  //  Выводим название зафиксированного жеста:                    //
  if (apds.isGestureAvailable()) {                            // Если зафиксировано движение, то ...
    switch (apds.readGesture()) {                           // Сверяем значение соответствующее жесту ...
      case DIR_UP:    Serial.println("UP"   ); break;     // Зафиксировано движение вперёд или вверх (зависит от положения датчика)
      case DIR_DOWN:  Serial.println("DOWN" ); break;     // Зафиксировано движение назад или вниз (зависит от положения датчика)
      case DIR_LEFT:  Serial.println("LEFT" ); break;     // Зафиксировано движение влево
      case DIR_RIGHT: Serial.println("RIGHT"); break;     // Зафиксировано движение вправо
      case DIR_NEAR:  Serial.println("NEAR" ); break;     // Зафиксировано движение к датчику
      case DIR_FAR:   Serial.println("FAR"  ); break;     // Зафиксировано движение от датчика
      default:        Serial.println("NONE" ); break;     // Зафиксировано движение, но жест не опознан
    }                                                       //
  }                                                           //
  //
  //  Приостанавливаем выполнение скетча на 0,1 секунду:          //
  delay(100);                                                 // Чтоб не перегружать шину I2C постоянными запросами
}