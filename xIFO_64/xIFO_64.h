
/**
 * @file    xifo_64.h
 * @brief   xifo circular buffer with 16 bit elements
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

#ifndef _xifo_64_H_
#define _xifo_64_H_

#include <inttypes.h>

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
    uint32_t count;  /**< @brief Number of elements used */
    uint32_t size;			/**< @brief Size of buffer */
}xifo64_t;

/**< @brief   Circular Buffer memory pool type. */
typedef uint64_t xifo64_pool_t;

#ifdef __cplusplus
extern "C" {
#endif
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
#ifdef __cplusplus
}
#endif
#endif //_xifo_32_H_

/** @} */
