/**
 * @file    xifo.h
 * @brief   xifo circular buffer for all elements
 * @details xifo supplies object oriented circular buffer with x bit size elements. \n
 * 			To use either as FIFO (First In First Out) or as FILO (First In Last Out)
 *			Below are defines to disable or enable any xIFO type you like.
 *
 * @author	Jeroen Lodder
 * @date	March 2014
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
#ifndef _xifo_H_
#define _xifo_H_
 
#include <inttypes.h>

#if !defined(TRUE) || defined(__DOXYGEN__)
#define TRUE 	(1)
#endif
#if !defined(FALSE) || defined(__DOXYGEN__)
#define FALSE 	!(TRUE)
#endif
 
#ifdef __cplusplus
#if !defined(xIFO_USE_CPP) || defined(__DOXYGEN__)
#define xIFO_USE_CPP			TRUE
#endif
#endif

#if !defined(xIFO_USE_64BIT) || defined(__DOXYGEN__)
#define xIFO_USE_64BIT			TRUE
#endif

#if !defined(xIFO_USE_32BIT) || defined(__DOXYGEN__)
#define xIFO_USE_32BIT			TRUE
#endif

#if !defined(xIFO_USE_16BIT) || defined(__DOXYGEN__)
#define xIFO_USE_16BIT			TRUE
#endif

#if !defined(xIFO_USE_8BIT) || defined(__DOXYGEN__)
#define xIFO_USE_8BIT			TRUE
#endif

#if !defined(xIFO_USE_32FLOAT) || defined(__DOXYGEN__)
#define xIFO_USE_32FLOAT		TRUE
#endif

#ifdef __cplusplus
#if xIFO_USE_CPP == TRUE
/**
 * @brief   Circular Buffer object.
 * @details This class holds the object of a circular buffer
 */
template <class xifo_dtype>
class Xifo
{
public:
    /**
     * @brief   Initialise xifo.
     * @note    Does not clear memory pool.
     * @param[in] Number of elements buffer can hold (size).
     */
    Xifo(uint32_t size)
    {
        startpool = new xifo_dtype[size];
        endpool  		= &startpool[size-1];
        isize 			= size;
        ifull 			= 0;
        icount           = 0;
        read 			= startpool;
        pwrite			= startpool;
    }

    /**
     * @brief   Initialise xifo.
     * @note    Does not clear memory pool.
     * @param[in] Number of elements buffer can hold (size).
     * @param[in] Start of pre-allocated memory pool.
     */
    Xifo(uint32_t size, xifo_dtype *sp)
    {
        startpool		= sp;
        endpool  		= &sp[size-1];
        isize 			= size;
        ifull 			= 0;
        icount            = 0;
        read 			= sp;
        pwrite			= sp;
    }

    /**
     * @brief   Deinitialise (and deallocate) buffer xifo.
     * @note    Does not clear memory pool.
     */
    ~Xifo(void)
    {
        if(dynamic){
            delete startpool;
        }
    }

    /**
     * @brief   Clear buffer memory pool
     * @note    Must be used on initialised buffer object.
     * @param[in] c   Pointer to @p xifo_SIZETYPE_t object.
     */
    void clear(void)
    {
        register xifo_dtype *ptemp = startpool;
        while(ptemp <= endpool){
            *ptemp++ = 0;
        }
    }
	
	 /**
     * @brief  	Reset buffer
     * @note    Must be used on initialised buffer object.
     * @param[in] c   Pointer to @p xifo_SIZETYPE_t object.
     */
    void reset(void)
    {
        register xifo_dtype *ptemp = startpool;
        while(ptemp <= endpool){
            *ptemp++ = 0;
        }
		ifull 		= 0;
        icount      = 0;
        read 		= startpool;
        pwrite		= startpool;
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
     * @param[in] c			Pointer to @p xifo_SIZETYPE_t used for configuration.
     * @param[in] data	Data to add to buffer
     *
     * @return	Number of free buffer elements
     */
    uint32_t write(xifo_dtype data)
    {
        /* Write data */
        *pwrite = data;
        /* Update read pointer to most recent element */
        read = pwrite;
        /* Write pointer increment */
        pwrite += 1;
        /* Validate pointer */
        if( pwrite > endpool){
            /* Exceeded pool boundaries */
            pwrite = startpool;
        }
        /* Update count */
        icount++;
        /* Verify full */
        if( icount >= isize ){
            ifull = 1;
            icount = isize;
        }
        /* return free elements count */
        return isize - icount;
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
     * @param[in] c   		Pointer to @p xifo_SIZETYPE_t used for configuration.
     * @param[in] index   Index relative from least recent
     *
     * @return	Contents of element or 0 if failed (element can hold 0)
     */
    xifo_dtype read_lr(uint32_t index)
    {
        register xifo_dtype *ptemp;
        /* Verify there is valid data to read */
        if(index+1 > icount){
            return 0;
        }
        /* Calculate index of oldest element */
        index = (icount-1) - index;
        /* Set pointer */
        ptemp = (read) - index;
        if(ptemp < startpool){
            /* Exceeded pool boundaries */
            /* Calculate overshoot (startpool - indexptr) and subtract from end */
            /* Since one element of overshoot results in end - 1 you would miss the last value */
            ptemp = (endpool+1) - (startpool - ptemp);
        }
        /* Read most recent */
        return *ptemp;
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
     * @param[in] c	Pointer to @p xifo_SIZETYPE_t used for configuration.
     *
     * @return	Contents of element or 0 if failed (element can hold 0)
     */
    xifo_dtype pop_lr()
    {
        register xifo_dtype *ptemp;
        xifo_dtype temp;
        /* Verify there is valid data read */
        if(icount == 0){
            return 0;
        }
        /* Derive least recent buffer element */
        ptemp = read+1 - icount;
        /* Validate pointer */
        if(ptemp < startpool){
            /* Exceeded pool boundaries */
            /* Calculate overshoot (startpool - indexptr) and subtract from end */
            /* Since one element of overshoot results in end - 1 you would miss the last value */
            ptemp = (endpool+1) - (startpool - ptemp);
        }
        /* Read oldest buffer element */
        /* Read to temp register */
        temp = *ptemp;
        /* Empty buffer element */
        *ptemp = 0;
        /* Reduce count */
        icount--;
        /* Check full flag */
        if(icount < isize)
            ifull = 0;
        return temp;
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
     * @param[in] c   		Pointer to @p xifo_SIZETYPE_t used for configuration.
     * @param[in] index   Index relative from most recent
     *
     * @return	Contents of element or 0 if failed (element can hold 0)
     */
    xifo_dtype read_mr(uint32_t index)
    {
        register xifo_dtype *ptemp;
        /* Verify there is valid data to read */
        if(index+1 > icount){
            return 0;
        }
        /* Set pointer */
        ptemp = read - index;
        /* Validate pointer */
        if(ptemp < startpool){
            /* Exceeded pool boundaries */
            /* Calculate overshoot (startpool - indexptr) and subtract from end */
            /* Since one element of overshoot results in end - 1 you would miss the last value */
            ptemp = (endpool+1) - (startpool - ptemp);
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
     * @param[in] c   		Pointer to @p xifo_SIZETYPE_t used for configuration.
     *
     * @return	Contents of element or 0 if failed (element can hold 0)
     */
    xifo_dtype pop_mr()
    {
        register xifo_dtype temp;
        /* Verify there is valid data read */
        if(icount == 0){
            return 0;
        }
        /* Read */
        temp = *read;
        /* Empty */
        *read = 0;
        /* Most recent element read, return write pointer */
        pwrite = read;
        /* Decrement read pointer */
        read--;
        /* Validate pointer */
        if( read < startpool ){
            /* Exceeded pool boundaries */
            read = endpool;
        }
        /* Reduce count */
        icount--;
        if(icount < isize)
            ifull = 0;
        return temp;
    }

    /* Extractors */
    uint32_t size(){ return isize; }         /**< @brief   Get buffer size */
    uint32_t used(){ return icount; }        /**< @brief   Get number of used elements */
    uint32_t full(){ return ifull; }         /**< @brief   Get full flag */
    uint32_t free(){ return isize-icount; }   /**< @brief   Get number of free elements */
private:
    bool dynamic;
    xifo_dtype *startpool;         /**< @brief First element in pool */
    xifo_dtype *endpool;    /**< @brief Last element in pool */
    xifo_dtype *read;       /**< @brief Read pointer */
    xifo_dtype *pwrite;     /**< @brief Write pointer */
    /* Variables: */
    uint32_t ifull;          /**< @brief Flag indicating buffer is full */
    uint32_t icount;         /**< @brief Number of elements used */
    uint32_t isize;          /**< @brief Size of buffer */
};
#endif
#endif

#ifdef __cplusplus
extern "C" {
#endif
#if xIFO_USE_64BIT == TRUE
/**
 * @brief   Circular Buffer object.
 * @details This struct holds the object of a circular buffer
 */
typedef struct  {
    /* Pointers: */
    uint64_t *startpool;    /**< @brief First element in pool */
    uint64_t *endpool;      /**< @brief Last element in pool */
    uint64_t *read;			/**< @brief Read pointer */
    uint64_t *write;		/**< @brief Write pointer */
    /* Variables: */
    uint32_t full;          /**< @brief Flag indicating buffer is full */
    uint32_t count;         /**< @brief Number of elements used */
    uint32_t size;			/**< @brief Size of buffer */
}xifo64_t;

/**< @brief   Circular Buffer memory pool type. */
typedef uint64_t xifo64_pool_t;

/* xifo Common */
void xifo64_init( xifo64_t *c, uint32_t size, uint64_t *startpool );
void xifo64_clear( xifo64_t *c );
uint32_t xifo64_write( xifo64_t *c, uint64_t data );
/* FIFO use */
uint64_t xifo64_read_lr( xifo64_t *c, uint32_t index );
uint64_t xifo64_pop_lr( xifo64_t *c );
/* LIFO use */
uint64_t xifo64_read_mr( xifo64_t *c, uint32_t index );
uint64_t xifo64_pop_mr( xifo64_t *c );
/* Extractors */
uint32_t xifo64_get_size( xifo64_t *c );
uint32_t xifo64_get_used( xifo64_t *c );
uint32_t xifo64_get_full( xifo64_t *c );
uint32_t xifo64_get_free( xifo64_t *c );
#endif

#if xIFO_USE_32BIT == TRUE
/**
 * @brief   Circular Buffer object.
 * @details This struct holds the object of a circular buffer
 */
typedef struct  {
    /* Pointers: */
    uint32_t *startpool;    /**< @brief First element in pool */
    uint32_t *endpool;      /**< @brief Last element in pool */
    uint32_t *read;			/**< @brief Read pointer */
    uint32_t *write;		/**< @brief Write pointer */
    /* Variables: */
    uint32_t full;          /**< @brief Flag indicating buffer is full */
    uint32_t count;         /**< @brief Number of elements used */
    uint32_t size;			/**< @brief Size of buffer */
}xifo32_t;

/**< @brief   Circular Buffer memory pool type. */
typedef uint32_t xifo32_pool_t;

/* xifo Common */
void xifo32_init( xifo32_t *c, uint32_t size, uint32_t *startpool );
void xifo32_clear( xifo32_t *c );
uint32_t xifo32_write( xifo32_t *c, uint32_t data );
/* FIFO use */
uint32_t xifo32_read_lr( xifo32_t *c, uint32_t index );
uint32_t xifo32_pop_lr( xifo32_t *c );
/* LIFO use */
uint32_t xifo32_read_mr( xifo32_t *c, uint32_t index );
uint32_t xifo32_pop_mr( xifo32_t *c );
/* Extractors */
uint32_t xifo32_get_size( xifo32_t *c );
uint32_t xifo32_get_used( xifo32_t *c );
uint32_t xifo32_get_full( xifo32_t *c );
uint32_t xifo32_get_free( xifo32_t *c );
#endif

#if xIFO_USE_16BIT == TRUE
/**
 * @brief   Circular Buffer object.
 * @details This struct holds the object of a circular buffer
 */
typedef struct  {
    /* Pointers: */
    uint16_t *startpool;    /**< @brief First element in pool */
    uint16_t *endpool;      /**< @brief Last element in pool */
    uint16_t *read;         /**< @brief Read pointer */
    uint16_t *write;        /**< @brief Write pointer */
    /* Variables: */
    uint32_t full;          /**< @brief Flag indicating buffer is full */
    uint32_t count;         /**< @brief Number of elements used */
    uint32_t size;          /**< @brief Size of buffer */
}xifo16_t;

/**
 * @brief   Circular Buffer memory pool type.
 */
typedef uint16_t xifo16_pool_t;

/* xifo Common */
void xifo16_init( xifo16_t *c, uint32_t size, uint16_t *startpool);
void xifo16_clear( xifo16_t *c);
uint32_t xifo16_write( xifo16_t *c, uint16_t data);
/* FIFO use */
uint16_t xifo16_read_lr( xifo16_t *c, uint32_t index);
uint16_t xifo16_pop_lr( xifo16_t *c);
/* LIFO use */
uint16_t xifo16_read_mr( xifo16_t *c, uint32_t index);
uint16_t xifo16_pop_mr( xifo16_t *c);
/* Extractors */
uint32_t xifo16_get_size( xifo16_t *c);
uint32_t xifo16_get_used( xifo16_t *c);
uint32_t xifo16_get_full( xifo16_t *c);
uint32_t xifo16_get_free( xifo16_t *c);
#endif

#if xIFO_USE_8BIT == TRUE
/**
 * @brief   Circular Buffer object.
 * @details This struct holds the object of a circular buffer
 */
typedef struct  {
    /* Pointers: */
    uint8_t *startpool;     /**< @brief First element in pool */
    uint8_t *endpool;       /**< @brief Last element in pool */
    uint8_t *read;          /**< @brief Read pointer */
    uint8_t *write;         /**< @brief Write pointer */
    /* Variables: */
    uint32_t full;          /**< @brief Flag indicating buffer is full */
    uint32_t count;         /**< @brief Number of elements used */
    uint32_t size;			/**< @brief Size of buffer */
}xifo8_t;

/**< @brief   Circular Buffer memory pool type. */
typedef uint8_t xifo8_pool_t;

/* xifo Common */
void xifo8_init(xifo8_t *c, uint32_t size, uint8_t *startpool );
void xifo8_clear( xifo8_t *c );
uint32_t xifo8_write( xifo8_t *c, uint8_t data );
/* FIFO use */
uint8_t xifo8_read_lr( xifo8_t *c, uint32_t index );
uint8_t xifo8_pop_lr( xifo8_t *c );
/* LIFO use */
uint8_t xifo8_read_mr( xifo8_t *c, uint32_t index );
uint8_t xifo8_pop_mr( xifo8_t *c );
/* Extractors */
uint32_t xifo8_get_size( xifo8_t *c );
uint32_t xifo8_get_used( xifo8_t *c );
uint32_t xifo8_get_full( xifo8_t *c );
uint32_t xifo8_get_free( xifo8_t *c );
#endif

#if xIFO_USE_32FLOAT == TRUE
/**
 * @brief   Circular Buffer object.
 * @details This struct holds the object of a circular buffer
 */
typedef struct  {
    /* Pointers: */
    float *startpool;       /**< @brief First element in pool */
    float *endpool;         /**< @brief Last element in pool */
    float *read;			/**< @brief Read pointer */
    float *write;		    /**< @brief Write pointer */
    /* Variables: */
    uint32_t full;          /**< @brief Flag indicating buffer is full */
    uint32_t count;         /**< @brief Number of elements used */
    uint32_t size;			/**< @brief Size of buffer */
}xifo32f_t;

/**< @brief   Circular Buffer memory pool type. */
typedef float xifo32f_pool_t;

/* xifo Common */
void xifo32f_init( xifo32f_t *c, uint32_t size, float *startpool );
void xifo32f_clear( xifo32f_t *c );
uint32_t xifo32f_write( xifo32f_t *c, float data );
/* FIFO use */
float xifo32f_read_lr( xifo32f_t *c, uint32_t index );
float xifo32f_pop_lr( xifo32f_t *c );
/* LIFO use */
float xifo32f_read_mr( xifo32f_t *c, uint32_t index );
float xifo32f_pop_mr( xifo32f_t *c );
/* Extractors */
uint32_t xifo32f_get_size( xifo32f_t *c );
uint32_t xifo32f_get_used( xifo32f_t *c );
uint32_t xifo32f_get_full( xifo32f_t *c );
uint32_t xifo32f_get_free( xifo32f_t *c );
#endif

#ifdef __cplusplus
}
#endif
#endif //_xifo_H_

/** @} */
