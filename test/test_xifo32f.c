#include "unity.h"
#include "xifo.h"

TEST_FILE("xifo32f.c")

xifo32f_t xifo;
xifo32f_pool_t buffer[5];

float testdata[10] ={
    1.1,
    2.2,
    3.3,
    4.4,
    5.5,
    6.6,
    7.7,
    8.8,
    9.9,
    0.0
};

void setUp(void)
{
    xifo32f_init(&xifo, 5, buffer);
    TEST_ASSERT_EQUAL(xifo.size, 5);
}

void tearDown(void)
{
}

void testXifoClear(void){
    xifo32f_clear(&xifo);
    TEST_ASSERT_EQUAL(0, buffer[1]);
}

void testWriteUntilFull(void){
    TEST_ASSERT_TRUE(xifo32f_write(&xifo, testdata[0]));
    TEST_ASSERT_TRUE(xifo32f_write(&xifo, testdata[1]));
    TEST_ASSERT_TRUE(xifo32f_write(&xifo, testdata[2]));
    TEST_ASSERT_TRUE(xifo32f_write(&xifo, testdata[3]));
    TEST_ASSERT_FALSE(xifo32f_write(&xifo, testdata[4]));
    TEST_ASSERT_EQUAL(xifo32f_pop_lr(&xifo), testdata[0]);
    TEST_ASSERT_EQUAL(xifo32f_pop_lr(&xifo), testdata[1]);
    TEST_ASSERT_EQUAL(xifo32f_pop_lr(&xifo), testdata[2]);
    TEST_ASSERT_EQUAL(xifo32f_pop_lr(&xifo), testdata[3]);
    TEST_ASSERT_EQUAL(xifo32f_pop_lr(&xifo), testdata[4]);
}

void testWriteAndReadUntilFull(void){
    TEST_ASSERT_TRUE(xifo32f_write(&xifo, testdata[0]));
    TEST_ASSERT_TRUE(xifo32f_write(&xifo, testdata[1]));
    TEST_ASSERT_TRUE(xifo32f_write(&xifo, testdata[2]));
    TEST_ASSERT_TRUE(xifo32f_write(&xifo, testdata[3]));
    TEST_ASSERT_FALSE(xifo32f_write(&xifo, testdata[4]));
    TEST_ASSERT_FALSE(xifo32f_write(&xifo, testdata[5]));
    TEST_ASSERT_FALSE(xifo32f_write(&xifo, testdata[6]));
    TEST_ASSERT_EQUAL(xifo32f_pop_lr(&xifo), testdata[2]);
    TEST_ASSERT_EQUAL(xifo32f_pop_lr(&xifo), testdata[3]);
    TEST_ASSERT_EQUAL(xifo32f_pop_lr(&xifo), testdata[4]);
    TEST_ASSERT_EQUAL(xifo32f_pop_lr(&xifo), testdata[5]);
    TEST_ASSERT_EQUAL(xifo32f_pop_lr(&xifo), testdata[6]);

    TEST_ASSERT_EQUAL(xifo32f_pop_lr(&xifo), 0);
}

void test_module_generator_needs_to_be_implemented(void)
{
    TEST_IGNORE_MESSAGE("Implement me!");
}

// void xifo32f_init( xifo32f_t *c, uint32_t size, float *startpool );
// void xifo32f_clear( xifo32f_t *c );
// uint32_t xifo32f_write( xifo32f_t *c, float data );
// /* FIFO use */
// float xifo32f_read_lr( xifo32f_t *c, uint32_t index );
// float xifo32f_pop_lr( xifo32f_t *c );
// /* LIFO use */
// float xifo32f_read_mr( xifo32f_t *c, uint32_t index );
// float xifo32f_pop_mr( xifo32f_t *c );
// /* Extractors */
// uint32_t xifo32f_get_size( xifo32f_t *c );
// uint32_t xifo32f_get_used( xifo32f_t *c );
// uint32_t xifo32f_get_full( xifo32f_t *c );
// uint32_t xifo32f_get_free( xifo32f_t *c );