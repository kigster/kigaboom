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
#ifdef ENABLE_TFT

#ifndef PEAKMETER_TFT_H_
#define PEAKMETER_TFT_H_

#include "TFTHelper.h"
#include "ColorScheme.h"
#include <Audio.h>

extern colorScheme_t colors;

class PeakMeterTFT : public PeakMeter {

public:
    PeakMeterTFT(
            AudioAnalyzePeak &left,
            AudioAnalyzePeak &right,
            TFTHelper &tftHelper) :
            PeakMeter(left, right),
            _tftHelper(tftHelper) {
    };

    ~PeakMeterTFT() {};

    void show() {
        if (!_enabled) return;
        update();
        _tftHelper.drawVerticalBar(35, 60, _peaks.left, 0, 140, 120, 10,
                colors.peakBar, colors.background);
        _tftHelper.drawVerticalBar(35, 60, _peaks.right, 1, 140, 120, 10,
                colors.peakBar, colors.background);
    }

private:
    TFTHelper _tftHelper;
};

#endif /* PEAKMETER_TFT_H_ */
#endif /* ENABLE_TFT */
