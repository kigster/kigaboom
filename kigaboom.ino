#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#define ARM_MATH_CM4
#include <arm_math.h>

#include <ILI9341_t3.h>
#include <SimpleTimer.h>
// GUItool: begin automatically generated code
AudioInputI2S i2s1;
AudioMixer4 mixer1;
AudioOutputI2S i2s2;
AudioAnalyzeFFT1024 fft1024;
AudioAnalyzePeak peak_L;
AudioAnalyzePeak peak_R;
AudioConnection patchCord1(i2s1, 0, peak_L, 0);
AudioConnection patchCord2(i2s1, 1, peak_R, 0);
AudioConnection patchCord3(i2s1, 0, i2s2, 0);
AudioConnection patchCord4(i2s1, 1, i2s2, 1);
AudioConnection patchCord5(mixer1, fft1024);
AudioConnection patchCord7(i2s1, 0, mixer1, 0);
AudioConnection patchCord8(i2s1, 1, mixer1, 1);
AudioControlSGTL5000 audioShield;    //xy=366,225
// GUItool: end automatically generated code

const int myInput = AUDIO_INPUT_LINEIN;
//const int myInput = AUDIO_INPUT_MIC;

// The scale sets how much sound is needed in each frequency range to
// show all 8 bars.  Higher numbers are more sensitive.
float scale = 60.0;

// An array to hold the 16 frequency bands
float level[16];

// This array holds the on-screen levels.  When the signal drops quickly,
// these are used to lower the on-screen level 1 bar per update, which
// looks more pleasing to corresponds to human sound perception.
int shown[16];

#define TFT_DC  20
#define TFT_CS  21

SimpleTimer timer(1);

ILI9341_t3 tft = ILI9341_t3(TFT_CS, TFT_DC);

elapsedMillis fps;
uint8_t cnt = 0;

uint16_t colorBackground  = ILI9341_BLACK;
uint16_t colorText = ILI9341_YELLOW;

uint16_t colorStatusBar = ILI9341_WHITE;
uint16_t colorMenuBar = ILI9341_WHITE;
uint16_t colorSpectrum = ILI9341_BLUE;
uint16_t colorPeaks = ILI9341_GREEN;

short padding = 5;

void drawStatusBar() {
    tft.fillRect(0,0,tft.width(), 20, colorBackground);
    tft.drawRect(1,1,tft.width()-2, 20, colorStatusBar);
}
void updateStatusBar(int timerId) {
    drawStatusBar();
    tft.setCursor(10,8);
    tft.print("CPU: ");
    tft.print(AudioProcessorUsageMax());
    tft.print("%");
}
void drawMenuBar() {
    tft.fillRect(0,tft.height() - 20, tft.width(), 20, colorMenuBar);
}

void drawBars(int timerId) {
    for (int i = 0; i < 16; i++) {
        drawSpectrumMeter(i, 1.0 * random(50) / 50);
    }
}
void drawVerticalBar(
        short width,
        short height,
        float value,
        short index,
        short positionTop,
        short positionFirstLeft,
        short padding,
        uint16_t color) {
    short xPos = positionFirstLeft + index * (width + padding);
    if (value > 1.0) value = 1.0;
    tft.fillRect(xPos, positionTop, width, height, colorBackground);
    tft.fillRect(xPos, positionTop + height - value * height, width, value * height, color);

}

void drawPeakMeter(short channel, float value) {
    drawVerticalBar(35, 60, value, channel, 140, 120, 10, colorPeaks);
}

void drawSpectrumMeter(short index, float value) {
    drawVerticalBar(20, 60, value, index, 30, 10, 5,  colorSpectrum);
}

void setup() {
    SPI.setMOSI(7);
    SPI.setSCK(14);
    AudioMemory(12);

    // Enable the audio shield and set the output volume.
    audioShield.enable();
    audioShield.inputSelect(myInput);
    audioShield.volume(0.6);

    // configure the mixer to equally add left & right
    mixer1.gain(0, 0.7);
    mixer1.gain(1, 0.7);

    // pin 21 will select rapid vs animated display
    pinMode(21, INPUT_PULLUP);
    tft.begin();
    tft.setRotation(3);
    tft.fillScreen(colorBackground);
    tft.setTextColor(colorText);
    tft.setTextSize(2);
    tft.setCursor(30, tft.height() / 2 - 20);
    tft.print("TeensyDSP Audio v0.1");
    tft.setTextSize(1);
    delay(1000);
    tft.fillScreen(colorBackground);

    timer.setInterval(1000, updateStatusBar);
}

void loop() {
    timer.run();
    showSpectrum();
    showPeaks();
}

void showSpectrum() {
    if (fft1024.available()) {
      // read the 512 FFT frequencies into 16 levels
      // music is heard in octaves, but the FFT data
      // is linear, so for the higher octaves, read
      // many FFT bins together.
      level[0] =  fft1024.read(0);
      level[1] =  fft1024.read(1);
      level[2] =  fft1024.read(2, 3);
      level[3] =  fft1024.read(4, 6);
      level[4] =  fft1024.read(7, 10);
      level[5] =  fft1024.read(11, 15);
      level[6] =  fft1024.read(16, 22);
      level[7] =  fft1024.read(23, 32);
      level[8] =  fft1024.read(33, 46);
      level[9] =  fft1024.read(47, 66);
      level[10] = fft1024.read(67, 93);
      level[11] = fft1024.read(94, 131);
      level[12] = fft1024.read(132, 184);
      level[13] = fft1024.read(185, 257);
      level[14] = fft1024.read(258, 359);
      level[15] = fft1024.read(360, 511);

      // if you have the volume pot soldered to your audio shield
      // uncomment this line to make it adjust the full scale signal
      //scale = 8.0 + analogRead(A1) / 5.0;


      for (int i=0; i<16; i++) {
        drawSpectrumMeter(i, level[i] * 2);
//        Serial.print(level[i]);
//        // TODO: conversion from FFT data to display bars should be
//        // exponentially scaled.  But how keep it a simple example?
//        int val = level[i] * scale;
//        if (val > 8) val = 8;
//
//        if (val >= shown[i]) {
//          shown[i] = val;
//        } else {
//          if (shown[i] > 0) shown[i] = shown[i] - 1;
//          val = shown[i];
//        }
//
//        //Serial.print(shown[i]);
//        Serial.print(" ");
//
//        // print each custom digit
//        if (shown[i] == 0) {
//  //        lcd.write(' ');
//        } else {
//  //        lcd.write(shown[i] - 1);
//        }
      }
//      Serial.print(" cpu:");
//      Serial.println(AudioProcessorUsageMax());
    }
}


void showPeaks() {
  if(fps > 24) {
    if (peak_L.available() && peak_R.available()) {
      fps=0;
      float left = peak_L.read();
      float right = peak_R.read();
      uint8_t leftPeak= left  * 30.0;
      uint8_t rightPeak= right * 30.0;
      drawPeakMeter(0, left);
      drawPeakMeter(1, right);

      for(cnt=0;cnt<30-leftPeak;cnt++) {
        Serial.print(" ");
      }
      while(cnt++<30) {
        Serial.print("<");
      }
      Serial.print("||");
      for(cnt=0;cnt<rightPeak;cnt++) {
        Serial.print(">");
      }
      while(cnt++<30) {
        Serial.print(" ");
      }
      Serial.println();
    }
  }
}
