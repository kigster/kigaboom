#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <IRremote.h>
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
#include "PeakMeterLED.h"
#include "PeakMeterTFT.h"
#include "PeakMeterASCII.h"

#include "SpectrumAnalyzer.h"
#include "TFTHelper.h"
#include "Mode.h"
#include "StatusLED.h"
#include "Debug.h"
#include "LCDDebug.h"


// Local Hardware
#ifdef ENABLE_TFT
    TFTHelper tftHelper;
#endif

#ifdef ENABLE_SHIFT_REGISTER
Shifter shifter(config.pinShifterSerial,
        config.pinShifterRclk,
        config.pinShifterSrclk,
        config.numRegisters);
#endif

SimpleTimer timer(1);

// Audio Hardware
AudioInputI2S i2s1;
AudioOutputI2S i2s2;
AudioConnection patchCord1(i2s1, 0, i2s2, 0);
AudioConnection patchCord2(i2s1, 1, i2s2, 1);
AudioControlSGTL5000 audioShield;
float volume = 0.6;

#ifdef ENABLE_PEAKS
    AudioAnalyzePeak peak_L;
    AudioAnalyzePeak peak_R;
    AudioConnection patchCord3(i2s1, 0, peak_L, 0);
    AudioConnection patchCord4(i2s1, 1, peak_R, 0);

    #ifdef ENABLE_TFT
        PeakMeterTFT peakMeterTFT(peak_L, peak_R, tftHelper);
        PeakMeter *peakMeter = &peakMeterTFT;
    #elif defined(ENABLE_SHIFT_REGISTER)
        PeakMeterLED peakMeterLED(peak_L, peak_R, shifter);
        PeakMeter *peakMeter = &peakMeterLED;
    #else
        PeakMeterASCII peakMeterASCII(&peak_L, &peak_R);
        PeakMeter *peakMeter = &peakMeterASCII;
    #endif
#endif

#if defined(ENABLE_SPECTRUM)
    AudioMixer4 mixer1;
    AudioAnalyzeFFT1024 fft1024;
    AudioConnection patchCord5(mixer1, fft1024);
    AudioConnection patchCord7(i2s1, 0, mixer1, 0);
    AudioConnection patchCord8(i2s1, 1, mixer1, 1);
    SpectrumAnalyzer spectrumAnalyzer(fft1024);
#endif


#if defined(ENABLE_ROTARY_ENC) && defined(ENABLE_PEAKS) && defined(ENABLE_SHIFT_REGISTER)
    RotaryEncoderWithButton rotary(
            config.pinRotaryA,
            config.pinRotaryB,
            config.pinButton,
            false);

    StatusLED led(shifter, config.red, config.green, config.blue);
    Mode mode(rotary, rotary.button, shifter, peakMeter, led);
    elapsedMillis lastKnobRotationTimestamp;
#endif

LCDDebug debug;
static short currentStatus = 0;
short statusRefreshEvery = 5, numStatuses = 3;
// =======================================================================================

void periodicUpdates(int timerId) {
    #ifdef ENABLE_TFT
        tftHelper.drawStatusBar();
    #endif
    char floatValue1[10], floatValue2[10];
    switch(currentStatus / statusRefreshEvery) {
        case 0:
            ftoa(floatValue1, AudioProcessorUsage(), 2);
            ftoa(floatValue2, AudioProcessorUsageMax(), 2);
            sprintf(buf, "CPU: %s%% MAX: %s%%", floatValue1, floatValue2);
            break;
        case 1:
            sprintf(buf, "MEM: %d(BLK) MAX: %d", AudioMemoryUsage(), AudioMemoryUsageMax());
            break;
        case 2:
            ftoa(floatValue1, peak_L.processorUsage() + peak_R.processorUsage(), 2);
            sprintf(buf, "Peak Meter CPU: %s%%", floatValue1);
            break;
    }
    debug.overwrite(buf, 3);
    currentStatus++; currentStatus = currentStatus % (numStatuses * statusRefreshEvery);
}

void modeChange() {
    #if defined(ENABLE_ROTARY_ENC) && defined(ENABLE_PEAKS)
        mode.next();
    #endif
}

void setup() {
    #ifdef ENABLE_TFT
        SPI.setMOSI(7);
        SPI.setSCK(14);
    #endif
    Serial.begin(9600);
    AudioMemory(12);
    debug.init();
    debug.print("Setup starting...");
    // Enable the audio shield and set the output volume.
    audioShield.enable();
    audioShield.inputSelect(config.audioInput);
    audioShield.volume(volume);

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
        rotary.button.attachClick(modeChange);
        lastKnobRotationTimestamp = 1001;
        mode.begin();
    #endif
    debug.print("Setup completed.");
    timer.setInterval(1000, periodicUpdates);
}

void loop() {
    timer.run();
    #if defined(ENABLE_SPECTRUM) && defined(ENABLE_TFT)
        spectrumAnalyzer.show(tftHelper);
    #endif

    #ifdef ENABLE_PEAKS
        if (lastKnobRotationTimestamp > 1000)
            peakMeter->show();
    #endif

    #ifdef ENABLE_ROTARY_ENC
        rotary.tick();
    #endif

    int d = rotary.delta();
    if (d != 0) {
        lastKnobRotationTimestamp = 0;
        float newVolume = volume + 0.01 * d;
        if (newVolume < 0) newVolume = 0;
        if (newVolume > 0.9) newVolume = 0.9;
        if (abs(newVolume - volume) > 0) {
            volume = newVolume;
            audioShield.volume(volume);
            sprintf(buf, "Volume: %d%%", (int) (100.0 * volume));
            debug.overwrite(buf, 2);
        }
        peakMeter->showValue(volume);
    }

}


