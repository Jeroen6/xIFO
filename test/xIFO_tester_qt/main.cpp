/**
 * @file    main.cpp
 * @brief   xifo circular buffer testing with Qt 5
 * @details This file runs a basic test of all xIFO types.
 *
 * @author	Jeroen Lodder
 * @date	March 2014
 * @version 1
 *
 * @{
 */

#include <QCoreApplication>
#include <QTextStream>

#include "../../xIFO_8/xIFO_8.h"
#include "../../xIFO_16/xIFO_16.h"
#include "../../xIFO_32/xIFO_32.h"
#include "../../xIFO_64/xIFO_64.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QTextStream cout(stdout);

    cout << "This little tester tests the basics of the xIFO buffer" << endl;

    // This should be allocated as block
#define TEST_PATTERN 0x44444444
    static uint32_t failure1 = TEST_PATTERN;
    static xifo8_pool_t x8_p[5];
    static uint32_t failure2 = TEST_PATTERN;
    static xifo16_pool_t x16_p[5];
    static uint32_t failure3 = TEST_PATTERN;
    static xifo32_pool_t x32_p[5];
    static uint32_t failure4 = TEST_PATTERN;
    static xifo64_pool_t x64_p[5];
    static uint32_t failure5 = TEST_PATTERN;

    xifo8_t x8;
    xifo16_t x16;
    xifo32_t x32;
    xifo64_t x64;

    xifo8_init(&x8, 5 ,x8_p);
    xifo16_init(&x16, 5 ,x16_p);
    xifo32_init(&x32, 5 ,x32_p);
    xifo64_init(&x64, 5 ,x64_p);

    xifo8_clear(&x8);
    xifo16_clear(&x16);
    xifo32_clear(&x32);
    xifo64_clear(&x64);

    cout << "Created buffers of 5 bytes" << endl;
    cout << "Size = 5 -> "
         << xifo8_get_size(&x8) << " "
         << xifo16_get_size(&x16) << " "
         << xifo32_get_size(&x32) << " "
         << xifo64_get_size(&x64) << endl;
    cout << "Free = 5 -> "
         << xifo8_get_free(&x8) << " "
         << xifo16_get_free(&x16) << " "
         << xifo32_get_free(&x32) << " "
         << xifo64_get_free(&x64) << endl;
    cout << "Used = 0 -> "
         <<  xifo8_get_used(&x8) << " "
          << xifo16_get_used(&x16) << " "
          << xifo32_get_used(&x32) << " "
          << xifo64_get_used(&x64) << endl;
    cout << "Full = 0 -> "
         <<  xifo8_get_full(&x8) << " "
          << xifo16_get_full(&x16) << " "
          << xifo32_get_full(&x32) << " "
          << xifo64_get_full(&x64) << endl;

    cout << endl << "Lets do some stuff!" << endl << endl;

    uint8_t     data8   = 10;
    uint16_t    data16  = 10;
    uint32_t    data32  = 10;
    uint64_t    data64  = 10;

// Write 3 bytes
    for(int i=0; i<3; i++){
        xifo8_write(&x8,data8++);
        xifo16_write(&x16,data16++);
        xifo32_write(&x32,data32++);
        xifo64_write(&x64,data64++);
    }

    cout << "Written 3 of 5 bytes" << endl;
    cout << "Free = 2 -> "
         << xifo8_get_free(&x8) << " "
         << xifo16_get_free(&x16) << " "
         << xifo32_get_free(&x32) << " "
         << xifo64_get_free(&x64) << endl;
    cout << "Used = 3 -> "
         <<  xifo8_get_used(&x8) << " "
          << xifo16_get_used(&x16) << " "
          << xifo32_get_used(&x32) << " "
          << xifo64_get_used(&x64) << endl;
    cout << "Full = 0 -> "
         <<  xifo8_get_full(&x8) << " "
          << xifo16_get_full(&x16) << " "
          << xifo32_get_full(&x32) << " "
          << xifo64_get_full(&x64) << endl;
    cout << "Show content: " << endl;
    cout << "From start (FIFO): " << endl;
    for(int i=0; i<5; i++){
        cout << "Read " << i << ": "
             << xifo8_read_lr(&x8,i) << " "
             << xifo16_read_lr(&x16,i) << " "
             << xifo32_read_lr(&x32,i) << " "
             << xifo64_read_lr(&x64,i) << endl;
    }
    cout << "From end (LIFO): " << endl;
    for(int i=0; i<5; i++){
        cout << "Read " << i << ": "
             << xifo8_read_mr(&x8,i) << " "
             << xifo16_read_mr(&x16,i) << " "
             << xifo32_read_mr(&x32,i) << " "
             << xifo64_read_mr(&x64,i) << endl;
    }

    cout << endl;

// Write 7 bytes in buffer
    for(int i=0; i<7; i++){
        xifo8_write(&x8,data8++);
        xifo16_write(&x16,data16++);
        xifo32_write(&x32,data32++);
        xifo64_write(&x64,data64++);
    }

    cout << "Written another 7 bytes, total 10\nBuffer should contain 15 upto 19 since its a ringbuffer" << endl;
    cout << "From start (FIFO): " << endl;
    for(int i=0; i<5; i++){
        cout << "Read " << i << ": "
             << xifo8_read_lr(&x8,i) << " "
             << xifo16_read_lr(&x16,i) << " "
             << xifo32_read_lr(&x32,i) << " "
             << xifo64_read_lr(&x64,i) << endl;
    }
    cout << "From end (LIFO): " << endl;
    for(int i=0; i<5; i++){
        cout << "Read " << i << ": "
             << xifo8_read_mr(&x8,i) << " "
             << xifo16_read_mr(&x16,i) << " "
             << xifo32_read_mr(&x32,i) << " "
             << xifo64_read_mr(&x64,i) << endl;
    }

    cout << "Free = 0 -> "
         << xifo8_get_free(&x8) << " "
         << xifo16_get_free(&x16) << " "
         << xifo32_get_free(&x32) << " "
         << xifo64_get_free(&x64) << endl;
    cout << "Used = 5 -> "
         <<  xifo8_get_used(&x8) << " "
          << xifo16_get_used(&x16) << " "
          << xifo32_get_used(&x32) << " "
          << xifo64_get_used(&x64) << endl;
    cout << "Full = 1 -> "
         <<  xifo8_get_full(&x8) << " "
          << xifo16_get_full(&x16) << " "
          << xifo32_get_full(&x32) << " "
          << xifo64_get_full(&x64) << endl;

    cout << endl;

// Lets pop
    cout << "Pop = 19 -> "
         <<  xifo8_pop_mr(&x8) << " "
          << xifo16_pop_mr(&x16) << " "
          << xifo32_pop_mr(&x32) << " "
          << xifo64_pop_mr(&x64) << endl;
    cout << "Pop = 15 -> "
         <<  xifo8_pop_lr(&x8) << " "
          << xifo16_pop_lr(&x16) << " "
          << xifo32_pop_lr(&x32) << " "
          << xifo64_pop_lr(&x64) << endl;
    cout << "Free = 2 -> "
         << xifo8_get_free(&x8) << " "
         << xifo16_get_free(&x16) << " "
         << xifo32_get_free(&x32) << " "
         << xifo64_get_free(&x64) << endl;
    cout << "Used = 3 -> "
         <<  xifo8_get_used(&x8) << " "
          << xifo16_get_used(&x16) << " "
          << xifo32_get_used(&x32) << " "
          << xifo64_get_used(&x64) << endl;
    cout << "Full = 0 -> "
         <<  xifo8_get_full(&x8) << " "
          << xifo16_get_full(&x16) << " "
          << xifo32_get_full(&x32) << " "
          << xifo64_get_full(&x64) << endl;

// Pop empty
    cout << "Pop = 19 -> "
         <<  xifo8_pop_mr(&x8) << " "
          << xifo16_pop_mr(&x16) << " "
          << xifo32_pop_mr(&x32) << " "
          << xifo64_pop_mr(&x64) << endl;
    cout << "Pop = 15 -> "
         <<  xifo8_pop_lr(&x8) << " "
          << xifo16_pop_lr(&x16) << " "
          << xifo32_pop_lr(&x32) << " "
          << xifo64_pop_lr(&x64) << endl;
// One left
    cout << "Pop = 19 -> "
         <<  xifo8_pop_mr(&x8) << " "
          << xifo16_pop_mr(&x16) << " "
          << xifo32_pop_mr(&x32) << " "
          << xifo64_pop_mr(&x64) << endl;
// Empty
    cout << "Pop = 0 (because empty) -> "
         <<  xifo8_pop_lr(&x8) << " "
          << xifo16_pop_lr(&x16) << " "
          << xifo32_pop_lr(&x32) << " "
          << xifo64_pop_lr(&x64) << endl;

    cout << "Free = 5 -> "
         << xifo8_get_free(&x8) << " "
         << xifo16_get_free(&x16) << " "
         << xifo32_get_free(&x32) << " "
         << xifo64_get_free(&x64) << endl;
    cout << "Used = 0 -> "
         <<  xifo8_get_used(&x8) << " "
          << xifo16_get_used(&x16) << " "
          << xifo32_get_used(&x32) << " "
          << xifo64_get_used(&x64) << endl;
    cout << "Full = 0 -> "
         <<  xifo8_get_full(&x8) << " "
          << xifo16_get_full(&x16) << " "
          << xifo32_get_full(&x32) << " "
          << xifo64_get_full(&x64) << endl;

    cout << "Buffer is empty:"<<endl;
    cout << "From start (FIFO): " << endl;
    for(int i=0; i<5; i++){
        cout << "Read " << i << ": "
             << xifo8_read_lr(&x8,i) << " "
             << xifo16_read_lr(&x16,i) << " "
             << xifo32_read_lr(&x32,i) << " "
             << xifo64_read_lr(&x64,i) << endl;
    }
    cout << "From end (LIFO): " << endl;
    for(int i=0; i<5; i++){
        cout << "Read " << i << ": "
             << xifo8_read_mr(&x8,i) << " "
             << xifo16_read_mr(&x16,i) << " "
             << xifo32_read_mr(&x32,i) << " "
             << xifo64_read_mr(&x64,i) << endl;
    }

    // Test for mem range error
    cout << endl << "Any of the 5 memory errors?" << endl;
    if(failure1 != TEST_PATTERN)
        cout << "YES: failure 1" << endl;
    else
        cout << "1 NO!" << endl;
    if(failure2 != TEST_PATTERN)
        cout << "YES: failure 2" << endl;
    else
        cout << "2 NO!" << endl;
    if(failure3 != TEST_PATTERN)
        cout << "YES: failure 3" << endl;
    else
        cout << "3 NO!" << endl;
    if(failure4 != TEST_PATTERN)
        cout << "YES: failure 4" << endl;
    else
        cout << "4 NO!" << endl;
    if(failure5 != TEST_PATTERN)
        cout << "YES: failure 5" << endl;
    else
        cout << "5 NO!" << endl;
    return a.exec();
}
/** @} */
