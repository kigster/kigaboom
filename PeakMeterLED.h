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
#include "PeakMeter.h"

#ifdef ENABLE_SHIFT_REGISTER

#ifndef PEAKMETER_LED_H_
#define PEAKMETER_LED_H_

#include <Shifter.h>
#include <Audio.h>

#define NUM_LEDS 10

class PeakMeterLED : public PeakMeter {

public:
    PeakMeterLED(
            AudioAnalyzePeak &left,
            AudioAnalyzePeak &right,
            Shifter &shifter) :

            PeakMeter(left, right),
            _shifter(shifter) {
    }

    ~PeakMeterLED() {};

    void clear() {
        _resetAllPins(LOW);
        _shifter.write();
    }

    void show() {
        if (!_enabled)
            return;

        if (lastLedUpdate > 24) {
            lastLedUpdate = 0;
            update();
            drawPeak(0, _peaks.left);
            drawPeak(1, _peaks.right);
            _shifter.write();
        }
    }

    void showValue(float value) {
        uint8_t v = value * NUM_LEDS;
        _resetAllPins(LOW);
        _shifter.setPin(v, HIGH);
        _shifter.setPin(v + NUM_LEDS, HIGH);
        _shifter.write();
    };

private:
    Shifter &_shifter;
    elapsedMillis lastLedUpdate;

    void drawPeak(short channel, float volume) {
        short bar = volume * NUM_LEDS;
        for (int i = channel * NUM_LEDS; i < (channel + 1) * NUM_LEDS; i++) {
            _shifter.setPin(i, (i - channel * NUM_LEDS <= bar) ? HIGH : LOW);
        }
    }

    void _resetAllPins(int value) {
        for (int i = 0; i < NUM_LEDS * 2; i++) {
            _shifter.setPin(i, value);
        }
    }
};

#endif /* PEAKMETER_LED_H_ */
#endif /* ENABLE_SHIFT_REGISTER */
