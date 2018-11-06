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
  u8g2.begin();

  //u8g2.setFont(u8g2_font_inb30_mr);	// set the target font to calculate the pixel width
  width = u8g2.getUTF8Width(text);		// calculate the pixel width of the text
  u8g2.enableUTF8Print();
  //  u8g2.setFontMode(0);		// enable transparent mode, which is faster
}

void loop(void) {

  u8g2.setFont(u8g2_font_inb30_mr);  //
  u8g2.setFontDirection(0);

  for (long i = 0; i < 100000; i++) {
    u8g2.firstPage();
    do {
      u8g2.setCursor(5, 30);
      u8g2.print(String(i));
    } while ( u8g2.nextPage() );

    delay(50);             // do some small delay
  }



}
