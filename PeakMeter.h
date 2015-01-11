/*
 * PeakMeter.h
 *
 *  Created on: Jan 10, 2015
 *      Author: Konstantin Gredeskoul
 *        Code: https://github.com/kigster
 *
 *  (c) 2014 All rights reserved, MIT License.
 */
#include "Hardware.h"
#ifdef ENABLE_PEAKS

#ifndef PEAKMETER_H_
#define PEAKMETER_H_

#include "TFTHelper.h"
#include <Audio.h>
extern colorScheme_t colors;

typedef struct peakReading_s {
    float left;
    float right;
} peakReading_t;

class PeakMeter {

public:
    PeakMeter(AudioAnalyzePeak &left, AudioAnalyzePeak &right) :
        _peakLeft(left), _peakRight(right) {};

    peakReading_t update() {
      if(fps > 24) {
        if (_peakLeft.available() && _peakRight.available()) {
          fps=0;
          _peaks.left = _peakLeft.read();
          _peaks.right = _peakRight.read();
        }
      }
      return _peaks;
    }
#ifdef ENABLE_TFT
    void show(TFTHelper *tftHelper) {
        tftHelper->drawVerticalBar(35, 60, _peaks.left, 0, 140, 120, 10, colors.peakBar, colors.background);
        tftHelper->drawVerticalBar(35, 60, _peaks.left, 1, 140, 120, 10, colors.peakBar, colors.background);
    }
#endif

    void printPeaksToSerial() {
        uint8_t leftPeak = _peaks.left  * 30.0;
        uint8_t rightPeak = _peaks.right * 30.0;
        for (cnt = 0; cnt < 30 - leftPeak; cnt++) {
            Serial.print(" ");
        }
        while (cnt++ < 30) {
            Serial.print("<");
        }
        Serial.print("||");
        for (cnt = 0; cnt < rightPeak; cnt++) {
            Serial.print(">");
        }
        while (cnt++ < 30) {
            Serial.print(" ");
        }
        Serial.println();
    }

private:
    elapsedMillis fps;
    uint8_t cnt = 0;
    peakReading_t _peaks;
    AudioAnalyzePeak &_peakLeft;
    AudioAnalyzePeak &_peakRight;
};


#endif /* PEAKMETER_H_ */
#endif /* ENABLE_PEAKS */
