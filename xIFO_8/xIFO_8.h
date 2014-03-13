
/**
 * @file    xifo_8.h
 * @brief   xifo circular buffer with 8 bit elements
 * @details xifo supplies object oriented circular buffer with 32 bit size elements. \n
 * 			To use either as FIFO (First In First Out ) or as FILO (First In Last Out )
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

#ifndef _xifo_8_H_
#define _xifo_8_H_

#include <inttypes.h>

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
    uint32_t count;  /**< @brief Number of elements used */
    uint32_t size;			/**< @brief Size of buffer */
}xifo8_t;

/**< @brief   Circular Buffer memory pool type. */
typedef uint8_t xifo8_pool_t;

#ifdef __cplusplus
extern "C" {
#endif
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
#ifdef __cplusplus
}
#endif
#endif //_xifo_32_H_

/** @} */
