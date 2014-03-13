
/**
 * @file    xifo_16.h
 * @brief   xifo circular buffer with 16 bit elements
 * @details xifo supplies object oriented circular buffer with 32 bit size elements. \n
 * 			To use either as FIFO (First In First Out) or as FILO (First In Last Out)
 *
 * @author	Jeroen Lodder
 * @date	April 2013
 * @version 3
 *
 * @{
 */

/**
 *	Code has been tested on Cortex M0 (LPC1114)
 *  Performance table, number of core clocks
 *	Measured with a timer before and after
 *	r1 = timer->TC
 *		test_function
 *  r2 = timer->TC
 *
 *  Function	Speed		Worst Case
 *
 *  write		0       0
 *  read_mr		0       0
 *  read_lr		0       0
 *  pop_mr		0       0
 *	pop_lr		0       0
 *
 */

#ifndef _xifo_16_H_
#define _xifo_16_H_

#include <inttypes.h>

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
    uint32_t count;  /**< @brief Number of elements used */
    uint32_t size;          /**< @brief Size of buffer */
}xifo16_t;

/**
 * @brief   Circular Buffer memory pool type.
 */
typedef uint16_t xifo16_pool_t;

#ifdef __cplusplus
extern "C" {
#endif
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
#ifdef __cplusplus
}
#endif
#endif //_xifo_16_H_

/** @} */
