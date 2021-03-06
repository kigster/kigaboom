/*
 * hardware.h
 *
 *  Created on: Jan 10, 2015
 *      Author: Konstantin Gredeskoul
 *        Code: https://github.com/kigster
 *
 *  (c) 2014 All rights reserved, MIT License.
 */

#ifndef HARDWARE_H_
#define HARDWARE_H_

#include <Audio.h>

// Feature flags.  Comment out to disable.
//#define ENABLE_TFT
//#define ENABLE_SPECTRUM
#define ENABLE_PEAKS
#define ENABLE_SHIFT_REGISTER
#define ENABLE_ROTARY_ENC

typedef struct hardwareConfig_s{
    uint8_t pinButton;
    uint8_t pinRotaryA;
    uint8_t pinRotaryB;

    uint8_t pinShifterSerial;
    uint8_t pinShifterRclk;
    uint8_t pinShifterSrclk;

    uint8_t numRegisters;
    int audioInput;

    uint8_t red;
    uint8_t green;
    uint8_t blue;

} HardwareConfig;

const static HardwareConfig config = {
        26,     // button
        25,     // rotaryA
        24,     // rotaryB

        3,      // SER_IN
        4,      // L_CLOCK
        5,      // CLOCK

        3,
        AUDIO_INPUT_LINEIN, // or AUDIO_INPUT_MIC,

        21,     // red
        22,     // green
        23      // blue
};


#endif /* HARDWARE_H_ */
