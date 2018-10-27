#include <OLED_I2C.h>

#include <SPI.h>
#include <RF24.h>
//#include <OLED_I2C.h>


RF24 radio(9, 10); // можно использовать любые
const uint32_t pipe = 123456789; // адрес


OLED  myOLED(SDA, SCL, 8);

//extern uint8_t SmallFont[];
extern uint8_t RusFont[];
//extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];

void setup()
{
  Serial.begin(9600);

  myOLED.begin();
  radio.begin();
  radio.setDataRate(RF24_250KBPS); // скорость обмена данными RF24_1MBPS или RF24_2MBPS
  //radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_MAX);

  radio.openWritingPipe (0xAABBCCDD11LL); // Arduino Transmitter 1.
 // radio.openWritingPipe (0xAABBCCDD22LL); // Arduino Transmitter 2.
 // radio.openWritingPipe (0xAABBCCDD33LL); // Arduino Transmitter 3.
  //radio.openWritingPipe (0xAABBCCDD44LL); // Arduino Transmitter 4.
  delay(150);
}

long count = 0;

void loop()
{
  count = random(10000, 99999); // (count + 10);

  radio.write(&count, sizeof(count));
  Serial.println("Generate: " + (String)count);

  // Clear screen
  myOLED.clrScr();

  myOLED.setFont(RusFont);
  myOLED.print(F("Utythbhetv "), LEFT, 4);
  myOLED.setFont(BigNumbers);
  myOLED.printNumI(count, RIGHT, 30);
  myOLED.update();

  delay(300);
  count = count + 100;
}
