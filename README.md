# Multipurpose ringbuffer

When I needed a ringbuffer there weren't any simple standalone ones available for 32-Bit ARM Cortex.
At least not without posix style overhead and dynamic memory complexity.

So I created xIFO for my need with ARM Cortex M microprocessors.
xIFO stands for FIFO or LIFO, xIFO can be used in any way.
It doesn't allocate any memory itself, and all functions are optimised to not use any stack.
But further optimisation suggestions are welcome, as long as it stays C, or C++ for the template version.

**The default types are:**

xifo8_t, xifo16_t, xifo32_t, xifo64_t and *xifo<T>* which is C++ only.

Which work internally with:

uint8_t, uint16_t, uint32_t, uint64_t.

But with (suitable) typecasting any type that fits is possible.

### Why so much?
Since C doesn't support templates each major type need one.
There is a C++ templated version, but most embedded software is C.

### Notes:
Most overhead types are 32 bit optimised. But for an single byte buffer, single bytes are used to save space.
The buffer can keep a maximum of 2^32-1 elements, good luck with that!

With a fixed 8/16/32/64 Bit element size it is optimized for 32 bit arm processors. 
If you need larger than 64 bit you can either use multiple elements, or only store a pointer.
In 32 bit platforms the xifo32 is excellent to put pointers in.
eg: xifo32_write(&buffer, (uint32_t)your_50_byte_table)	// Store pointers to dynamic tables

The C part is like a C++ class, it is a C module. It does work object oriented, however you cannot work on the object, you work with the object. You pass the object structure's pointer every call.

### Usage C:
```c
xifo_pool_t input_buffer[100];
xifo_t input;
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

### Usage C++:
```c
static int *x_data[64];
Xifo<int> xifoCpp(64, x_data);  // Use predefined allocation
Xifo<int> xifoCpp(64);          // Use dynamic allocation
// No need for init
xifoCpp.clear();
xifoCpp.size();
xifoCpp.free();
xifoCpp.used();
xifoCpp.full();
xifoCpp.write(data);
xifoCpp.read_lr(i);
xifoCpp.read_mr(i);
xifoCpp.pop_mr(); 
xifoCpp.pop_lr();
// This one can be deinitialised (and the memory freed):
~Xifo();
```

###Performance
I've run a little performance test on some [mbeds](https://mbed.org/) to measure performance
The test works with a timer running at the same clock as the core, optimise: xifo -O3, tester -O0
```c
R0 = TC
Call xifo
R1 = TC
dT = R1-R0
```

| Cortex M0 | xifo8 | xifo16 | xifo32 | xifo64 |
|-----------|-------|--------|--------|--------|
| Init:     | 47    | 52     | 50     | 57     |
| Clear:    | 131   | 112    | 113    | 159    |
| Write 8:  | 65    | 62     | 63     | 63     |
| Read 8:   | 97    | 99     | 101    | 105    |
| Write 4:  | 67    | 64     | 65     | 69     |
| Pop 8:    | 118   | 114    | 118    | 123    |

| Cortex M3 | xifo8 | xifo16 | xifo32 | xifo64 |
|-----------|-------|--------|--------|--------|
| Init:     | 36    | 36     | 45     | 41     |
| Clear:    | 68    | 69     | 68     | 105    |
| Write 8:  | 53    | 50     | 52     | 53     |
| Read 8:   | 88    | 83     | 83     | 89     |
| Write 4:  | 55    | 52     | 54     | 54     |
| Pop 8:    | 84    | 85     | 82     | 98     |

