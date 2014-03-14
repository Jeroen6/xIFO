
/**
 * @file    xifo_32.h
 * @brief   xifo circular buffer with 32 bit elements
 * @details xifo supplies object oriented circular buffer with 32 bit size elements. \n
 * 			To use either as FIFO (First In First Out ) or as FILO (First In Last Out )
 *
 * @author	Jeroen Lodder
 * @date	April 2013
 * @version 3
 *
 * @{
 */

#ifndef _xifo_32_H_
#define _xifo_32_H_

#include <inttypes.h>

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

#ifdef __cplusplus
extern "C" {
#endif
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
#ifdef __cplusplus
}
#endif
#endif //_xifo_32_H_

/** @} */
