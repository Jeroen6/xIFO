
/**
 * @file    xifo_SizeType.h
 * @brief   xifo circular buffer for custom elements
 * @details xifo supplies object oriented circular buffer with 32 bit size elements. \n
 * 			To use either as FIFO (First In First Out) or as FILO (First In Last Out)
 *
 * @author	Jeroen Lodder
 * @date	April 2013
 * @version 3
 *
 * @{
 */

#ifndef _xifo_H_
#define _xifo_H_
#include <inttypes.h>

/**
 * @brief   Circular Buffer object.
 * @details This struct holds the object of a circular buffer
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
     * @brief   Clear buffer contents
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


#endif //_xifo_H_

/** @} */
