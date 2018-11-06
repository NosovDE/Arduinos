#define LS_PIN A2  //Light Sensor pin
#define R_PIN  9   //Red pin
#define G_PIN  10  //Green pin
#define B_PIN  11  //Blue pin
#define POWER  4   /* Power to raise the ratio of read-value to largest read-value. 
                      Higher is less likely to be on during the day,
                      but too high and it may not turn on once it's night.*/

#define TIME  500 /* The amount of milliseconds before taking a step through the color function.
                     Higher values = slower transitions between colors.*/

long  _max =  0;  //Holds the largest brightness reading so far.
int      count = 0;  //Keeps track of where we are on the color spectrum
float     ratio = 0;  //Stores the ratio of current brightness to largest recorded brightness.






#define LEDPIN 11         //LED brightness (PWM) writing
#define LIGHTSENSORPIN A1 //Ambient light sensor reading 



#include <HTU21D.h>
/*
  HTU21D(resolution)
  resolution:
  HTU21D_RES_RH12_TEMP14 - RH: 12Bit, Temperature: 14Bit, by default
  HTU21D_RES_RH8_TEMP12  - RH: 8Bit,  Temperature: 12Bit
  HTU21D_RES_RH10_TEMP13 - RH: 10Bit, Temperature: 13Bit
  HTU21D_RES_RH11_TEMP11 - RH: 11Bit, Temperature: 11Bit
*/
HTU21D myHTU21D(HTU21D_RES_RH12_TEMP14);



#include <Arduino.h>
#include <U8g2lib.h>

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

U8G2_SSD1306_128X32_UNIVISION_1_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);   // Adafruit ESP8266/32u4/ARM Boards + FeatherWing OLED

u8g2_uint_t offset;			// current offset for the scrolling text
u8g2_uint_t width;			// pixel width of the scrolling text (must be lesser than 128 unless U8G2_16BIT is defined
const char *text = "U8g2 ";	// scroll this text from right to left

void setup(void) {
  pinMode(LS_PIN,  INPUT);  //Reading in
  pinMode(R_PIN,  OUTPUT);  //Writing out*
  pinMode(G_PIN,  OUTPUT);  //*
  pinMode(B_PIN,  OUTPUT);  //*

  pinMode(LIGHTSENSORPIN,  INPUT);
  pinMode(LEDPIN, OUTPUT);

  u8g2.begin();

  //u8g2.setFont(u8g2_font_inb30_mr);	// set the target font to calculate the pixel width
  width = u8g2.getUTF8Width(text);		// calculate the pixel width of the text
  u8g2.enableUTF8Print();
  //  u8g2.setFontMode(0);		// enable transparent mode, which is faster

  while (myHTU21D.begin() != true)
  {
    Serial.println(F("HTU21D, SHT21 sensor is faild or not connected")); //(F()) saves string to flash & keeps dynamic memory free
    delay(5000);
  }
}

void loop(void) {
  
  float t = myHTU21D.readTemperature();

  float reading2 = analogRead(LIGHTSENSORPIN); //Read light level
  float square_ratio = reading2 / 1023.0;      //Get percent of maximum value (1023)
  square_ratio = pow(square_ratio, 2.0);      //Square to make response more obvious

float volts = reading2 * 5.0 / 1024.0;
float amps = volts / 10000.0;  // across 10,000 Ohms
float microamps = amps * 1000000;
float lux = microamps * 2.0;
  //float lux = reading2 * 0.9765625;  // 1000/1024


  u8g2.setFont(u8g2_font_inb30_mr);  //
  u8g2.setFontDirection(0);

  //for (long i = 0; i < 1; i++) {
  u8g2.firstPage();
  do {
    u8g2.setCursor(0, 30);
    u8g2.print(String(lux));
  } while ( u8g2.nextPage() );

  delay(1050);             // do some small delay
  //}

  float reading = analogRead(LS_PIN); //Take brightness reading
  if (reading > _max) _max = reading; //See if it's the brightest so far

  /* Calculate the ratio between the reading and the max, raise the power, and average it with
     the last reading to get our ratio but minimize flickering. */
  ratio = (ratio + pow(1.0 - reading / float(_max), POWER)) / 2.0;
  /* We're subtracting the ratio from 1 because we want brighter readings to result
     in a dimmer display of colors, and smaller readings to result in brighter displays. */

  //Write the color from our spot in the spectrum at the relative brightness to the LED
  analogWrite(R_PIN, Rainbow(count + 170) * ratio);
  analogWrite(G_PIN, Rainbow(count + 85)  * ratio);
  analogWrite(B_PIN, Rainbow(count)       * ratio);
  /* Each color has the same pattern of increasing and decreasing brightness, just not synchronously.
     We can reuse the same function and carefully offset each color to make sure we get to see all
     the colors.*/


  if (!(millis() % TIME)) count++;
  /* There's a lot going on in this concise use of syntax. Essentially, whenever millis() (the
     amount of milliseconds since the arduino has been on) is evenly divisble by TIME (i.e.
     TIME milliseconds have passed) then the modulo operator (%) will return 0, which evaluates
     as FALSE. The ! in front of the expression means invert, so this means every TIME milliseconds,
     the expression will return TRUE. If it's TRUE, we take a step through the color spectrum. Bytes
     can only count to 255 before they reset back to 0. The overflow of "count" is a passive way to
     take us back to the start of the color spectrum without having to explicitly set "count" to 0.*/

}

/* Returns an appropriate value in 0-255 for a given value in 0-255. With good timing, it can be used
 * to cycle through the entire color spectrum. */
byte Rainbow(byte i) {
  if (i > 213) return i * 6;  
  if (i > 127) return 0;
  if (i > 85)  return 255 - i * 6;  
  return 255;
}
