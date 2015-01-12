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
#ifndef PEAKMETER_H_
#define PEAKMETER_H_

#include <Audio.h>

typedef struct peakReading_s {
    float left;
    float right;
} peakReading_t;

class PeakMeter {

public:
    PeakMeter(AudioAnalyzePeak &left, AudioAnalyzePeak &right) :
            _peakLeft(left), _peakRight(right) {
    };

    virtual ~PeakMeter() = 0;

    virtual void show() {};
    virtual void clear() {};
    virtual void showValue(float value) {};

    bool enabled() {
        return _enabled;
    }

    void on() {
        _enabled = true;
    }

    void off() {
        _enabled = false;
        clear();
    }

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


protected:
    elapsedMillis fps;
    peakReading_t _peaks;
    bool _enabled = true;

    AudioAnalyzePeak &_peakLeft;
    AudioAnalyzePeak &_peakRight;

};

inline PeakMeter::~PeakMeter() {}

#endif
