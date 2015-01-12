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

#ifndef PEAKMETER_ASCII_H_
#define PEAKMETER_ASCII_H_

class PeakMeterASCII : public PeakMeter {

public:
    PeakMeterASCII(AudioAnalyzePeak &left, AudioAnalyzePeak &right) : PeakMeter(left, right) {
    };

    ~PeakMeterASCII() {};

    void show() {
        if (!_enabled) return;
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
    uint8_t cnt = 0;


};

#endif /* PEAKMETER_ASCII_H_ */
