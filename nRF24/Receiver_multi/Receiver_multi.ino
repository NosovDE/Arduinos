#include <OLED_I2C.h>

#include <SPI.h>
#include <RF24.h>

RF24 radio(9, 10);
OLED  myOLED(SDA, SCL, 8);

long receive = 0;
long receive2 = 0;
long receive3 = 0;

extern uint8_t SmallFont[];
//extern uint8_t RusFont[];
//extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];

void setup()
{
  myOLED.begin();
  radio.begin();
  radio.setDataRate(RF24_250KBPS); // RF24_1MBPS или RF24_2MBPS
  radio.setPALevel(RF24_PA_MAX);
  radio.openReadingPipe(1, 0xAABBCCDD11LL); // открыть канал на приём
  radio.openReadingPipe(2, 0xAABBCCDD22LL); // открыть канал на приём
  radio.openReadingPipe(3, 0xAABBCCDD33LL); // открыть канал на приём
  
  delay(50);

  radio.startListening(); // приём
  myOLED.setFont(SmallFont);
  myOLED.print("Receiver is start...", LEFT, 4);
  myOLED.print("----------------------------", CENTER, 14);
  myOLED.update( );
}

void loop()
{
  uint8_t pipeNum = 0;

  if (radio.available(&pipeNum))
  {
    switch (pipeNum) {
      case 1:
        radio.read(&receive, sizeof(receive));
        myOLED.print("Receiver1: " + (String)receive + "     ", LEFT, 25);
        break;

      case 2:
        radio.read(&receive2, sizeof(receive2));
        myOLED.print("Receiver2: " + (String)receive2 + "     ", LEFT, 35);
        break;

      case 3:
        radio.read(&receive3, sizeof(receive3));
        myOLED.print("Receiver3: " + (String)receive3 + "     ", LEFT, 45);
        break;
    }
    myOLED.update();
  }
}


