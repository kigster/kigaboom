#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#define ARM_MATH_CM4
#include <arm_math.h>
#include <SimpleTimer.h>
#include <RotaryEncoderWithButton.h>
#include <OneButton.h>
#include <Shifter.h>
// Local includes
#include "Hardware.h"
#include "ColorScheme.h"
#include "PeakMeter.h"
#include "SpectrumAnalyzer.h"
#include "TFTHelper.h"

// GUItool: begin automatically generated code
AudioInputI2S i2s1;
AudioOutputI2S i2s2;
AudioConnection patchCord1(i2s1, 0, i2s2, 0);
AudioConnection patchCord2(i2s1, 1, i2s2, 1);
#ifdef ENABLE_PEAKS
    AudioAnalyzePeak peak_L;
    AudioAnalyzePeak peak_R;
    AudioConnection patchCord3(i2s1, 0, peak_L, 0);
    AudioConnection patchCord4(i2s1, 1, peak_R, 0);
    PeakMeter peakMeter(peak_L, peak_R);
#endif
#ifdef ENABLE_TFT
    TFTHelper tftHelper;
#endif
#if defined(ENABLE_SPECTRUM)
    AudioMixer4 mixer1;
    AudioAnalyzeFFT1024 fft1024;
    AudioConnection patchCord5(mixer1, fft1024);
    AudioConnection patchCord7(i2s1, 0, mixer1, 0);
    AudioConnection patchCord8(i2s1, 1, mixer1, 1);
    SpectrumAnalyzer spectrumAnalyzer(fft1024);
#endif

AudioControlSGTL5000 audioShield;
SimpleTimer timer(1);

#ifdef ENABLE_ROTARY_ENC
RotaryEncoderWithButton encoder(
        config.pinRotaryA,
        config.pinRotaryB,
        config.pinButton,
        false);

const OneButton *button = encoder.getButton();
#endif

#ifdef ENABLE_SHIFT_REGISTER

Shifter shifter(config.pinShifterSerial,
        config.pinShifterRclk,
        config.pinShifterSrclk,
        config.numRegisters);

#endif

namespace State {
    typedef enum SystemMode_e {
        LedOn    = (1 << 0),
        LedOff   = (1 << 1),
        Last     = (1 << 2)
    } SystemMode;
};

State::SystemMode state = State::LedOn, firstState = State::LedOn;

void modeChange() {
    state = (State::SystemMode) ((int) state << 1);
    switch(state) {
    case State::LedOn:
        break;
    case State::LedOff:
#ifdef ENABLE_SHIFT_REGISTER
        shifter.clear();
        shifter.write();
#endif
        break;
    case State::Last:
        state = firstState;
        break;
    }
}

void periodicUpdates(int timerId) {
#ifdef ENABLE_TFT
    tftHelper.drawStatusBar();
#endif
}

void setup() {
    SPI.setMOSI(7);
    SPI.setSCK(14);
    AudioMemory(12);

    // Enable the audio shield and set the output volume.
    audioShield.enable();
    audioShield.inputSelect(config.audioInput);
    audioShield.volume(0.6);

#if defined(ENABLE_SPECTRUM) && defined(ENABLE_TFT)
    // configure the mixer to equally add left & right
    mixer1.gain(0, 0.7);
    mixer1.gain(1, 0.7);
    tftHelper.begin();
#endif

#ifdef ENABLE_SHIFT_REGISTER
    shifter.clear();
    shifter.write();
#endif

#ifdef ENABLE_ROTARY_ENC
    encoder.getButton()->attachClick(modeChange);
#endif
    timer.setInterval(1000, periodicUpdates);
}

void loop() {
    timer.run();

#if defined(ENABLE_SPECTRUM) && defined(ENABLE_TFT)
    spectrumAnalyzer.show(tftHelper);
    peakMeter.show(tftHelper);
#endif

#ifdef ENABLE_SHIFT_REGISTER
    peakMeter.show(shifter);
#endif

#ifdef ENABLE_ROTARY_ENC
    encoder.tick();
#endif
}


