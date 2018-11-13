#include <iarduino_IR_RX.h>                          // Подключаем библиотеку для работы с ИК-приёмником
iarduino_IR_RX IR(6);                                // Объявляем объект IR, с указанием вывода к которому подключён ИК-приёмник
                                                     //
bool flgKey1 = false; uint32_t codKey1 = 0xFF30CF;   // Определяем флаг нажатия и код кнопки 1
bool flgKey2 = false; uint32_t codKey2 = 0xFF18E7;   // Определяем флаг нажатия и код кнопки 2
bool flgKey3 = false; uint32_t codKey3 = 0xFF7A85;   // Определяем флаг нажатия и код кнопки 3
bool flgKey  = false; uint32_t tmrKey  = 0;          // Определяем флаг разрещающий вывод данных в монитор и время последнего нажатия кнопки.
                                                     //
void setup(){                                        //
  Serial.begin(9600);                                // Инициируем передачу данных в монитор последовательного порта, на скорости 9600 бит/сек
  IR.begin();                                        // Инициируем работу с ИК-приёмником
}                                                    //
                                                     //
void loop(){                                         //
  if(IR.check(true)){                                // Если в буфере имеются данные, принятые с пульта (удерживается кнопка), то ...
    if(millis()-200 > tmrKey){                       // Если с последней поступившей команды прошло более 200 мс, то
      flgKey1=false;                                 // Считаем что кнопка 1 не удерживается
      flgKey2=false;                                 // Считаем что кнопка 2 не удерживается
      flgKey3=false;                                 // Считаем что кнопка 3 не удерживается
    } tmrKey = millis();              flgKey=true;   // Сохраняем время последней реакции на пульт и азрешаем вывод данных
    if(IR.data==codKey1){ if(flgKey1){flgKey=false;} flgKey1=true; }else{flgKey1=false;} // Запрещаем вывод данных кнопки 1 при её удержании
    if(IR.data==codKey2){ if(flgKey2){flgKey=false;} flgKey2=true; }else{flgKey2=false;} // Запрещаем вывод данных кнопки 2 при её удержании
    if(IR.data==codKey3){ if(flgKey3){flgKey=false;} flgKey3=true; }else{flgKey3=false;} // Запрещаем вывод данных кнопки 3 при её удержании
    if(flgKey){                                      // Если вывод данных разрешен, то ...
      Serial.println(IR.data, HEX);                  // Выводим код нажатой кнопки
      Serial.println(IR.length   );                  // Выводим количество бит в коде
    }                                                //
  }                                                  //
}                                                    //
