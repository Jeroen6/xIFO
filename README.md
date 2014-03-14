# Multipurpose ringbuffer

When I needed a ringbuffer there weren't any simple standalone ones available for 32-Bit ARM Cortex.
At least not without posix style overhead and dynamic memory complexity.

So I created xIFO for my need with ARM Cortex M microprocessors.
xIFO stands for FIFO or LIFO, xIFO can be used in any way.
It doesn't allocate any memory itself, and all functions are optimised to not use any stack.
But further optimisation suggestions are welcome, as long as it stays C.

**The default types are:**

xifo8_t, xifo16_t, xifo32_t, xifo64_t and *xifo<T>* which is C++ only.

Which work internally with:

uint8_t, uint16_t, uint32_t, uint64_t

But with (proper) typecasting any type that fits is possible.

### Why so much?
Since C doesn't support templates each major type need one.

### Notes:
Most overhead types are 32 bit optimised. But for an single byte buffer, single bytes are used to save space.

With a fixed 8/16/32/64 Bit element size it is optimized for 32 bit arm processors. If you need larger sizes, take a look at the SIZETYPE files.
They are there to guide you to your own type. But in 32 bit platforms the xifo32 is excellent to put pointers in.
eg: xifo32_write(&buffer, (uint32_t)your_50_byte_table)	// Store pointers to dynamic tables

This is not a C++ class, it is a C module. It does work object oriented, however you cannot work on the object, you work with the object.
See the test folder for implementation examples

### Usage:
```c
xifo_pool_t input_buffer[100];
xifo_t input[100];
xifo_init(&input, 100, input_buffer);
void xifo_clear( &input );
// Write while not full
while( xifo_write( &input , data ) );
first = xifo_read_lr( &input , 0);
second  = xifo16_read_lr( &input , 1);
last  = xifo16_read_mr( &input , 0);
second_last  = xifo16_read_mr( &input , 1);
removed_first = xifo16_pop_lr( &input );
removed_last  = xifo16_pop_mr( &input );
xifo16_get_size( &input );
xifo16_get_used( &input );
xifo16_get_full( &input );
xifo16_get_free( &input );
```
