#include "unity.h"
#include "xifo.h"

TEST_FILE("xifo32f.c")

xifo32f_t xifo;

struct test {
    uint32_t guard0;
    xifo32f_pool_t buffer[5];
    uint32_t guard1;
} testallocation;

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

int testGuards(){
    if(testallocation.guard0 != 0xAAAAAAAA) return 0;
    if(testallocation.guard1 != 0xAAAAAAAA) return 0;
    return 1;
}

void setUp(void)
{
    testallocation.guard0 = 0xAAAAAAAA;
    testallocation.guard1 = 0xAAAAAAAA;
    xifo32f_init(&xifo, 5, testallocation.buffer);
    TEST_ASSERT_EQUAL(xifo.size, 5);
}

void tearDown(void)
{
    
}

void testXifoClear(void){
    xifo32f_clear(&xifo);
    TEST_ASSERT_EQUAL(0, testallocation.buffer[1]);
    TEST_ASSERT_TRUE(testGuards());
}

void testWriteUntilFullAndReadBack(void){
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
    TEST_ASSERT_TRUE(testGuards());
}

void testOverwriteAndCheckBoundaries(void){
    xifo32f_write(&xifo, 0x11);
    xifo32f_write(&xifo, 0x11);
    xifo32f_write(&xifo, 0x11);
    xifo32f_write(&xifo, 0x11);
    xifo32f_write(&xifo, 0x11);
    xifo32f_write(&xifo, 0x11);
    xifo32f_write(&xifo, 0x11);
    xifo32f_write(&xifo, 0x11);
    xifo32f_write(&xifo, 0x11);
    xifo32f_write(&xifo, 0x11);
    xifo32f_write(&xifo, 0x11);
    TEST_ASSERT_TRUE(testGuards());
}

void testReadLeastRecent(){
    xifo32f_write(&xifo, testdata[0]);
    xifo32f_write(&xifo, testdata[1]);
    xifo32f_write(&xifo, testdata[2]);
    xifo32f_write(&xifo, testdata[3]);
    xifo32f_write(&xifo, testdata[4]);
    xifo32f_write(&xifo, testdata[5]);
    TEST_ASSERT_EQUAL(xifo32f_read_lr(&xifo, 0), testdata[1]);
    TEST_ASSERT_EQUAL(xifo32f_read_lr(&xifo, 1), testdata[2]);
    TEST_ASSERT_EQUAL(xifo32f_read_lr(&xifo, 2), testdata[3]);
    TEST_ASSERT_EQUAL(xifo32f_read_lr(&xifo, 3), testdata[4]);
    TEST_ASSERT_EQUAL(xifo32f_read_lr(&xifo, 4), testdata[5]);
    TEST_ASSERT_EQUAL(xifo32f_read_lr(&xifo, 5), 0);
}

void testReadMostRecent(){
    xifo32f_write(&xifo, testdata[0]);
    xifo32f_write(&xifo, testdata[1]);
    xifo32f_write(&xifo, testdata[2]);
    xifo32f_write(&xifo, testdata[3]);
    xifo32f_write(&xifo, testdata[4]);
    xifo32f_write(&xifo, testdata[5]);
    TEST_ASSERT_EQUAL(xifo32f_read_mr(&xifo, 0), testdata[5]);
    TEST_ASSERT_EQUAL(xifo32f_read_mr(&xifo, 1), testdata[4]);
    TEST_ASSERT_EQUAL(xifo32f_read_mr(&xifo, 2), testdata[3]);
    TEST_ASSERT_EQUAL(xifo32f_read_mr(&xifo, 3), testdata[2]);
    TEST_ASSERT_EQUAL(xifo32f_read_mr(&xifo, 4), testdata[1]);
    TEST_ASSERT_EQUAL(xifo32f_read_mr(&xifo, 5), 0);
}

void testWriteAndPopLeastRecent(void){
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

void testWriteAndPopMostRecent(void){
    TEST_ASSERT_TRUE(xifo32f_write(&xifo, testdata[0]));
    TEST_ASSERT_TRUE(xifo32f_write(&xifo, testdata[1]));
    TEST_ASSERT_TRUE(xifo32f_write(&xifo, testdata[2]));
    TEST_ASSERT_TRUE(xifo32f_write(&xifo, testdata[3]));
    TEST_ASSERT_FALSE(xifo32f_write(&xifo, testdata[4]));
    TEST_ASSERT_FALSE(xifo32f_write(&xifo, testdata[5]));
    TEST_ASSERT_FALSE(xifo32f_write(&xifo, testdata[6]));
    TEST_ASSERT_EQUAL(xifo32f_pop_mr(&xifo), testdata[6]);
    TEST_ASSERT_EQUAL(xifo32f_pop_mr(&xifo), testdata[5]);
    TEST_ASSERT_EQUAL(xifo32f_pop_mr(&xifo), testdata[4]);
    TEST_ASSERT_EQUAL(xifo32f_pop_mr(&xifo), testdata[3]);
    TEST_ASSERT_EQUAL(xifo32f_pop_mr(&xifo), testdata[2]);
    TEST_ASSERT_EQUAL(xifo32f_pop_mr(&xifo), 0);
}

void testGetSize(){
    TEST_ASSERT_EQUAL(xifo32f_get_size(&xifo), 5);
}

void testGetUsed(){
    TEST_ASSERT_EQUAL(xifo32f_get_used(&xifo), 0);
    xifo32f_write(&xifo, testdata[0]);
    TEST_ASSERT_EQUAL(xifo32f_get_used(&xifo), 1);
    xifo32f_write(&xifo, testdata[1]);
    TEST_ASSERT_EQUAL(xifo32f_get_used(&xifo), 2);
    xifo32f_write(&xifo, testdata[2]);
    TEST_ASSERT_EQUAL(xifo32f_get_used(&xifo), 3);
    xifo32f_write(&xifo, testdata[3]);
    TEST_ASSERT_EQUAL(xifo32f_get_used(&xifo), 4);
    xifo32f_write(&xifo, testdata[4]);
    TEST_ASSERT_EQUAL(xifo32f_get_used(&xifo), 5);
    xifo32f_pop_lr(&xifo);
    TEST_ASSERT_EQUAL(xifo32f_get_used(&xifo), 4);
    xifo32f_pop_lr(&xifo);
    TEST_ASSERT_EQUAL(xifo32f_get_used(&xifo), 3);
    xifo32f_pop_lr(&xifo);
    TEST_ASSERT_EQUAL(xifo32f_get_used(&xifo), 2);
    xifo32f_pop_lr(&xifo);
    TEST_ASSERT_EQUAL(xifo32f_get_used(&xifo), 1);
    xifo32f_pop_lr(&xifo);
    TEST_ASSERT_EQUAL(xifo32f_get_used(&xifo), 0);
    xifo32f_pop_lr(&xifo);
    TEST_ASSERT_EQUAL(xifo32f_get_used(&xifo), 0);
}

void testGetFull(){
    TEST_ASSERT_FALSE(xifo32f_get_full(&xifo));
    xifo32f_write(&xifo, testdata[0]);
    TEST_ASSERT_FALSE(xifo32f_get_full(&xifo));
    xifo32f_write(&xifo, testdata[1]);
    TEST_ASSERT_FALSE(xifo32f_get_full(&xifo));
    xifo32f_write(&xifo, testdata[2]);
    TEST_ASSERT_FALSE(xifo32f_get_full(&xifo));
    xifo32f_write(&xifo, testdata[3]);
    TEST_ASSERT_FALSE(xifo32f_get_full(&xifo));
    xifo32f_write(&xifo, testdata[4]);
    TEST_ASSERT_TRUE(xifo32f_get_full(&xifo));
    xifo32f_pop_lr(&xifo);
    TEST_ASSERT_FALSE(xifo32f_get_full(&xifo));
}

void testGetFree(){
    TEST_ASSERT_TRUE(xifo32f_get_free(&xifo));
    xifo32f_write(&xifo, testdata[0]);
    TEST_ASSERT_TRUE(xifo32f_get_free(&xifo));
    xifo32f_write(&xifo, testdata[1]);
    TEST_ASSERT_TRUE(xifo32f_get_free(&xifo));
    xifo32f_write(&xifo, testdata[2]);
    TEST_ASSERT_TRUE(xifo32f_get_free(&xifo));
    xifo32f_write(&xifo, testdata[3]);
    TEST_ASSERT_TRUE(xifo32f_get_free(&xifo));
    xifo32f_write(&xifo, testdata[4]);
    TEST_ASSERT_FALSE(xifo32f_get_free(&xifo));
    xifo32f_pop_lr(&xifo);
    TEST_ASSERT_TRUE(xifo32f_get_free(&xifo));
}