#include <OLED_I2C.h>
#include <SPI.h>
#include <RF24.h>


RF24 radio(9, 10);
const uint64_t pipe = 0xAABBCCDD11LL;

OLED  myOLED(SDA, SCL, 8);

long receive = 0;
int proc = 0;

extern uint8_t SmallFont[];
extern uint8_t RusFont[];
//extern uint8_t MediumNumbers[];
extern uint8_t BigNumbers[];

void setup()
{
  myOLED.begin();
  radio.begin();
  radio.setDataRate(RF24_250KBPS); // RF24_1MBPS или RF24_2MBPS
  radio.setPALevel(RF24_PA_MAX);
  radio.openReadingPipe(1, 0xAABBCCDD11LL); // открыть канал на приём
  delay(100);
  radio.startListening(); // приём
}

void loop()
{
  if (radio.available())
  {
    radio.read(&receive, sizeof(receive));
    // Clear screen
    myOLED.clrScr();
  
    myOLED.setFont(SmallFont);
    int pos = 7 + (proc % 19 * 6);
    myOLED.print("  \\\\\\\\   ", pos, 24);
    proc++;
    //myOLED.printNumI(massiv[0], RIGHT, 40);

    myOLED.setFont(RusFont);
    myOLED.print("Ghbyznj", LEFT, 4);
    myOLED.setFont(BigNumbers);

    myOLED.printNumI(receive, RIGHT, 40);
    myOLED.update();
  }
}


