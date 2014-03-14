
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

//#define CUSTOMIZED    /** @brief Uncomment this line to unlock the file */

#if !defined(CUSTOMIZED) || defined(__DOXYGEN__)
#warning  This file is for reference purpose only!!
#warning  Please change every occurance of SIZETYPE, rename the file, and remove this message
#endif

#ifndef _xifo_SIZETYPE_H_
#define _xifo_SIZETYPE_H_
#include <inttypes.h>
#define SIZETYPE    size_t

#if defined(__arm__)  || defined(__DOXYGEN__)
#pragma anon_unions				/**< @brief Allow unnamed unions */
#endif

/**
 * @brief   Circular Buffer object.
 * @details This struct holds the object of a circular buffer
 */
typedef struct  {
    /* Pointers: */
    SIZETYPE *startpool;    /**< @brief First element in pool */
    SIZETYPE *endpool;      /**< @brief Last element in pool */
    SIZETYPE *read;         /**< @brief Read pointer */
    SIZETYPE *write;        /**< @brief Write pointer */
    /* Variables: */
    uint32_t full;          /**< @brief Flag indicating buffer is full */
    uint32_t count;  /**< @brief Number of elements used */
    uint32_t size;			/**< @brief Size of buffer */
}xifo_SIZETYPE_t;

/**< @brief   Circular Buffer memory pool type. */
typedef SIZETYPE xifo_SIZETYPE_pool_t;

#ifdef __cplusplus
extern "C" {
#endif
/* xifo Common */
void xifo_init(xifo_SIZETYPE_t *c, uint32_t size, xifo_SIZETYPE_pool_t *startpool);
void xifo_clear(xifo_SIZETYPE_t *c);
uint32_t xifo_write(xifo_SIZETYPE_t *c, SIZETYPE data);
/* FIFO use */
SIZETYPE xifo_read_lr(xifo_SIZETYPE_t *c, uint32_t index);
SIZETYPE xifo_pop_lr(xifo_SIZETYPE_t *c);
/* LIFO use */
SIZETYPE xifo_read_mr(xifo_SIZETYPE_t *c, uint32_t index);
SIZETYPE xifo_pop_mr(xifo_SIZETYPE_t *c);
/* Extractors */
uint32_t xifo_get_size(xifo_SIZETYPE_t *c);
uint32_t xifo_get_used(xifo_SIZETYPE_t *c);
uint32_t xifo_get_full(xifo_SIZETYPE_t *c);
uint32_t xifo_get_free(xifo_SIZETYPE_t *c);
#ifdef __cplusplus
}
#endif
#endif //_xifo_SIZETYPE_H_

/** @} */
