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


static char buf[128];

#define log(format, ...) \
    sprintf(buf, format, __VA_ARGS__) && \
    Serial.print(buf);

#endif /* DEBUG_H_ */
