/*
 * oled-menu-pages
 * based on Corridor 2 Hardware Rev. B
 * 
 * Made by @Schlueri 2017
 * 
 * Menu and display pages
 * 
 */

// ========================================================
// INCLUDES
// ========================================================

// for display
#include <U8g2lib.h>
// for timer
#include <TimerOne.h>
// for rotary encoder
#include <Rotary.h>

// ========================================================
// DEBUG
// ========================================================

//#define DEBUG
#define DEBUG_BAUD 9600

// ========================================================
// DEFAULTS
// ========================================================

#define VERSION_TITLE "OLED MENU PAGES"
#define VERSION_HW "Corridor 2 Rev. B"
#define VERSION_SUBTITLE "by @schlueri 2017"
#define VERSION "0.32B"

// Status LED
#define STATUS_LED_PIN 9
bool statusLedOn = false;

// Timer
#define TIMER 1000

// Display and menu stuff
int displayCurrentPage = 0;
bool setNeedsDisplay = false;

// Main menu fixed to 3 items, left, center, right...
#define MENU_SELECTED_TIMEOUT 4000
#define MENU_POS_Y 62
#define MENU_POS_Y_HIDDEN 76
#define MENU_ANIMATION_PIXEL_STEP 2
String menuItems[3] = {"MAIN", "NETWORK", "SETUP"};
int menuActive = 1;             // left active
int menuSelected = menuActive;  // selected
bool menuPageMode = false;      // true => rotary encoder control page and not menu

// Menu animation
bool menuAnimationRunning = false;
int menuPosY = MENU_POS_Y;

// Rotary encoder with switch
#define ROTARY_SWITCH 15  // A1
#define ROTARY_PIN1 4
#define ROTARY_PIN2 2
#define ROTARY_ACCEL_OFFSET1 20
#define ROTARY_ACCEL_OFFSET2 50
#define ROTARY_ACCEL_OFFSET3 70
unsigned long rotaryLastMove;
bool rotaryButtonPressed = false;

// Action button
//#define BUTTON1_PIN 16    // A2

// Test slider
int sliderPosX = 64;

// Logic
long int heartbeat = 0;
#define HEARTBEAT_TRIGGER 1000
#define HEARTBEAT_TRIGGER_TIME 50

// ========================================================
// PAGES STUFF
// ========================================================

#define SETUP_MENU_ITEMS 8
String setupMenuItems[SETUP_MENU_ITEMS] = {"EXIT", "LDR LEVEL", "LDR THRESHOLD", "PIR SENSOR", "LED COLOR", "LED ANIMATION", "LED OUT TEST", "VERSION & INFO"};
int setupMenuSelected = 0;

// ========================================================
// INITS
// ========================================================

// Display
U8G2_SH1106_128X64_VCOMH0_1_4W_HW_SPI u8g2(U8G2_R0, 10, 8);
// Rotary Encoder
Rotary rotary = Rotary(ROTARY_PIN1, ROTARY_PIN2);

// ========================================================
// SETUP
// ========================================================
 
void setup() {
  // put your setup code here, to run once:

  // Status LED
  pinMode(STATUS_LED_PIN, OUTPUT);
  digitalWrite(STATUS_LED_PIN, HIGH);

#ifdef DEBUG
  Serial.begin(DEBUG_BAUD);
  Serial.println("DEBUG MODE");
  Serial.print("Version ");
  Serial.print(VERSION);
  Serial.print("\n\n");
#endif

  // Menu Button
  pinMode(ROTARY_SWITCH, INPUT);
  digitalWrite(ROTARY_SWITCH, INPUT_PULLUP);

#ifdef BUTTON1_PIN
  pinMode(BUTTON1_PIN, INPUT);
  digitalWrite(BUTTON1_PIN, INPUT_PULLUP);
#endif

  // OLED Display
  u8g2.begin();

  setNeedsDisplay = true;

  Timer1.initialize(TIMER);
  Timer1.attachInterrupt(timerEvent);

  //delay(1000);
}

// ========================================================
// LOOP
// ========================================================

void loop() {
  // put your main code here, to run repeatedly:

  digitalWrite(STATUS_LED_PIN, statusLedOn);

  if (menuAnimationRunning) {
    if (menuPageMode && menuPosY < MENU_POS_Y_HIDDEN) {
      // do animation
      menuPosY = menuPosY + MENU_ANIMATION_PIXEL_STEP;
      setNeedsDisplay = true;
    }
    if (!menuPageMode && menuPosY > MENU_POS_Y) {
      // do animation
      menuPosY = menuPosY - MENU_ANIMATION_PIXEL_STEP;
      setNeedsDisplay = true;
    }
  }
  if (menuAnimationRunning && (menuPosY == MENU_POS_Y || menuPosY == MENU_POS_Y_HIDDEN)) {
    // looks like animation is done
    menuAnimationRunning = false;
  }

  if (setNeedsDisplay) {
    noInterrupts();
    displayRenderCurrentPage();
    setNeedsDisplay = false;
    interrupts();
  }

}

// ========================================================
// TIMER
// ========================================================

void timerEvent() {
  // Heartbeat
  if (heartbeat > HEARTBEAT_TRIGGER) {
    statusLedOn = true;
  }
  if (heartbeat > HEARTBEAT_TRIGGER + HEARTBEAT_TRIGGER_TIME) {
    statusLedOn = false;
    heartbeat = 0;
  }
  heartbeat++;

  // Menu logic
  unsigned long timeOffset = millis() - rotaryLastMove;
  if (timeOffset > MENU_SELECTED_TIMEOUT) {
    // deselect menu
    menuSelected = menuActive;
    rotaryLastMove = millis();
    setNeedsDisplay = true;
  }
  
  // Rotary Encoder
  unsigned char result = rotary.process();
  if (result) {

    if (!menuPageMode) {
      if (result == DIR_CW) {
        // right
        if (menuSelected < 3) {
          menuSelected++;
        }
      } else {
        // left
        if (menuSelected > 1) {
          menuSelected--;
        }
      }
      setNeedsDisplay = true;
    } else {

      // Acceleration
      byte acceleration = 1;
      unsigned long timeOffset = millis() - rotaryLastMove;
      
      //Serial.println(timeOffset);

      if (displayCurrentPage == 0 || displayCurrentPage == 1) {
        if (timeOffset < ROTARY_ACCEL_OFFSET1) {
          acceleration = 16;
        } else if (timeOffset < ROTARY_ACCEL_OFFSET2) {
          acceleration = 4;
        } else if (timeOffset < ROTARY_ACCEL_OFFSET3) {
          acceleration = 2;
        }
      
        // Development test => control slider
        if (result == DIR_CW) {
          // right
          if (sliderPosX < 128) {
            sliderPosX = sliderPosX + acceleration;
          }
        } else {
          // left
          if (sliderPosX > 0) {
            sliderPosX = sliderPosX - acceleration;
          }
        }
        setNeedsDisplay = true;
      }

      if (displayCurrentPage == 2) {
        if (result == DIR_CW) {
          // right
          setupMenuSelected++;
        } else {
          // left
          setupMenuSelected--;
        }
        if (setupMenuSelected > SETUP_MENU_ITEMS - 1) {
          setupMenuSelected = SETUP_MENU_ITEMS - 1;
        }
        if (setupMenuSelected < 1) {
          setupMenuSelected = 0;
        }
        setNeedsDisplay = true;
      }

    }

    rotaryLastMove = millis();
    
  }

  // Rotary button
  if (buttonEvent()) {
    rotaryLastMove = millis();
    if (menuActive == menuSelected) {
      if (!menuPageMode) {
        // give controls to page (button press on selected page)
        menuPageMode = true;
        menuAnimationRunning = true;
        
        sliderPosX = 64;
        
        setNeedsDisplay = true;
        
        #ifdef DEBUG
        Serial.println("PAGE MODE ON");
        #endif
      } else {
        menuPageMode = false;   
        menuAnimationRunning = true;
        setNeedsDisplay = true;
        sliderPosX = 64;

        setupMenuSelected = 0;
        
        #ifdef DEBUG
        Serial.println("PAGE MODE OFF"); 
        #endif
      }
    }
    if (!menuPageMode) {
      menuActive = menuSelected;
      if (menuActive == 1) {
        displayCurrentPage = 0;
      }
      if (menuActive == 2) {
        displayCurrentPage = 1;
      }
      if (menuActive == 3) {
        displayCurrentPage = 2;
      }
      setNeedsDisplay = true;
    }
  }

  // Action button => reset page mode during development
#ifdef BUTTON1_PIN
  if (digitalRead(BUTTON1_PIN) == 0 && menuPageMode) {
    menuPageMode = false;   
    menuAnimationRunning = true;
    setNeedsDisplay = true;
    sliderPosX = 64;
    #ifdef DEBUG
    Serial.println("PAGE MODE OFF"); 
    #endif
  }
#endif
  
}

// ========================================================
// MENU BUTTON
// ========================================================

bool buttonEvent() {
  bool result = false;
  bool menuButton = false;
  if (digitalRead(ROTARY_SWITCH) == 1) {
    menuButton = true;
  }
  if (menuButton && !rotaryButtonPressed) {
    rotaryButtonPressed = true;
  } else if (!menuButton && rotaryButtonPressed) {
    rotaryButtonPressed = false;
    result = true;
    // FIXME: debounce for try, check if it's really needed
    //delay(4);
  }
  return result;
}

// ========================================================
// DISPLAY - Screen Drawing
// ========================================================

void displayRenderCurrentPage() {
  // OLED Display update
  u8g2.firstPage();
  do {

    if (displayCurrentPage == 0) {
      u8g2.setFont(u8g2_font_8x13B_tr);
      u8g2.drawStr(0, 12, "Main Page");
    }

    if (displayCurrentPage == 1) {
      u8g2.setFont(u8g2_font_8x13B_tr);
      u8g2.drawStr(0, 12, "Network Page");
    }

    if (displayCurrentPage == 2) {
      if (!menuPageMode) {
        u8g2.setFont(u8g2_font_8x13B_tr);
        u8g2.drawStr(0, 10, "Setup Page");
        u8g2.setFont(u8g2_font_5x7_tr);
        u8g2.drawStr(0, 28, "PRESS BUTTON FOR SUBMENU");
      } else {
        drawPageMenu();

        if (setupMenuSelected == 0) {
          u8g2.setFont(u8g2_font_5x7_tr);
          u8g2.drawStr(0, 28, "DEMO MODE");
          u8g2.drawStr(0, 38, "PRESS BUTTON TO EXIT");
        }

        if (setupMenuSelected == 7) {
          // Version & Info
          u8g2.setFont(u8g2_font_5x7_tr);
          u8g2.setCursor(0, 28);
          u8g2.print(VERSION_TITLE);
          u8g2.print(" ");
          u8g2.print(VERSION);

          u8g2.setCursor(0, 38);
          u8g2.print(VERSION_SUBTITLE);

          u8g2.setCursor(0, 56);
          u8g2.print("HW: ");
          u8g2.print(VERSION_HW);
          

        }
      }
    }

    if (displayCurrentPage == 0 || displayCurrentPage == 1) {
      u8g2.setFont(u8g2_font_5x7_tr);
      if (menuPageMode) { 
        u8g2.drawStr(0, 28, "ROTARY CONTROL ON PAGE");
        u8g2.setCursor(0, 46);
        u8g2.print("VALUE ");
        u8g2.print(sliderPosX);
      } else {
        u8g2.drawStr(0, 28, "ROTARY CONTROL ON MENU");
      }
      drawSlider(31);
    }

    drawMenuBar();
    
  } while ( u8g2.nextPage() );
}

void drawPageMenu() {
  u8g2.setFont(u8g2_font_6x12_tr);
  if (displayCurrentPage == 2) {
    String text = setupMenuItems[setupMenuSelected];
    // center text
    int textWidth = u8g2.getStrWidth(text.c_str());
    int textX = (128 - textWidth) / 2;
    int textXPadding = 4;
    u8g2.drawRBox(textX - textXPadding, 0, textWidth + textXPadding + textXPadding, 11, 2);
    u8g2.setDrawColor(0);
    u8g2.setCursor(textX, 11 - 2);
    u8g2.print(text);
    u8g2.setDrawColor(1);

    bool drawLeftTriangle = false;
    bool drawRightTriangle = false;

    if (setupMenuSelected < SETUP_MENU_ITEMS - 1) {
      drawRightTriangle = true;
    }
    if (setupMenuSelected > 0) {
      drawLeftTriangle = true;
    }

    if (drawLeftTriangle) {
      // Triangle left
      u8g2.drawTriangle(4, 1, 4, 9, 0, 5);
    }
    if (drawRightTriangle) {
      // Triangle right
      u8g2.drawTriangle(128 - 5, 1, 128 - 5, 9, 127, 5);
    }
    u8g2.drawHLine(0, 14, 128);
  }
  
}

void drawSlider(int yPos) {
  u8g2.drawFrame(0, yPos, 128, 6);
  if (sliderPosX < 1) {
    sliderPosX = 0;
  }
  if (sliderPosX > 128) {
    sliderPosX = 128;
  }
  u8g2.drawVLine(sliderPosX, yPos, 6);
}

void drawMenuBar() {
  int textX = 0;
  int textY = menuPosY;
  int textWidth = 0;
  int textXPadding = 4;
  
  u8g2.setFont(u8g2_font_6x12_tr);
  u8g2.setDrawColor(1);

  u8g2.drawHLine(0, textY - 11 - 2, 128);

  if (textY < MENU_POS_Y_HIDDEN) {
    // center menu
    String text = menuItems[1];
    textWidth = u8g2.getStrWidth(text.c_str());
    textX = (128 - textWidth) / 2;
    if (menuActive == 2) {
      u8g2.drawRBox(textX - textXPadding, textY + 2 - 11, textWidth + textXPadding + textXPadding, 11, 2);
      u8g2.setDrawColor(0);
    } 
    if (menuActive != menuSelected && menuSelected == 2) {
      u8g2.drawRFrame(textX - textXPadding, textY + 2 - 11, textWidth + textXPadding + textXPadding, 11, 2);
      u8g2.setDrawColor(1);
    }
  
    u8g2.setCursor(textX, textY);
    u8g2.print(text);
    u8g2.setDrawColor(1);
  
    // left menu
    text = menuItems[0];
    textX = textXPadding;
    textWidth = u8g2.getStrWidth(text.c_str());
    if (menuActive == 1) {
      u8g2.drawRBox(textX - textXPadding, textY + 2 - 11, textWidth + textXPadding + textXPadding, 11, 2);
      u8g2.setDrawColor(0);
    } 
    if (menuActive != menuSelected && menuSelected == 1) {
      u8g2.drawRFrame(textX - textXPadding, textY + 2 - 11, textWidth + textXPadding + textXPadding, 11, 2);
      u8g2.setDrawColor(1);
    }
    u8g2.setCursor(textX, textY);
    u8g2.print(text);
    u8g2.setDrawColor(1);
  
    // right menu
    text = menuItems[2];
    textWidth = u8g2.getStrWidth(text.c_str());
    textX = 128 - textWidth - textXPadding;
    if (menuActive == 3) {
      u8g2.drawRBox(textX - textXPadding, textY + 2 - 11, textWidth + textXPadding + textXPadding, 11, 2);
      u8g2.setDrawColor(0);
    }
    if (menuActive != menuSelected && menuSelected == 3) {
      u8g2.drawRFrame(textX - textXPadding, textY + 2 - 11, textWidth + textXPadding + textXPadding, 11, 2);
      u8g2.setDrawColor(1);
    }
    u8g2.setCursor(textX, textY);
    u8g2.print(text);
    u8g2.setDrawColor(1);
  }
  
}

