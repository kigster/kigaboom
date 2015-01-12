/*
 * LCDDebug.h
 *
 *  Created on: Jan 12, 2015
 *      Author: Konstantin Gredeskoul
 *        Code: https://github.com/kigster
 *
 *  (c) 2014 All rights reserved, MIT License.
 */

#ifndef LCDDEBUG_H_
#define LCDDEBUG_H_
#include <LiquidCrystal_I2C.h>

class LCDDebug {
public:
    LCDDebug(uint16_t address, short columns, short rows) :
            _lcd(address, columns, rows) {
        _rows = rows;
        _cols = columns;
        _size = rows * columns + 1;
        _bufferNew = (char *) malloc(_size);
        _bufferShown = (char *) malloc(_size);
        memset(_bufferNew,   0x0, _size);
        memset(_bufferShown, 0x0, _size);
    }

    LCDDebug() : LCDDebug(0x3F, 20, 4) {
    }

    void init() {
        _lcd.init();
        _lcd.backlight();
    }

    ~LCDDebug() {
        if (_bufferNew != NULL) free(_bufferNew);
        if (_bufferShown != NULL) free(_bufferShown);
        _bufferNew = _bufferShown = NULL;
    }

    void overwrite(const char *message, int row) {
        memcpy(_bufferNew, _bufferShown, _size);
        memcpy(_bufferNew + row * _cols, message, max(strlen(message), _cols));
        _syncBufferToDisplay();
    }
    void insert(const char *message, int row) {
        memcpy(_bufferNew, _bufferShown, _size);
        row = row % _rows;
        for (int i = row; i < _rows - 1; i++) {
            memcpy(_bufferNew + (i + 1) * _cols, _bufferShown + i * _cols, _cols);
        }
        memcpy(_bufferNew + row * _cols, message, max(strlen(message), _cols));
        _syncBufferToDisplay();
    }
    void print(const char *message) {
        insert(message, 0);
    }
    void stickyPrint(const char *message, int stickyRow) {
        overwrite(message, stickyRow);
    }
    void clear() {
        _lcd.clear();
    }

private:
    LiquidCrystal_I2C _lcd;
    unsigned short _rows, _cols, _size;
    char *_bufferNew, *_bufferShown;

    void _syncBufferToDisplay() {
        if (strncmp(_bufferNew, _bufferShown, _size) != 0) {
            _lcd.clear();
            for (int i = 0; i < _rows; i++) {
                char temp[_cols + 1];
                strncpy(_bufferShown + i * _cols, _bufferNew + i * _cols, _cols);
                strncpy(temp, _bufferNew + i * _cols, _cols);
                temp[_cols] = 0x0;
                _lcd.setCursor(0, i);
                _lcd.print(temp);
            }
        }
    }
};

#endif /* LCDDEBUG_H_ */
