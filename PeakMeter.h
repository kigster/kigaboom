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
#ifdef ENABLE_TFT
#include "TFTHelper.h"
#endif

#ifdef ENABLE_SHIFT_REGISTER
#include <Shifter.h>
#endif

#include <Audio.h>
extern colorScheme_t colors;

typedef struct peakReading_s {
    float left;
    float right;
} peakReading_t;

class PeakMeter {

public:
    PeakMeter(AudioAnalyzePeak &left, AudioAnalyzePeak &right) :
            _peakLeft(left), _peakRight(right) {
    };

    peakReading_t update() {
        if (fps > 24) {
            if (_peakLeft.available() && _peakRight.available()) {
                fps = 0;
                _peaks.left = _peakLeft.read();
                _peaks.right = _peakRight.read();
            }
        }
        return _peaks;
    }

#ifdef ENABLE_TFT
    void show(TFTHelper &tftHelper) {
        update();
        tftHelper.drawVerticalBar(35, 60, _peaks.left, 0, 140, 120, 10,
                colors.peakBar, colors.background);
        tftHelper.drawVerticalBar(35, 60, _peaks.right, 1, 140, 120, 10,
                colors.peakBar, colors.background);
    }
#endif

#ifdef ENABLE_SHIFT_REGISTER
    void show(Shifter &shifter) {
        if (lastLedUpdate > 24) {
            lastLedUpdate = 0;
            update();
            drawPeak(shifter, 0, _peaks.left);
            drawPeak(shifter, 1, _peaks.right);
            shifter.write();
        }
    }
#endif

    // Version without params for Serial
    void show() {
        uint8_t leftPeak = _peaks.left * 30.0;
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
    elapsedMillis lastLedUpdate;

#ifdef ENABLE_SHIFT_REGISTER
    void drawPeak(Shifter &shifter, short channel, float volume) {
        short bar = volume * 10;
        for (int i = channel * 10; i < (channel + 1) * 10; i ++) {
            shifter.setPin(i,  (i - channel * 10 <= bar) ? HIGH : LOW);
        }
    }
#endif

};

#endif /* PEAKMETER_H_ */
#endif /* ENABLE_PEAKS */
