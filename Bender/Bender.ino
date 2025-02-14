#include <SPI.h>
#include "GyverMAX7219.h"
#include "EEManager.h"
#include <FastLED.h>

#define MTRX_CS 10;
#define MTRX_DAT 11;
#define MTRX_CLK 13;

#define ANALYZ_WIDTH (3 * 8)


// data
MAX7219<5, 1, 10, 11, 13> mtrx;

struct Data {
  bool state = 0;
  int8_t vol = 10;
  int8_t bright_eyes = 0;
  int8_t bright_mouth = 0;
  uint16_t trsh = 50;
  uint8_t mode = 0;
  int8_t station = 0;
};

Data data;
EEManager memory(data);
bool pulse = 0;

// func
// ========================= MATRIX =========================
void upd_bright() {
  uint8_t m = data.bright_mouth, e = data.bright_eyes;
  uint8_t br[] = {m, m, m, e, e, 0, 0};
  mtrx.setBright(br);
}

void print_val(char c, uint8_t v) {
  mtrx.rect(0, 0, ANALYZ_WIDTH - 1, 7, GFX_CLEAR);
  mtrx.setCursor(8 * 0 + 2, 1);
  mtrx.print(c);
  mtrx.setCursor(8 * 1 + 2, 1);
  mtrx.print(v / 10);
  mtrx.setCursor(8 * 2 + 2, 1);
  mtrx.print(v % 10);
  mtrx.update();
}

// ========================= EYES =========================
void draw_eye(uint8_t i) {
  uint8_t x = ANALYZ_WIDTH + i * 8;
  mtrx.rect(1 + x, 1, 6 + x, 6, GFX_FILL);
  mtrx.lineV(0 + x, 2, 5);
  mtrx.lineV(7 + x, 2, 5);
  mtrx.lineH(0, 2 + x, 5 + x);
  mtrx.lineH(7, 2 + x, 5 + x);
}
void draw_eyeb(uint8_t i, int x, int y, int w = 2) {
  x += ANALYZ_WIDTH + i * 8;
  mtrx.rect(x, y, x + w - 1, y + w - 1, GFX_CLEAR);
}

void drawSearch() {
  static int8_t pos = 4, dir = 1;
  boolean tmr = true;
  if (tmr) {
    pos += dir;
    if (pos >= 6) dir = -1;
    if (pos <= 0) dir = 1;
    mtrx.rect(ANALYZ_WIDTH, 2, ANALYZ_WIDTH + 16 - 1, 5, GFX_FILL);
    draw_eyeb(0, pos, 3);
    draw_eyeb(1, pos, 3);
    mtrx.update();
  }
}


void drawSleepy() {
  mtrx.clear();
  if (random(10) % 2 > 1) {
    upd_bright();

    draw_eye(0);
    draw_eye(1);
    draw_eyeb(0, 2, 2, 4);
    draw_eyeb(1, 2, 2, 4);
  } else {
    mtrx.setBright((uint8_t)0);
    draw_eye(0);
    draw_eye(1);
    mtrx.rect(ANALYZ_WIDTH, 0, ANALYZ_WIDTH + 16 - 1, 3, GFX_CLEAR);
    draw_eyeb(0, 3, 5);
    draw_eyeb(1, 3, 5);
  }
  delay(500);
  mtrx.update();
}
// ========================= ANALYZ =========================
void analyz0(uint8_t vol) {
  static uint16_t offs;
  offs += 20 * vol / 100;
  for (uint8_t i = 0; i < ANALYZ_WIDTH; i++) {
    int16_t val = inoise8(i * 50, offs);
    val -= 128;
    val = val * vol / 100;
    val += 128;
    val = map(val, 45, 255 - 45, 0, 7);
    mtrx.dot(i, val);
  }
}
void analyzWave(uint8_t vol) {
  static uint8_t prevs[ANALYZ_WIDTH];
  for (uint8_t i = 0; i < ANALYZ_WIDTH - 1; i++) prevs[i] = prevs[i + 1];
  prevs[ANALYZ_WIDTH - 1] = 9 * vol / (100 + 1);
  for (uint8_t i = 0; i < ANALYZ_WIDTH; i++) {
    uint8_t mask = ((1 << prevs[i]) - 1) << ((8 - prevs[i]) >> 1);
    // 0-00000000-4
    // 1-00001000-3
    // 2-00011000-3
    // 3-00011100-2
    // 4-00111100-2
    // 5-00111110-1
    // 6-01111110-1
    // 7-01111111-0
    // 8-11111111-0
    for (uint8_t n = 0; n < 8; n++) {
      if (mask & 1) mtrx.dot(i, n);
      mask >>= 1;
    }
  }
}
/////////////////////////////////////////////////////////////////////

void setup(void) {
  mtrx.begin();
  upd_bright();
}


void loop(void) {


  // mtrx.clear();
  // mtrx.update();
  for (int i = 0; i < 5 + random(10); i++) {
    drawEye();
    analyz0(random(50));
    mtrx.update();


  }
  mtrx.clear();

  for (int i = 0; i < 25 + random(10); i++) {
    drawSearch();
    analyz0(random(3));
    mtrx.update();
    delay(100);

  }
  mtrx.clear();


  for (int i = 0; i < 35 + random(20); i++) {
    drawCrazy();
    analyz0(random(128));
    mtrx.update();
    delay(100);
    mtrx.clear();
  }

  /*
    for (int i = 0; i < 10; i++) {
      drawWave();
      analyzWave(random(98));
       mtrx.update();
      delay(100);
      mtrx.clear();
    }
  */

  /*
    mtrx.update();
    draw_eye(0);
    draw_eye(1);
    draw_eyeb(0, 3, 3);
    draw_eyeb(1, 3, 3);

    mtrx.update();

    delay(2000);
    /*
    draw_eye(0);
    draw_eye(1);
    draw_eyeb(0, 3, 3);
    draw_eyeb(1, 3, 3);

    drawWave();

    analyz0(random(128));
    mtrx.update();
    delay(3000);

    drawCrazy();
    for (int i = 0; i < 10; i++) {
      change_state();
    }

    delay(2000);

    draw_eyeb(0, 4, 3, 3);
    draw_eyeb(1, 1, 3, 3);

    mtrx.update();

    delay(2000);

    draw_eyeb(0, 4, 3, 3);
    draw_eyeb(1, 1, 3, 3);
    mtrx.update();

    delay(2000);




    upd_bright();
    mtrx.clear();
    mtrx.update();
  */
}

void drawCrazy() {
  draw_eye(0);
  draw_eye(1);

  if (random(100) % 2) {
    draw_eyeb(0, 4, 3, 3);
    draw_eyeb(1, 1, 3, 3);
  } else {
    draw_eyeb(0, 3, 3);
    draw_eyeb(1, 3, 3);
  }
  mtrx.lineH(0, ANALYZ_WIDTH, ANALYZ_WIDTH + 16 - 1, GFX_CLEAR);
  mtrx.lineH(1, ANALYZ_WIDTH + 5, ANALYZ_WIDTH + 5 + 6 - 1, GFX_CLEAR);
  mtrx.lineH(2, ANALYZ_WIDTH + 6, ANALYZ_WIDTH + 6 + 4 - 1, GFX_CLEAR);
  mtrx.lineH(3, ANALYZ_WIDTH + 7, ANALYZ_WIDTH + 7 + 2 - 1, GFX_CLEAR);

  // mtrx.update();
}

void drawWave() {
  mtrx.lineH(0, ANALYZ_WIDTH, ANALYZ_WIDTH + 16 - 1, GFX_CLEAR);
  mtrx.lineH(1, ANALYZ_WIDTH + 5, ANALYZ_WIDTH + 5 + 6 - 1, GFX_CLEAR);
  mtrx.lineH(2, ANALYZ_WIDTH + 6, ANALYZ_WIDTH + 6 + 4 - 1, GFX_CLEAR);
  mtrx.lineH(3, ANALYZ_WIDTH + 7, ANALYZ_WIDTH + 7 + 2 - 1, GFX_CLEAR);
}

void drawEye() {
  for (int i = 0; i < 10; i++) {
    static uint16_t pos;
    pos += 15;
    uint8_t x = inoise8(pos);
    uint8_t y = inoise8(pos + UINT16_MAX / 4);
    x = constrain(x, 40, 255 - 40);
    y = constrain(y, 40, 255 - 40);
    x = map(x, 40, 255 - 40, 2, 5);
    y = map(y, 40, 255 - 40, 2, 5);
    if (pulse) {
      pulse = 0;
      int8_t sx = random(-1, 1);
      int8_t sy = random(-1, 1);
      draw_eyeb(0, x + sx, y + sy, 3);
      draw_eyeb(1, x + sx, y + sy, 3);
    } else {
      draw_eyeb(0, x, y);
      draw_eyeb(1, x, y);
    }
    mtrx.update();
    delay(400);
    draw_eye(0);
    draw_eye(1);
  }

  //mtrx.clear();

}
