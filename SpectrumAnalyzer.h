/*
 * SpectrumAnalyzer.h
 *
 *  Created on: Jan 5, 2015
 *      Author: Konstantin Gredeskoul
 *        Code: https://github.com/kigster
 *
 *  (c) 2014 All rights reserved, MIT License.
 */

#include "Hardware.h"
#if defined(ENABLE_SPECTRUM) && defined(ENABLE_TFT)

#ifndef SPECTRUMANALYZER_H_
#define SPECTRUMANALYZER_H_
#include <ILI9341_t3.h>
#include "ColorScheme.h"
#include <Audio.h>
#include "TFTHelper.h"

extern colorScheme_t colors;

class SpectrumAnalyzer {
     AudioAnalyzeFFT1024 &_fft1024;

    // An array to hold the 16 frequency bands
    float _level[16];

    // This array holds the on-screen _levels.  When the signal drops quickly,
    // these are used to lower the on-screen _level 1 bar per update, which
    // looks more pleasing to corresponds to human sound perception.
    int _shown[16];
public:
    SpectrumAnalyzer( AudioAnalyzeFFT1024 &fft1024) : _fft1024(fft1024) {};

    void drawSpectrumMeter(TFTHelper &tftHelper, short index, float value) {
        tftHelper.drawVerticalBar(20, 60, value, index, 30, 10, 5,
                colors.spectrumBar, colors.background);
    }

    void show(TFTHelper &tftHelper) {
        if (_fft1024.available()) {
            // read the 512 FFT frequencies into 16 _levels
            // music is heard in octaves, but the FFT data
            // is linear, so for the higher octaves, read
            // many FFT bins together.
            _level[0] = _fft1024.read(0);
            _level[1] = _fft1024.read(1);
            _level[2] = _fft1024.read(2, 3);
            _level[3] = _fft1024.read(4, 6);
            _level[4] = _fft1024.read(7, 10);
            _level[5] = _fft1024.read(11, 15);
            _level[6] = _fft1024.read(16, 22);
            _level[7] = _fft1024.read(23, 32);
            _level[8] = _fft1024.read(33, 46);
            _level[9] = _fft1024.read(47, 66);
            _level[10] = _fft1024.read(67, 93);
            _level[11] = _fft1024.read(94, 131);
            _level[12] = _fft1024.read(132, 184);
            _level[13] = _fft1024.read(185, 257);
            _level[14] = _fft1024.read(258, 359);
            _level[15] = _fft1024.read(360, 511);

            for (int i = 0; i < 16; i++) {
                drawSpectrumMeter(tftHelper, i, _level[i] * 2);
            }
        }
    }


};

#endif /* SPECTRUMANALYZER_H_ */
#endif /* ENABLE_SPECTRUM */
