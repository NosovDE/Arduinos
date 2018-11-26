#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     0 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);




// DHT датчик температуры и влажности
#include <Adafruit_Sensor.h>
#include <DHT.h>
//#include <DHT_U.h>

#define DHTPIN D4     // what digital pin we're connected to
// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);
//float hum = 0;
//float tmp = 0;
String temperature = "--";
String humidity = "--";

#include "RTClib.h"
RTC_DS1307 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


uint16_t tmp_timer;
uint16_t hum_timer;

void setup() {
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    //Serial.println(F("SSD1306 allocation failed"));
    for (;;); // Don't proceed, loop forever
  }
  dht.begin();

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (! rtc.isrunning()) {
    // Serial.println("RTC is NOT running!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    // rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0));
  }
}

void loop() {
  DateTime now = rtc.now();
  String t = getTemperature();
  String h = getHumidity();
  draw(now, ":", t, h);
  delay(500);
  draw(now, " ", t, h);
  delay(500);

}


/**
   Update OLED display
*/
void draw(DateTime now, String delim, String temperature, String humidity) {
  display.clearDisplay();
  // draw UI
  display.drawRoundRect(0, 0, 127, 15, 5, WHITE);
  display.drawRoundRect(0, 0, 127, 63, 5, WHITE);
  display.drawLine(0, 40, 127, 40, WHITE);

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setCursor(5, 4);

  display.println(String(now.day(), DEC) + "." + String(now.month(), DEC) + "." + String(now.year(), DEC) + "  " + daysOfTheWeek[now.dayOfTheWeek()]);

  char min[5];

  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setCursor(90, 28);
  sprintf(min, ":%02d", now.second());
  display.println(min);

  display.setTextSize(2);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(25, 20);            // Start at top-left corner
  sprintf(min, "%02d:%02d",now.hour(), now.minute());
  display.println(min);



  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setCursor(5, 50);            // Start at top-left corner
  display.println("T: " + temperature);
  display.setCursor(70, 50);            // Start at top-left corner
  display.println("H: " + humidity);

  display.display();
}

/**
   Get Temperature
*/
String getTemperature() {
  if (millis() - tmp_timer > 2000 || temperature == "--") {
    int16_t tmp = ceil(dht.readTemperature());

    if (isnan(tmp) || tmp <= 0  ) {
      return temperature;
    }
    tmp_timer = millis();    // сбросить таймер
    return String(tmp) + F("C");
  }
  return temperature;
}

/**
   Get Humidity
*/
String getHumidity() {
  if (millis() - hum_timer > 2000 || humidity == "--") {
    int16_t hum = ceil(dht.readHumidity());

    if (isnan(hum) || hum <= 0 ) {
      return humidity;
    }
    hum_timer = millis();    // сбросить таймер
    return String(hum) + F("%");
  }
  return humidity;
}
