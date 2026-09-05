/* ================================================================
   Name: RETRO OS BOOT SIMULATOR
   WINDOWS XP FULL SYSTEM FOR ESP32-S3 (REAL IMAGES VERSION)
   Target Hardware: ESP32-S3 + ST7789 TFT (240x320)
   Youtube Chanel- https://www.youtube.com/@razfriday
   ================================================================ */

#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>

#include "Boot.h"
#include "Welcome.h"
#include "Error.h"
#include "Bsod.h"

// Display Pin Definitions for ESP32-S3
#define TFT_SCK   13
#define TFT_MISO  12
#define TFT_MOSI  11
#define TFT_CS    10
#define TFT_DC     9
#define TFT_RST    8
#define TFT_LED    5

Adafruit_ST7789 tft = Adafruit_ST7789(&SPI, TFT_CS, TFT_DC, TFT_RST);

// Draw a full-screen RGB565 image stored in PROGMEM, streaming row by row
// (avoids allocating a full-frame buffer in RAM, which overflows DRAM)
void drawProgmemImage(const uint16_t *img, int w, int h) {
  uint16_t rowBuf[320]; // one row at a time, small enough for RAM
  tft.startWrite();
  tft.setAddrWindow(0, 0, w, h);
  for (int y = 0; y < h; y++) {
    memcpy_P(rowBuf, img + (y * w), w * sizeof(uint16_t));
    tft.writePixels(rowBuf, w);
  }
  tft.endWrite();
}

// 1. Boot Screen - plays Boot.gif animation, looped 3 times
void showBootScreen() {
  for (int loopNum = 0; loopNum < 3; loopNum++) {
    for (int i = 0; i < BOOT_IMG_FRAMES; i++) {
      const uint16_t *frame = (const uint16_t*)pgm_read_ptr(&boot_img_frames[i]);
      drawProgmemImage(frame, BOOT_IMG_W, BOOT_IMG_H);
      delay(160);
    }
  }
}

// 2. Welcome Screen - static image, held on screen across 2 "loops"
void showWelcomeScreen() {
  for (int loopNum = 0; loopNum < 2; loopNum++) {
    drawProgmemImage(welcome_img, WELCOME_IMG_W, WELCOME_IMG_H);
    delay(2000);
  }
}

// 3. Desktop + Error dialogs (cascading popups), looped 1 times
void showDesktopWithError() {
  for (int loopNum = 0; loopNum < 1; loopNum++) {
    for (int i = 0; i < ERROR_IMG_FRAMES; i++) {
      const uint16_t *frame = (const uint16_t*)pgm_read_ptr(&error_img_frames[i]);
      drawProgmemImage(frame, ERROR_IMG_W, ERROR_IMG_H);
      delay(2000);
    }
  }
}

// 4. BSOD Screen - static image, shown after the error dialogs
void showBSOD() {
  for (int loopNum = 0; loopNum < 2; loopNum++) {
  drawProgmemImage(bsod_img, BSOD_IMG_W, BSOD_IMG_H);
  delay(4000);
}
  }
  
void setup() {
  Serial.begin(115200);

#if TFT_LED >= 0
  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, HIGH); // Backlight ON
#endif

  SPI.begin(TFT_SCK, TFT_MISO, TFT_MOSI, -1);

  tft.init(240, 320);          // Screen Size 
  tft.setRotation(1);          // Landscape Mode 1 or 3 (320x240)
  tft.invertDisplay(false);
}

void loop() {
  showBootScreen();
  showWelcomeScreen();
  showDesktopWithError();
  showBSOD();
}