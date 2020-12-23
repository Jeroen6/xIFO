
/**
 * @file    xifo.c
 * @brief   xifo circular buffer with <t>/8/16/32/64 bit elements
 * @details xifo supplies object oriented circular buffer with 8 bit size elements. \n
 * 			To use either as FIFO (First In First Out) or as FILO (First In Last Out)
 * 			You might want to rename this file is you are using a C compiler.
 *
 * @Author	Jeroen Lodder
 * @Date	March 2014
 * @version 3
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
#include "xifo.h"

#if xIFO_USE_32BIT == TRUE
/**
 * @brief   Initialize buffer object structure.
 *
 * @note    Does not clear memory pool.
 *
 * @param[in] c   Pointer to @p xifo32_t object used for configuration.
 * @param[in] s   Number of elements buffer can hold (size).
 * @param[in] sp  Start of pre-allocated memory pool.
 */
void xifo32_init(xifo32_t *c, uint32_t s, uint32_t *sp){
    c->startpool		= sp;
    c->size 			= s;
    c->endpool  		= &sp[--s];
    c->full 			= 0;
    c->count            = 0;
    c->read 			= sp;
    c->write			= sp;
}

/**
 * @brief   Clear buffer memory pool
 *
 * @note    Must be used on initialised buffer object.
 *
 * @param[in] c   Pointer to @p xifo32_t object.
 */
void xifo32_clear(xifo32_t *c){
    register uint32_t *ptemp = c->startpool;
    register uint32_t i = c->size;
    while(i--){
        *ptemp++ = 0;
    }
}

/**
 * @brief   Read from buffer (lr) Least Recent oriented (fifo)
 *
 * @note    Buffer state will be preserved
 *
 * @warning	Consider this opertaion as atomic!
 *
 * @details Read n elements from the oldest element to the most recent.
 *					As for index[0] the least recently added element is returned.
 *					And for index[count] the most recent element is returned.
 *					This makes it possible to peek in fifo.
 *
 * @param[in] c   		Pointer to @p xifo32_t used for configuration.
 * @param[in] index   Index relative from least recent
 *
 * @return	Contents of element or 0 if failed (element can hold 0)
 */
uint32_t xifo32_read_lr(xifo32_t *c, uint32_t index){
    register uint32_t *ptemp;
    /* Verify there is valid data to read */
    if(index >= c->count){
        return 0;	/* Nothing to read there */
    }
    /* Calculate index of oldest element */
    index = (c->count-1) - index;
    /* Set pointer */
    ptemp = (c->read) - index;
    if(ptemp < c->startpool){
        /* Exceeded pool boundaries */
        /* Calculate overshoot (startpool - indexptr) and subtract from end */
        /* Since one element of overshoot results in end - 1 you would miss the last value */
        ptemp = (c->endpool+1) - (c->startpool - ptemp);
    }
    /* Read most recent */
    return *ptemp;
}

/**
 * @brief   Read from buffer (mr) Most Recent oriented (filo)
 *
 * @note    Buffer state will be preserved
 *
 * @warning	Consider this opertaion as atomic!
 *
 * @details Read n elements back in time.
 *					As for index[0] the most recently added element is returned.
 *					And for index[count] the oldest element is returned.
 *					This makes it possible to keep history. For DSP application.
 *
 * @param[in] c   		Pointer to @p xifo32_t used for configuration.
 * @param[in] index   Index relative from most recent
 *
 * @return	Contents of element or 0 if failed (element can hold 0)
 */
uint32_t xifo32_read_mr(xifo32_t *c, uint32_t index){
    register uint32_t *ptemp;
    /* Verify there is valid data to read */
    if(index >= c->count){
        return 0;	/* Nothing to read there */
    }
    /* Set pointer */
    ptemp = (c->read) - index;
    /* Validate pointer */
    if(ptemp < c->startpool){
        /* Exceeded pool boundaries */
        /* Calculate overshoot (startpool - indexptr) and subtract from end */
        /* Since one element of overshoot results in end - 1 you would miss the last value */
        ptemp = (c->endpool+1) - (c->startpool - ptemp);
    }
    /* Read most recent */
    return *ptemp;
}

/**
 * @brief   Pop (mr) most recent from buffer (filo)
 *
 * @note    Buffer state will be altered
 *
 * @warning	Consider this opertaion as atomic!
 *
 * @details Read and remove the most recently added from the buffer.
 * 					Using this results in a stack type of buffer.
 *
 * @param[in] c   		Pointer to @p xifo32_t used for configuration.
 *
 * @return	Contents of element or 0 if failed (element can hold 0)
 */
uint32_t xifo32_pop_mr(xifo32_t *c){
    register uint32_t temp;
    /* Verify there is valid data read */
    if(c->count == 0){
        return 0;	/* Nothing to read there */
    }
    /* Read */
    temp = *c->read;
    /* Empty */
    *c->read = 0;
    /* Most recent element read, return write pointer */
    c->write = c->read;
    /* Decrement read pointer */
    c->read--;
    /* Validate pointer */
    if( c->read < c->startpool ){
        /* Exceeded pool boundaries */
        c->read = c->endpool;
    }
    /* Reduce count */
    c->count--;
    if(c->count < c->size)
        c->full = 0;
    return temp;
}

/**
 * @brief   Pop (lr) least recent from buffer (fifo)
 *
 * @note    Buffer state will be altered
 *
 * @warning	Consider this opertaion as atomic!
 *
 * @details Read and remove the least recently added from the buffer.
 * 					Using this results in a fifo type of buffer.
 *
 * @param[in] c	Pointer to @p xifo32_t used for configuration.
 *
 * @return	Contents of element or 0 if failed (element can hold 0)
 */
uint32_t xifo32_pop_lr(xifo32_t *c){
    register uint32_t *ptemp;
    register uint32_t temp;
    /* Verify there is valid data read */
    if(c->count == 0){
        return 0;	/* Nothing to read there */
    }
    /* Derive least recent buffer element */
    ptemp = (c->read+1) - c->count;
    /* Validate pointer */
    if(ptemp < c->startpool){
        /* Exceeded pool boundaries */
        /* Calculate overshoot (startpool - indexptr) and subtract from end */
        /* Since one element of overshoot results in end - 1 you would miss the last value */
        ptemp = (c->endpool+1) - (c->startpool - ptemp);
    }
    /* Read oldest buffer element */
    /* Read to temp register */
    temp = *ptemp;
    /* Empty buffer element */
    *ptemp = 0;
    /* Reduce count */
    c->count--;
    /* Check full flag */
    if(c->count < c->size)
        c->full = 0;
    return temp;
}

/**
 * @brief   Write to buffer
 *
 * @note    Readpointer is automatically set to the last added element.
 *
 * @warning	Consider this opertaion as atomic!
 *
 * @details Adds a value to the buffer.
 *					Automatically overwrites oldest elements when full.
 *
 * @param[in] c			Pointer to @p xifo32_t used for configuration.
 * @param[in] data	Data to add to buffer
 *
 * @return	Number of free buffer elements
 */
uint32_t xifo32_write(xifo32_t *c, uint32_t data){
    /* Write data */
    *c->write = data;
    /* Update read pointer to most recent element */
    c->read = c->write;
    /* Write pointer increment */
    c->write++;
    /* Validate pointer */
    if( c->write > c->endpool){
        /* We exceeded pool boundaries */
        c->write = c->startpool;
    }
    /* Update count */
    c->count++;
    /* Verify full */
    if( c->count >= c->size ){
        c->full = 1;
        c->count = c->size;
    }
    /* return free elements count */
    return c->size - c->count;
}

/**
 * @brief   Get buffer size
 *
 * @param[in] c	Pointer to @p xifo32_t used for configuration.
 *
 * @return	Size of memory pool in elements
 */
uint32_t xifo32_get_size(xifo32_t *c){
    return c->size;
}

/**
 * @brief   Get number of used elements
 *
 * @param[in] c	Pointer to @p xifo32_t used for configuration.
 *
 * @return	Number of used buffer elements
 */
uint32_t xifo32_get_used(xifo32_t *c){
    return c->count;
}

/**
* @brief   Get number of free elements
*
* @param[in] c	Pointer to @p xifo32_t used for configuration.
*
* @return	Number of free elements
*/
uint32_t xifo32_get_free(xifo32_t *c){
    return c->size - c->count;
}

/**
 * @brief   Get full flag
 *
 * @param[in] c	Pointer to @p xifo32_t used for configuration.
 *
 * @return	1 if full
 */
uint32_t xifo32_get_full(xifo32_t *c){
    return c->full;
}

/** @} */
#endif