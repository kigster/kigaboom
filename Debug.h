/*
 * Debug.h
 *
 *  Created on: Jan 11, 2015
 *      Author: Konstantin Gredeskoul
 *        Code: https://github.com/kigster
 *
 *  (c) 2014 All rights reserved, MIT License.
 */

#ifndef DEBUG_H_
#define DEBUG_H_

#include "LCDDebug.h"
static char buf[128];

extern LCDDebug debug;

#define log(format, ...) \
    sprintf(buf, format, __VA_ARGS__); \
    Serial.println(buf); \
    debug.overwrite(buf, 1);

char *ftoa(char *a, double f, int precision) {
  long p[] = {0,10,100,1000,10000,100000,1000000,10000000,100000000};

  char *ret = a;
  long heiltal = (long)f;
  itoa(heiltal, a, 10);
  while (*a != '\0') a++;
  *a++ = '.';
  long desimal = abs((long)((f - heiltal) * p[precision]));
  itoa(desimal, a, 10);
  return ret;
}

#endif /* DEBUG_H_ */
