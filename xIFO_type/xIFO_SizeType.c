
/**
 * @file    xifo_SizeType.c
 * @brief   xifo circular buffer for custom elements
 * @details xifo supplies object oriented circular buffer with 32 bit size elements. \n
 * 			To use either as FIFO (First In First Out) or as FILO (First In Last Out)
 *
 * @Author	Jeroen Lodder
 * @Date	April 2013
 * @version 3
 *
 * @{
 */
#include "xifo_sizetype.h"

/**
 * @brief   Initialise buffer object structure.
 *
 * @note    Does not clear memory pool.
 *
 * @param[in] c   Pointer to @p xifo_SIZETYPE_t object used for configuration.
 * @param[in] s   Number of elements buffer can hold (size).
 * @param[in] sp  Start of pre-allocated memory pool.
 */
void xifo_SIZETYPE_init(xifo_SIZETYPE_t *c, uint32_t s, xifo_SIZETYPE_pool_t *sp){
    c->startpool		= sp;
    c->endpool  		= &sp[s-1];
    c->size 			= s;
    c->full 			= 0;
    c->count 	= 0;
    c->read 			= sp;
    c->write			= sp;
}

/**
 * @brief   Clear buffer contents
 *
 * @note    Must be used on initialised buffer object.
 *
 * @param[in] c   Pointer to @p xifo_SIZETYPE_t object.
 */
void xifo_SIZETYPE_clear(xifo_SIZETYPE_t *c){
    c->ptemp = c->startpool;
    while(c->ptemp <= c->endpool){
        *c->ptemp++ = 0;
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
 * @param[in] c   		Pointer to @p xifo_SIZETYPE_t used for configuration.
 * @param[in] index   Index relative from least recent
 *
 * @return	Contents of element or 0 if failed (element can hold 0)
 */
SIZETYPE xifo_read_lr(xifo_SIZETYPE_t *c, uint32_t index){
    /* Verify there is valid data to read */
    if(index+1 > c->count){
        return 0;	/* Nothing to read there */
    }
    /* Calculate index of oldest element */
    index = (c->count-1) - index;
    /* Set pointer */
    c->ptemp = (c->read) - index;
    if(c->ptemp < c->startpool){
        /* We exceeded pool boundaries */
        /* Calculate overshoot (startpool - indexptr) and subtract from end */
        /* Since one element of overshoot results in end - 1 you would miss the last value */
        c->ptemp = (c->endpool+1) - (c->startpool - c->ptemp);
    }
    /* Read most recent */
    return *c->ptemp;
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
SIZETYPE xifo_read_mr(xifo_SIZETYPE_t *c, uint32_t index){
    /* Verify there is valid data to read */
    if(index+1 > c->count){
        return 0;	/* Nothing to read there */
    }
    /* Set pointer */
    c->ptemp = (c->read) - index;
    /* Validate pointer */
    if(c->ptemp < c->startpool){
        /* We exceeded pool boundaries */
        /* Calculate overshoot (startpool - indexptr) and subtract from end */
        /* Since one element of overshoot results in end - 1 you would miss the last value */
        c->ptemp = (c->endpool+1) - (c->startpool - c->ptemp);
    }
    /* Read most recent */
    return *c->ptemp;
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
SIZETYPE xifo_pop_mr(xifo_SIZETYPE_t *c){
    /* Verify there is valid data read */
    if(c->count == 0){
        return 0;	/* Nothing to read there */
    }
    /* Read */
    c->temp = *c->read;
    /* Empty */
    *c->read = 0;
    /* Most recent element read, return write pointer */
    c->write = c->read;
    /* Decrement read pointer */
    c->read--;
    /* Validate pointer */
    if( c->read < c->startpool ){
        /* We exceeded pool boundaries */
        c->read = c->endpool;
    }
    /* Reduce count */
    c->count--;
    if(c->count < c->size)
        c->full = 0;
    return c->temp;
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
SIZETYPE xifo_pop_lr(xifo_SIZETYPE_t *c){
    /* Verify there is valid data read */
    if(c->count == 0){
        return 0;	/* Nothing to read there */
    }
    /* Derive least recent buffer element */
    c->ptemp = c->read+1 - c->count;
    /* Validate pointer */
    if(c->ptemp < c->startpool){
        /* We exceeded pool boundaries */
        /* Calculate overshoot (startpool - indexptr) and subtract from end */
        /* Since one element of overshoot results in end - 1 you would miss the last value */
        c->ptemp = (c->endpool+1) - (c->startpool - c->ptemp);
    }
    /* Read oldest buffer element */
    { /* block with temporary variable to prevent stack use */
        register uint32_t element;
        /* Read to temp register */
        element = *c->ptemp;
        /* Empty */
        *c->ptemp = 0;
        /* Clear temp register */
        c->temp = element;
    }
    /* Reduce count */
    c->count--;
    /* Check full flag */
    if(c->count < c->size)
        c->full = 0;
    return c->temp;
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
uint32_t xifo_write(xifo_SIZETYPE_t *c, SIZETYPE data){
    /* Write data */
    *c->write = data;
    /* Update read pointer to most recent element */
    c->read = c->write;
    /* Write pointer increment */
    c->write += 1;
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
 * @param[in] c	Pointer to @p xifo_SIZETYPE_t used for configuration.
 *
 * @return	Size of memory pool in elements
 */
uint32_t xifo_get_size(xifo_SIZETYPE_t *c){
    return c->size;
}

/**
 * @brief   Get number of used elements
 *
 * @param[in] c	Pointer to @p xifo_SIZETYPE_t used for configuration.
 *
 * @return	Number of used buffer elements
 */
uint32_t xifo_get_used(xifo_SIZETYPE_t *c){
    return c->count;
}

/**
* @brief   Get number of free elements
*
* @param[in] c	Pointer to @p xifo_SIZETYPE_t used for configuration.
*
* @return	Number of free elements
*/
uint32_t xifo_get_free(xifo_SIZETYPE_t *c){
    return c->size - c->count;
}

/**
 * @brief   Get full flag
 *
 * @param[in] c	Pointer to @p xifo_SIZETYPE_t used for configuration.
 *
 * @return	1 if full
 */
uint32_t xifo_get_full(xifo_SIZETYPE_t *c){
    return c->full;
}

/** @} */
