/*
 * Mode.h
 *
 *  Created on: Jan 11, 2015
 *      Author: Konstantin Gredeskoul
 *        Code: https://github.com/kigster
 *
 *  (c) 2014 All rights reserved, MIT License.
 */

#ifndef MODE_H_
#define MODE_H_

#ifdef ENABLE_PEAKS

#include "Hardware.h"
#include <RotaryEncoderWithButton.h>
#include <Shifter.h>
#include "PeakMeter.h"
#include "Debug.h"
#include "StatusLED.h"

namespace State {
    typedef enum SystemMode_e {
        PeakMeterOn = (1 << 0),
        PeakMeterOff = (1 << 1),
        Last = (1 << 2)
    } SystemMode;
};

State::SystemMode initialMode = State::PeakMeterOn;

class Mode {

public:
    Mode( RotaryEncoderWithButton &encoder,
            OneButton &button,
            Shifter &shifter,
            PeakMeter *peakMeter,
            StatusLED &led) :

        _encoder(encoder),
        _button(button),
        _shifter(shifter),
        _led(led){
        _peakMeter = peakMeter;
        _mode = initialMode;
    };

    void begin() {
        _modeSelector();
    }

    void next() {
        _mode = (State::SystemMode) ((int) _mode << 1);
        _modeSelector();
    }

private:

    RotaryEncoderWithButton &_encoder;
    OneButton &_button;
    Shifter &_shifter;
    PeakMeter *_peakMeter;
    StatusLED &_led;
    State::SystemMode _mode;

    void _modeSelector() {
        switch(_mode) {
                case State::Last:
                _mode = initialMode;
                // no break;
            case State::PeakMeterOn:
                log("mode %d, peaks [on]", (int) _mode);
                _peakMeter->on();
                _led.rgb(LED_GREEN | LED_BLUE);
                break;
            case State::PeakMeterOff:
                log("mode %d, peaks [off]", (int) _mode);
                _peakMeter->off();
                _led.rgb(LED_RED | LED_GREEN);
                break;
        }
    }
};

#endif /* MODE_H_ */
#endif /* ENABLE_PEAKS */
