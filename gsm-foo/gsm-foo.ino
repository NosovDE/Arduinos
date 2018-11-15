/// GSM реле, для GSM\GPRS shield SIM900

//// как подключен модем?
#include <SoftwareSerial.h>                 // если программный
SoftwareSerial gsm(3, 2); // RX, TX
//#define gsm Serial                           // если аппаратный в UNO
#define gsm Serial1                          // если аппаратный в леонардо

#define LED 13

#define TELMODE "79216442023"                // !! номер с которого принимаем команду
#define relay A2                             // !! выход на реле

#define pinBOOT 9                            // нога BOOT или K на модеме M590, на SIM900 это 9
/////////////////////////


#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED











void setup() {


  u8g2.begin();

  //u8g2.setFont(u8g2_font_inb30_mr);  // set the target font to calculate the pixel width
  //  width = u8g2.getUTF8Width(text);    // calculate the pixel width of the text
  u8g2.enableUTF8Print();
  //  u8g2.setFontMode(0);    // enable transparent mode, which is faster



  delay(1000);                              //// !! чтобы нечего не повисало при включении

  gsm.begin(115200);                           /// незабываем указать скорость работы UART модема

  // Serial.begin(9600);

  pinMode(LED, OUTPUT);
  pinMode(pinBOOT, OUTPUT);                  /// нога BOOT на модеме
  pinMode(relay, OUTPUT);                    /// нога для реле

  delay(1000);
  digitalWrite(LED, HIGH);                   //  включаем лед
  ///// включаем модем,
  gsm.println("AT");                         //  смотрим есть ли ответ от модема
  delay(100);
  if (!gsm.find("OK")) {                     // если нет, дергаем ногу включения
    digitalWrite(pinBOOT, HIGH);
    delay(1000);
    digitalWrite(pinBOOT, LOW);
  }
  show("AT OK!");
  // нужно дождатся включения модема и соединения с сетью
  delay(2000);

  gsm.println("ATE0");                       // выключаем эхо

  while (1) {                                // ждем подключение модема к сети
    gsm.println("AT+COPS?");
    if (gsm.find("+COPS: 0")) break;
    digitalWrite(LED, LOW);               // блымаем светодиодом
    delay(50);
    digitalWrite(LED, HIGH);
    delay(500);
  }

  //Serial.println("Modem OK");
  digitalWrite(LED, LOW);               // блымаем светодиодом
  delay(1500);
  digitalWrite(LED, HIGH);
  delay(250);
  digitalWrite(LED, LOW);

}

void loop() {

  if (gsm.find("RING")) {                  // если нашли RING
    // Serial.println("RING");
    show("RING");
    while (1) {                            // в цикле
      if (gsm.find(TELMODE)) {              // ищим номер телефона, если нашли
        digitalWrite(relay, !digitalRead(relay));  // переключаем реле
        Serial.print("relay:");
        Serial.println(digitalRead(relay));
        show("RELAY");
        break;                              // и выходим
      }
      else {                                // иначе
        gsm.println("AT+CPAS");             // спрашиваем состояние модема
        delay(50);
        if (gsm.find("+CPAS: 0")) break;    // и если он в "готовности", выходим из цикла
        //   если звонок в процессе, возвращает +CPAS: 3
        gsm.println("AT+CLCC");            // даем команду для АОН, нужно для SIM900
        // и крутимся дальше
      }
    }

    gsm.println("ATH0");                 // сбрасываем вызов

    digitalWrite(LED, LOW);             // сигнализируем об этом
    delay(500);
    digitalWrite(LED, HIGH);
    delay(250);
    digitalWrite(LED, LOW);

  }

}


void show(String str) {
  u8g2.setFont(u8g2_font_inb30_mr);  //
  u8g2.setFontDirection(0);
  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 30);
    u8g2.print(str);
  } while ( u8g2.nextPage() );
}
