#include <Arduino.h>

// Load TFT driver library
#include <SPI.h>
#include <TFT_eSPI.h>

// ****** CONFIGURATION IS DONE IN HERE ******
#include "DIyEyes/eye_functions.hpp"

TFT_eSPI tft; // Invoke library

void setup()
{
  Serial.begin(115200);

  Serial.println("Diy Eyes!");
  Serial.println("Starting!");

#if defined(DISPLAY_BACKLIGHT) && (DISPLAY_BACKLIGHT >= 0)
  // Enable backlight pin, initially off
  Serial.println("Backlight turned off");
  pinMode(DISPLAY_BACKLIGHT, OUTPUT);
  digitalWrite(DISPLAY_BACKLIGHT, LOW);
#endif
  // Initialise the eye(s), this will set all chip selects low for the tft.init()
  initEyes();

  Serial.println("Initialising displays!");
  tft.init();
  // tft.fillScreen(TFT_BLACK);

  // Raise chip select(s) so that displays can be individually configured
  digitalWrite(eye[0].tft_cs, HIGH);
  if (NUM_EYES > 1)
    digitalWrite(eye[1].tft_cs, HIGH);

  for (uint8_t e = 0; e < NUM_EYES; e++)
  {
    digitalWrite(eye[e].tft_cs, LOW);
    tft.setRotation(eyeInfo[e].rotation);
    tft.fillScreen(TFT_BLACK);
    digitalWrite(eye[e].tft_cs, HIGH);
  }

#if defined(DISPLAY_BACKLIGHT) && (DISPLAY_BACKLIGHT >= 0)
  Serial.println("Backlight now on!");
  analogWrite(DISPLAY_BACKLIGHT, BACKLIGHT_MAX);
#endif

  // For frame-rate calculation
  startTime = millis();
}

void loop()
{
  updateEye();
}
