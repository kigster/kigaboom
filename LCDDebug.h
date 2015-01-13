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
        _size = rows * columns;
        _scroll_rows = 2;
        _bufferNew = (char *) malloc(_size);
        _bufferShown = (char *) malloc(_size);
        _blankRow = (char *) malloc(_cols);
        memset(_bufferNew, ' ', _size);
        memset(_bufferShown, ' ', _size);
        memset(_blankRow, ' ', _cols);
    }

    LCDDebug() : LCDDebug(0x3F, 20, 4) {
    }

    void init() {
        _lcd.init();
        _lcd.backlight();
        _lcd.cursor_on();
        _lcd.noAutoscroll();
    }

    ~LCDDebug() {
        if (_bufferNew != NULL)
            free(_bufferNew);
        if (_bufferShown != NULL)
            free(_bufferShown);
        _bufferNew = _bufferShown = NULL;
    }

    void overwrite(const char *message, int row) {
        row = row % _rows;
        memcpy(_bufferNew, _bufferShown, _size);
        strncpy(_bufferNew + row * _cols, _blankRow, _cols);
        strncpy(_bufferNew + row * _cols, message, max(strlen(message), (size_t) (_cols)));
        _syncBufferToDisplay();
    }
    void print(const char *message) {
        overwrite(message, 0);
    }
    void stickyPrint(const char *message, int stickyRow) {
        overwrite(message, stickyRow);
    }
    void clear() {
        _lcd.clear();
    }

private:
    LiquidCrystal_I2C _lcd;
    unsigned short _rows, _cols, _size, _scroll_rows;
    char *_bufferNew, *_bufferShown, *_blankRow;
    void _syncBufferToDisplay() {
        for (int i = 0; i < _size; i++)
            if (!isprint(_bufferNew[i])) _bufferNew[i] = ' ';

        if (strncmp(_bufferNew, _bufferShown, _size) == 0) return;
        for (int j = 0; j < _rows; j++) {
            for (int i = 0; i < _cols; i++) {
                int index = i + j * _cols;
                if (_bufferNew[index] == _bufferShown[index])
                    continue;
                _bufferShown[index] = _bufferNew[index];
                _lcd.setCursor(i, j);
                _lcd.print((char) _bufferShown[index]);
            }
        }
    }
};

#endif /* LCDDEBUG_H_ */
