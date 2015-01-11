/*
 * TFTHelper.h
 *
 *  Created on: Jan 10, 2015
 *      Author: Konstantin Gredeskoul
 *        Code: https://github.com/kigster
 *
 *  (c) 2014 All rights reserved, MIT License.
 */

#include "Hardware.h"
#ifdef ENABLE_TFT

#ifndef TFTHELPER_H_
#define TFTHELPER_H_
#include <Audio.h>
#include <ILI9341_t3.h>
#include "ColorScheme.h"
#ifndef TFT_CS
#define TFT_CS 21
#endif

#ifndef TFT_DC
#define TFT_DC 20
#endif

extern colorScheme_t colors;

class TFTHelper {
    ILI9341_t3 _tft = ILI9341_t3(TFT_CS, TFT_DC);

public:
    TFTHelper() {

    }

    ILI9341_t3 *getTFT() {
        return &_tft;
    }

    void begin() {
        _tft.begin();
        _tft.setRotation(3);
        _tft.fillScreen(colors.background);
        _tft.setTextColor(colors.text);
        _tft.setTextSize(2);
        _tft.setCursor(30, _tft.height() / 2 - 20);
        _tft.print("TeensyDSP Audio v0.1");
        _tft.setTextSize(1);
        delay(1000);
        _tft.fillScreen(colors.background);
    }

    void drawVerticalBar(short width, short height, float value, short index,
            short positionTop, short positionFirstLeft, short padding,
            uint16_t barColor, uint16_t backgroundColor) {

        short xPos = positionFirstLeft + index * (width + padding);

        if (value > 1.0)
            value = 1.0;
        _tft.fillRect(xPos, positionTop, width, height, backgroundColor);
        _tft.fillRect(xPos, positionTop + height - value * height, width,
                value * height, barColor);
    }

    void drawStatusBar() {
        _tft.fillRect(0, 0, _tft.width(), 20, colors.background);
        _tft.drawRect(1, 1, _tft.width() - 2, 20, colors.status);
    }

    void updateStatusBar(char * messageLeft, char *messageRight) {
        drawStatusBar();
        _tft.setCursor(10, 8);
        _tft.print(messageLeft);
    }

};

#endif /* TFTHELPER_H_ */

#endif /* ENABLE_TFT */
