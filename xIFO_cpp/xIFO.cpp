#if 0
/**
 * @file    xifo.c
 * @brief   xifo circular buffer with template
 * @details xifo supplies object oriented circular buffer with template sized elements. \n
 * 			To use either as FIFO (First In First Out) or as FILO (First In Last Out)
 *
 * @Author	Jeroen Lodder
 * @Date	April 2013
 * @version 3
 *
 * @{
 */
#include "xifo.h"

/**
 * @brief   Initialise xifo.
 * @note    Does not clear memory pool.
 * @param[in] size  Number of elements buffer can hold (size).
 */
template <class xifo_dtype>
Xifo<xifo_dtype>::Xifo(uint32_t size) : dynamic(true)
{
    sp = new xifo_dtype[size];
    this.startpool		= sp;
    this.endpool  		= &sp[size-1];
    this.size 			= size;
    this.full 			= 0;
    this.count            = 0;
    this.read 			= sp;
    this.write			= sp;
}

/**
 * @brief   Initialise xifo.
 *
 * @note    Does not clear memory pool.
 *
 * @param[in] size  Number of elements buffer can hold (size).
 * @param[in] sp    Start of pre-allocated memory pool.
 */
template <class xifo_dtype>
Xifo<xifo_dtype>::Xifo(uint32_t size, void *pointer) : dynamic(false)
{
    register xifo_dtype *sp = pointer;
    this.startpool		= sp;
    this.endpool  		= &sp[size-1];
    this.size 			= size;
    this.full 			= 0;
    this.count            = 0;
    this.read 			= sp;
    this.write			= sp;
}

/**
 * @brief   Deinitialise (and deallocate) buffer xifo.
 *
 * @note    Does not clear memory pool.
 *
 */
template <class xifo_dtype>
Xifo<xifo_dtype>::~Xifo(void)
{
    if(dynamic){
        delete sp;
    }
}


/**
 * @brief   Clear buffer contents
 *
 * @note    Must be used on initialised buffer object.
 *
 * @param[in] c   Pointer to @p xifo_SIZETYPE_t object.
 */
template <class xifo_dtype>
void Xifo<xifo_dtype>::clear()
{
    register uint32_t *ptemp = this.startpool;
    while(ptemp <= this.endpool){
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
template <class xifo_dtype>
uint32_t Xifo<xifo_dtype>::write(xifo_dtype data)
{
    /* Write data */
    *this.write = data;
    /* Update read pointer to most recent element */
    this.read = this.write;
    /* Write pointer increment */
    this.write += 1;
    /* Validate pointer */
    if( this.write > this.endpool){
        /* Exceeded pool boundaries */
        this.write = this.startpool;
    }
    /* Update count */
    this.count++;
    /* Verify full */
    if( this.count >= this.size ){
        this.full = 1;
        this.count = this.size;
    }
    /* return free elements count */
    return this.size - this.count;
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
template <class xifo_dtype>
xifo_dtype Xifo<xifo_dtype>::read_lr(uint32_t index)
{
    register uint32_t *ptemp;
    /* Verify there is valid data to read */
    if(index+1 > this.count){
        return 0;
    }
    /* Calculate index of oldest element */
    index = (this.count-1) - index;
    /* Set pointer */
    ptemp = (this.read) - index;
    if(ptemp < this.startpool){
        /* Exceeded pool boundaries */
        /* Calculate overshoot (startpool - indexptr) and subtract from end */
        /* Since one element of overshoot results in end - 1 you would miss the last value */
        ptemp = (this.endpool+1) - (this.startpool - ptemp);
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
template <class xifo_dtype>
xifo_dtype Xifo<xifo_dtype>::pop_lr()
{
    register uint32_t *ptemp;
    register uint32_t temp;
    /* Verify there is valid data read */
    if(this.count == 0){
        return 0;
    }
    /* Derive least recent buffer element */
    ptemp = this.read+1 - this.count;
    /* Validate pointer */
    if(ptemp < this.startpool){
        /* Exceeded pool boundaries */
        /* Calculate overshoot (startpool - indexptr) and subtract from end */
        /* Since one element of overshoot results in end - 1 you would miss the last value */
        ptemp = (this.endpool+1) - (this.startpool - ptemp);
    }
    /* Read oldest buffer element */
    /* Read to temp register */
    temp = *ptemp;
    /* Empty buffer element */
    *ptemp = 0;
    /* Reduce count */
    this.count--;
    /* Check full flag */
    if(this.count < this.size)
        this.full = 0;
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
template <class xifo_dtype>
xifo_dtype Xifo<xifo_dtype>::read_mr(uint32_t index)
{
    register uint32_t *ptemp;
    /* Verify there is valid data to read */
    if(index+1 > this.count){
        return 0;
    }
    /* Set pointer */
    ptemp = (this.read) - index;
    /* Validate pointer */
    if(ptemp < this.startpool){
        /* Exceeded pool boundaries */
        /* Calculate overshoot (startpool - indexptr) and subtract from end */
        /* Since one element of overshoot results in end - 1 you would miss the last value */
        ptemp = (this.endpool+1) - (this.startpool - ptemp);
    }
    /* Read most recent */
    return *ptemp;
}

template <class xifo_dtype>
xifo_dtype Xifo<xifo_dtype>::pop_mr()
{
    register uint32_t temp;
    /* Verify there is valid data read */
    if(this.count == 0){
        return 0;
    }
    /* Read */
    temp = *this.read;
    /* Empty */
    *this.read = 0;
    /* Most recent element read, return write pointer */
    this.write = this.read;
    /* Decrement read pointer */
    this.read--;
    /* Validate pointer */
    if( this.read < this.startpool ){
        /* Exceeded pool boundaries */
        this.read = this.endpool;
    }
    /* Reduce count */
    this.count--;
    if(this.count < this.size)
        this.full = 0;
    return temp;
}
#endif
/** @} */
