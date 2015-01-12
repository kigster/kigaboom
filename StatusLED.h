/*
 * StatusLED.h
 *
 *  Created on: Jan 11, 2015
 *      Author: Konstantin Gredeskoul
 *        Code: https://github.com/kigster
 *
 *  (c) 2014 All rights reserved, MIT License.
 */
#include "Hardware.h"
#ifdef ENABLE_SHIFT_REGISTER
#ifndef STATUSLED_H_
#define STATUSLED_H_

#include <Shifter.h>

#define LED_RED     (1 << 0)
#define LED_GREEN   (1 << 1)
#define LED_BLUE    (1 << 2)


class StatusLED {
public:
    StatusLED(Shifter &shifter, uint8_t red, uint8_t green, uint8_t blue) :
        _shifter(shifter), _red(red), _green(green), _blue(blue) {
    }

    void rgb(int mask) {
        red((mask & LED_RED) > 0);
        green((mask & LED_GREEN) > 0);
        blue((mask & LED_BLUE) > 0);
        _shifter.write();
    }

private:
    void turnOn(uint8_t pin) {
        _shifter.setPin(pin, LOW);
    }
    void turnOff(uint8_t pin) {
        _shifter.setPin(pin, HIGH);
    }

    void red(bool on) {
        on ? turnOn(_red) : turnOff(_red);
    }
    void green(bool on) {
        on ? turnOn(_green) : turnOff(_green);
    }
    void blue(bool on) {
        on ? turnOn(_blue) : turnOff(_blue);
    }
    Shifter &_shifter;
    uint8_t _red, _green, _blue;
};


#endif /* STATUSLED_H_ */
#endif
