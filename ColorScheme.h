/*
 * ColorScheme.h
 *
 *  Created on: Jan 10, 2015
 *      Author: Konstantin Gredeskoul
 *        Code: https://github.com/kigster
 *
 *  (c) 2014 All rights reserved, MIT License.
 */

#ifndef COLORSCHEME_H_
#define COLORSCHEME_H_
#include <ILI9341_t3.h>

typedef struct colorScheme_s {
    uint16_t background;
    uint16_t text;
    uint16_t status;
    uint16_t spectrumBar;
    uint16_t peakBar;
} colorScheme_t;

const static colorScheme_t defaultColors = {
    ILI9341_BLACK,
    ILI9341_YELLOW,
    ILI9341_WHITE,
    ILI9341_BLUE,
    ILI9341_GREEN
};

static colorScheme_t colors = defaultColors;

#endif /* COLORSCHEME_H_ */
