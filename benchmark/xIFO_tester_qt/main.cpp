/**
 * @file    main.cpp
 * @brief   xifo circular buffer testing with Qt 5
 * @details This file runs a basic test of all xIFO types.
 *
 * @author	Jeroen Lodder
 * @date	March 2014
 * @version 1
 * 
 * Copyright (c) 2014 Jeroen Lodder
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @{
 */

#include <QCoreApplication>
#include <QTextStream>

#include "../../xIFO/xIFO.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextStream cout(stdout);

    cout << "This little tester tests the basics of the xIFO buffer" << endl;

    // This should be allocated as block, so testing of pointer errors is possible
    struct mems_t{
        uint32_t failure1;
        xifo8_pool_t mx8_p[8];
        uint32_t failure2;
        xifo16_pool_t mx16_p[8];
        uint32_t failure3;
        xifo32_pool_t mx32_p[8];
        uint32_t failure4;
        xifo64_pool_t mx64_p[8];
        uint32_t failure5;
        int mx_data[8];
        uint32_t failure6;
    }mems;

    // Fill test regions
    #define TEST_PATTERN 0x44444444
    mems.failure1 = TEST_PATTERN;
    mems.failure2 = TEST_PATTERN;
    mems.failure3 = TEST_PATTERN;
    mems.failure4 = TEST_PATTERN;
    mems.failure5 = TEST_PATTERN;
    mems.failure6 = TEST_PATTERN;

    // For debugger:
    static uint32_t *f1 = &mems.failure1;
    static uint32_t *f2 = &mems.failure2;
    static uint32_t *f3 = &mems.failure3;
    static uint32_t *f4 = &mems.failure4;
    static uint32_t *f5 = &mems.failure5;
    static uint32_t *f6 = &mems.failure6;

    // To ease the code below
    static xifo8_pool_t *x8_p = mems.mx8_p;
    static xifo16_pool_t *x16_p = mems.mx16_p;
    static xifo32_pool_t *x32_p = mems.mx32_p;
    static xifo64_pool_t *x64_p = mems.mx64_p;
    static int *x_data = mems.mx_data;

    // Create buffers
    xifo8_t x8;
    xifo16_t x16;
    xifo32_t x32;
    xifo64_t x64;
    Xifo<int> xifoCpp(8, x_data);

    xifo8_init(&x8, 8 ,x8_p);
    xifo16_init(&x16, 8 ,x16_p);
    xifo32_init(&x32, 8 ,x32_p);
    xifo64_init(&x64, 8 ,x64_p);

    xifo8_clear(&x8);
    xifo16_clear(&x16);
    xifo32_clear(&x32);
    xifo64_clear(&x64);
    xifoCpp.clear();

    cout << "Created 5 buffers of 8 bytes" << endl;
    cout << "Size = 5 -> "
         << xifo8_get_size(&x8) << " "
         << xifo16_get_size(&x16) << " "
         << xifo32_get_size(&x32) << " "
         << xifo64_get_size(&x64) << " "
         << xifoCpp.size() << endl;
    cout << "Free = 8 -> "
         << xifo8_get_free(&x8) << " "
         << xifo16_get_free(&x16) << " "
         << xifo32_get_free(&x32) << " "
         << xifo64_get_free(&x64) << " "
         << xifoCpp.free() << endl;
    cout << "Used = 0 -> "
         <<  xifo8_get_used(&x8) << " "
          << xifo16_get_used(&x16) << " "
          << xifo32_get_used(&x32) << " "
          << xifo64_get_used(&x64) << " "
          << xifoCpp.used() << endl;
    cout << "Full = 0 -> "
         <<  xifo8_get_full(&x8) << " "
          << xifo16_get_full(&x16) << " "
          << xifo32_get_full(&x32) << " "
          << xifo64_get_full(&x64) << " "
          << xifoCpp.full() << endl;

    cout << endl << "Lets do some stuff!" << endl << endl;

    uint8_t     data8   = 10;
    uint16_t    data16  = 10;
    uint32_t    data32  = 10;
    uint64_t    data64  = 10;
    int         data    = 10;

    // Write 3 bytes
    for(int i=0; i<3; i++){
        xifo8_write(&x8,data8++);
        xifo16_write(&x16,data16++);
        xifo32_write(&x32,data32++);
        xifo64_write(&x64,data64++);
        xifoCpp.write(data++);
    }

    cout << "Written 3 of 5 bytes" << endl;
    cout << "Free = 5 -> "
         << xifo8_get_free(&x8) << " "
         << xifo16_get_free(&x16) << " "
         << xifo32_get_free(&x32) << " "
         << xifo64_get_free(&x64) << " "
         << xifoCpp.free() << endl;
    cout << "Used = 3 -> "
         <<  xifo8_get_used(&x8) << " "
          << xifo16_get_used(&x16) << " "
          << xifo32_get_used(&x32) << " "
          << xifo64_get_used(&x64) << " "
          << xifoCpp.used() << endl;
    cout << "Full = 0 -> "
         <<  xifo8_get_full(&x8) << " "
          << xifo16_get_full(&x16) << " "
          << xifo32_get_full(&x32) << " "
          << xifo64_get_full(&x64) << " "
          << xifoCpp.full() << endl;

    cout << "Show content: " << endl;
    cout << "From start (FIFO): " << endl;
    for(int i=0; i<8; i++){
        cout << "Read " << i << ": "
             << xifo8_read_lr(&x8,i) << " "
             << xifo16_read_lr(&x16,i) << " "
             << xifo32_read_lr(&x32,i) << " "
             << xifo64_read_lr(&x64,i) << " "
             << xifoCpp.read_lr(i) << endl;
    }
    cout << "From end (LIFO): " << endl;
    for(int i=0; i<8; i++){
        cout << "Read " << i << ": "
             << xifo8_read_mr(&x8,i) << " "
             << xifo16_read_mr(&x16,i) << " "
             << xifo32_read_mr(&x32,i) << " "
             << xifo64_read_mr(&x64,i) << " "
             << xifoCpp.read_mr(i) << endl;
    }

    cout << endl;

    // Write 13 bytes in buffer
    for(int i=0; i<13; i++){
        xifo8_write(&x8,data8++);
        xifo16_write(&x16,data16++);
        xifo32_write(&x32,data32++);
        xifo64_write(&x64,data64++);
        xifoCpp.write(data++);
    }

    cout << "Written another 13 bytes, total 16\nBuffer should contain 18 upto 25 since its a ringbuffer" << endl;
    cout << "From start (FIFO): " << endl;
    for(int i=0; i<8; i++){
        cout << "Read " << i << ": "
             << xifo8_read_lr(&x8,i) << " "
             << xifo16_read_lr(&x16,i) << " "
             << xifo32_read_lr(&x32,i) << " "
             << xifo64_read_lr(&x64,i) << " "
             << xifoCpp.read_lr(i) << endl;
    }
    cout << "From end (LIFO): " << endl;
    for(int i=0; i<8; i++){
        cout << "Read " << i << ": "
             << xifo8_read_mr(&x8,i) << " "
             << xifo16_read_mr(&x16,i) << " "
             << xifo32_read_mr(&x32,i) << " "
             << xifo64_read_mr(&x64,i) << " "
             << xifoCpp.read_mr(i) << endl;
    }

    cout << "Free = 0 -> "
         << xifo8_get_free(&x8) << " "
         << xifo16_get_free(&x16) << " "
         << xifo32_get_free(&x32) << " "
         << xifo64_get_free(&x64) << " "
         << xifoCpp.free() << endl;
    cout << "Used = 8 -> "
         <<  xifo8_get_used(&x8) << " "
          << xifo16_get_used(&x16) << " "
          << xifo32_get_used(&x32) << " "
          << xifo64_get_used(&x64) << " "
          << xifoCpp.used() << endl;
    cout << "Full = 1 -> "
         <<  xifo8_get_full(&x8) << " "
          << xifo16_get_full(&x16) << " "
          << xifo32_get_full(&x32) << " "
          << xifo64_get_full(&x64) << " "
          << xifoCpp.full() << endl;

    cout << endl;

    // Lets pop
    cout << "Pop = 25 -> "
         << xifo8_pop_mr(&x8) << " "
         << xifo16_pop_mr(&x16) << " "
         << xifo32_pop_mr(&x32) << " "
         << xifo64_pop_mr(&x64) << " "
         << xifoCpp.pop_mr() << endl;
    cout << "Pop = 18 -> "
         << xifo8_pop_lr(&x8) << " "
         << xifo16_pop_lr(&x16) << " "
         << xifo32_pop_lr(&x32) << " "
         << xifo64_pop_lr(&x64) << " "
         << xifoCpp.pop_lr() << endl;
    cout << "Free = 2 -> "
         << xifo8_get_free(&x8) << " "
         << xifo16_get_free(&x16) << " "
         << xifo32_get_free(&x32) << " "
         << xifo64_get_free(&x64) << " "
         << xifoCpp.free() << endl;
    cout << "Used = 6 -> "
         << xifo8_get_used(&x8) << " "
         << xifo16_get_used(&x16) << " "
         << xifo32_get_used(&x32) << " "
         << xifo64_get_used(&x64) << " "
         << xifoCpp.used() << endl;
    cout << "Full = 0 -> "
         << xifo8_get_full(&x8) << " "
         << xifo16_get_full(&x16) << " "
         << xifo32_get_full(&x32) << " "
         << xifo64_get_full(&x64) << " "
         << xifoCpp.full() << endl;

    // Pop empty
    cout << "Pop = 24 -> "
         <<  xifo8_pop_mr(&x8) << " "
          << xifo16_pop_mr(&x16) << " "
          << xifo32_pop_mr(&x32) << " "
          << xifo64_pop_mr(&x64) << " "
          << xifoCpp.pop_mr() << endl;
    cout << "Pop = 19 -> "
         <<  xifo8_pop_lr(&x8) << " "
          << xifo16_pop_lr(&x16) << " "
          << xifo32_pop_lr(&x32) << " "
          << xifo64_pop_lr(&x64) << " "
          << xifoCpp.pop_lr() << endl;
    // Pop empty
    cout << "Pop = 23 -> "
         <<  xifo8_pop_mr(&x8) << " "
          << xifo16_pop_mr(&x16) << " "
          << xifo32_pop_mr(&x32) << " "
          << xifo64_pop_mr(&x64) << " "
          << xifoCpp.pop_mr() << endl;
    cout << "Pop = 20 -> "
         <<  xifo8_pop_lr(&x8) << " "
          << xifo16_pop_lr(&x16) << " "
          << xifo32_pop_lr(&x32) << " "
          << xifo64_pop_lr(&x64) << " "
          << xifoCpp.pop_lr() << endl;
    // Pop empty
    cout << "Pop = 22 -> "
         <<  xifo8_pop_mr(&x8) << " "
          << xifo16_pop_mr(&x16) << " "
          << xifo32_pop_mr(&x32) << " "
          << xifo64_pop_mr(&x64) << " "
          << xifoCpp.pop_mr() << endl;
    cout << "Pop = 21 -> "
         <<  xifo8_pop_lr(&x8) << " "
          << xifo16_pop_lr(&x16) << " "
          << xifo32_pop_lr(&x32) << " "
          << xifo64_pop_lr(&x64) << " "
          << xifoCpp.pop_lr() << endl;
    // Empty
    cout << "Pop = 0 (because empty) -> "
         <<  xifo8_pop_lr(&x8) << " "
          << xifo16_pop_lr(&x16) << " "
          << xifo32_pop_lr(&x32) << " "
          << xifo64_pop_lr(&x64) << " "
          << xifoCpp.pop_lr() << endl;

    cout << "Free = 8 -> "
         << xifo8_get_free(&x8) << " "
         << xifo16_get_free(&x16) << " "
         << xifo32_get_free(&x32) << " "
         << xifo64_get_free(&x64) << " "
         << xifoCpp.free() << endl;
    cout << "Used = 0 -> "
         <<  xifo8_get_used(&x8) << " "
          << xifo16_get_used(&x16) << " "
          << xifo32_get_used(&x32) << " "
          << xifo64_get_used(&x64) << " "
          << xifoCpp.used() << endl;
    cout << "Full = 0 -> "
         <<  xifo8_get_full(&x8) << " "
          << xifo16_get_full(&x16) << " "
          << xifo32_get_full(&x32) << " "
          << xifo64_get_full(&x64) << " "
          << xifoCpp.full() << endl;

    cout << "Buffer is empty:"<<endl;
    cout << "From start (FIFO): " << endl;
    for(int i=0; i<8; i++){
        cout << "Read " << i << ": "
             << xifo8_read_lr(&x8,i) << " "
             << xifo16_read_lr(&x16,i) << " "
             << xifo32_read_lr(&x32,i) << " "
             << xifo64_read_lr(&x64,i) << " "
             << xifoCpp.read_lr(i) << endl;
    }
    cout << "From end (LIFO): " << endl;
    for(int i=0; i<8; i++){
        cout << "Read " << i << ": "
             << xifo8_read_mr(&x8,i) << " "
             << xifo16_read_mr(&x16,i) << " "
             << xifo32_read_mr(&x32,i) << " "
             << xifo64_read_mr(&x64,i) << " "
             << xifoCpp.read_mr(i) << endl;
    }

    // Test for mem range error
    cout << endl << "Any of the 6 memory errors?" << endl;
    if(mems.failure1 != TEST_PATTERN)
        cout << "YES: failure 1" << endl;
    else
        cout << "1 NO!" << endl;
    if(mems.failure2 != TEST_PATTERN)
        cout << "YES: failure 2" << endl;
    else
        cout << "2 NO!" << endl;
    if(mems.failure3 != TEST_PATTERN)
        cout << "YES: failure 3" << endl;
    else
        cout << "3 NO!" << endl;
    if(mems.failure4 != TEST_PATTERN)
        cout << "YES: failure 4" << endl;
    else
        cout << "4 NO!" << endl;
    if(mems.failure5 != TEST_PATTERN)
        cout << "YES: failure 5" << endl;
    else
        cout << "5 NO!" << endl;
    if(mems.failure6 != TEST_PATTERN)
        cout << "YES: failure 6" << endl;
    else
        cout << "6 NO!" << endl;
    return a.exec();
}
/** @} */
