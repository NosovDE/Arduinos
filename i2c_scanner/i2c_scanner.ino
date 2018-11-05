#include <Wire.h>
#include <Arduino.h>
 
long speed[] = { 
  50, 100, 200, 250, 400, 500, 800 };
const int speeds = sizeof(speed)/sizeof(speed[0]);
 
#define RESTORE_LATENCY  5    
bool delayFlag = false;
 
bool printAll = true;
bool header = true;
 
enum states {
  STOP, ONCE, CONT, HELP };
states state = STOP;
 
uint32_t startScan;
uint32_t stopScan;
 
void setup() 
{
  Serial.begin(115200);
  Wire.begin();
  displayHelp();
}
 
 
void loop() 
{
  switch (getCommand())
  {
  case 's': 
    state = ONCE; 
    break;
  case 'c': 
    state = CONT; 
    break;
  case 'd': 
    delayFlag = !delayFlag;
    Serial.print(F("<delay="));
    Serial.println(delayFlag?F("5>"):F("0>"));
    break;
  case 'e': 
 
    break;
  case 'h': 
    header = !header;
    Serial.print(F("<header="));
    Serial.println(header?F("yes>"):F("no>"));
    break;
  case '?': 
    state = HELP; 
    break;
  case 'p': 
    printAll = !printAll;
    Serial.print(F("<print="));
    Serial.println(printAll?F("all>"):F("found>"));
    break;
  case 'q': 
    state = HELP; 
    break;
  default:
    break;
  }
 
  switch(state)
  {
  case ONCE: 
    I2Cscan(); 
    state = HELP;
    break;
  case CONT:
    I2Cscan();
    delay(1000);
    break;    
  case HELP:
    displayHelp();
    state = STOP;
    break;
  case STOP:
    break;
  default: 
    break;
  }
}
 
char getCommand()
{
  char c = '\0';
  if (Serial.available())
  {
    c = Serial.read();
  }
  return c;
}
 
void displayHelp()
{
  Serial.println(F("\nArduino I2C Scanner - 0.1.03\n"));
  Serial.println(F("\ts = single scan"));
  Serial.println(F("\tc = continuous scan - 1 second delay"));
  Serial.println(F("\tq = quit continuous scan"));
  Serial.println(F("\td = toggle latency delay between successful tests."));
  Serial.println(F("\tp = toggle printAll - printFound."));
  Serial.println(F("\th = toggle header - noHeader."));
  Serial.println(F("\t? = help - this page"));
  Serial.println();
}
 
 
void I2Cscan()
{
  startScan = millis();
  uint8_t count = 0;
 
  if (header)
  {
    Serial.print(F("TIME\tDEC\tHEX\t"));
    for (uint8_t s = 0; s < speeds; s++)
    {
      Serial.print(F("\t"));
      Serial.print(speed[s]);
    }
    Serial.println(F("\t[KHz]"));
    for (uint8_t s = 0; s < speeds + 5; s++)
    {
      Serial.print(F("--------"));
    }
    Serial.println();
  }
  for (uint8_t address = 8; address < 120; address++)
  {
    bool printLine = printAll;
    bool found[speeds];
    bool fnd = false;
 
    for (uint8_t s = 0; s < speeds ; s++)
    {
      TWBR = (F_CPU/(speed[s]*1000) - 16)/2;
      Wire.beginTransmission (address);
      found[s] = (Wire.endTransmission () == 0);
      fnd |= found[s];
      // give device 5 millis
      if (fnd && delayFlag) delay(RESTORE_LATENCY);
    }
 
    if (fnd) count++;
    printLine |= fnd;
 
    if (printLine)
    {
      Serial.print(millis());
      Serial.print(F("\t"));
      Serial.print(address, DEC);
      Serial.print(F("\t0x"));
      Serial.print(address, HEX);
      Serial.print(F("\t"));
 
      for (uint8_t s = 0; s < speeds ; s++)
      {
        Serial.print(F("\t"));
        Serial.print(found[s]? F("V"):F("."));
      }
      Serial.println();
    }
  }
 
  stopScan = millis();
  if (header)
  {
    Serial.println();
    Serial.print(count);
    Serial.print(F(" devices found in "));
    Serial.print(stopScan - startScan);
    Serial.println(F(" milliseconds."));
  }
}






















/*

// --------------------------------------
// i2c_scanner
//
// Version 1
//    This program (or code that looks like it)
//    can be found in many places.
//    For example on the Arduino.cc forum.
//    The original author is not know.
// Version 2, Juni 2012, Using Arduino 1.0.1
//     Adapted to be as simple as possible by Arduino.cc user Krodal
// Version 3, Feb 26  2013
//    V3 by louarnold
// Version 4, March 3, 2013, Using Arduino 1.0.3
//    by Arduino.cc user Krodal.
//    Changes by louarnold removed.
//    Scanning addresses changed from 0...127 to 1...119,
//    according to the i2c scanner by Nick Gammon
//    http://www.gammon.com.au/forum/?id=10896
// Version 5, March 28, 2013
//    As version 4, but address scans now to 127.
//    A sensor seems to use address 120.
// Version 6, November 27, 2015.
//    Added waiting for the Leonardo serial communication.
//
//
// This sketch tests the standard 7-bit addresses
// Devices with higher bit address might not be seen properly.
//

#include <Wire.h>


void setup()
{
  Wire.begin();

  Serial.begin(9600);
  while (!Serial);             // Leonardo: wait for serial monitor
  Serial.println("\nI2C Scanner");
}


void loop()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for (address = 1; address < 127; address++ )
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address, HEX);
      Serial.println("  !");

      nDevices++;
    }
    else if (error == 4)
    {
      Serial.print("Unknow error at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.println(address, HEX);
    }
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(5000);           // wait 5 seconds for next scan
}
*/
