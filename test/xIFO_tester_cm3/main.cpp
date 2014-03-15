#include "mbed.h"

/* NOTICE, THIS FILE IS -O1 */

DigitalOut myled(LED1);
Serial pc(USBTX, USBRX); // tx, rx\

#include "xifo.h"

xifo8_pool_t data8[10] 	__attribute__((aligned (4)));
xifo16_pool_t data16[10]__attribute__((aligned (4)));
xifo32_pool_t data32[10]__attribute__((aligned (4)));
xifo64_pool_t data64[10]__attribute__((aligned (4)));

xifo8_t 	x8;
xifo16_t 	x16;
xifo32_t 	x32;
xifo64_t 	x64;

uint32_t i = 8;
uint32_t s = 0;

int main() {
    pc.printf(__DATE__" "__TIME__"\n");
    register uint32_t T0 = 0x44444444,T1 = 0x55555555;

    // Config timer on CORECLOCK
    LPC_SC->PCONP |= (1<<1)|(1<<2)|(1<<22)|(1<<23); // All timers
    LPC_SC->PCLKSEL0 |= (0<<3)|(1<<2);	// PCLK = CCLK
    LPC_TIM0->TCR = 2;
    LPC_TIM0->PR = 0;
    LPC_TIM0->TCR = 1;

    pc.printf("Testing xifo, all single call average times in sysclks\n");
    //**********************************
    {
        pc.printf("\nTesting xifo8\n");

        T0 = LPC_TIM0->TC;
        xifo8_init(&x8,10,data8);
        T1 = LPC_TIM0->TC;
        pc.printf("Init:   \t%d\n",T1-T0);

        T0 = LPC_TIM0->TC;
        xifo8_clear(&x8);
        T1 = LPC_TIM0->TC;
        pc.printf("Clear:  \t%d\n",T1-T0);

        i = 8;
        s = 0;
        while(i--){
            T0 = LPC_TIM0->TC;
            xifo8_write(&x8,T0);
            T1 = LPC_TIM0->TC;
            s+= T1-T0;
        }
        pc.printf("Write 8:\t%d\n",s/8);

        i = 8;
        s = 0;
        while(i--){
            T0 = LPC_TIM0->TC;
            T1 = xifo8_read_mr(&x8,i);
            T1 = LPC_TIM0->TC;
            s+= T1-T0;
            T0 = LPC_TIM0->TC;
            T1 = xifo8_read_lr(&x8,i);
            T1 = LPC_TIM0->TC;
            s+= T1-T0;
        }
        pc.printf("Read 8: \t%d\n",s/8);

        i = 8;
        s = 0;
        while(i--){
            T0 = LPC_TIM0->TC;
            xifo8_write(&x8,T0);
            T1 = LPC_TIM0->TC;
            s+= T1-T0;
        }
        pc.printf("Write 4:\t%d\n",s/8);

        i = 8;
        s = 0;
        while(i--){
            T0 = LPC_TIM0->TC;
            T1 = xifo8_pop_mr(&x8);
            T1 = LPC_TIM0->TC;
            s+= T1-T0;
            T0 = LPC_TIM0->TC;
            T1 = xifo8_pop_lr(&x8);
            T1 = LPC_TIM0->TC;
            s+= T1-T0;
        }
        pc.printf("Pop 8:  \t%d\n",s/8);
    }
    //**********************************
    {
        pc.printf("\nTesting xifo16\n");

        T0 = LPC_TIM0->TC;
        xifo16_init(&x16,10,data16);
        T1 = LPC_TIM0->TC;
        pc.printf("Init:   \t%d\n",T1-T0);

        T0 = LPC_TIM0->TC;
        xifo16_clear(&x16);
        T1 = LPC_TIM0->TC;
        pc.printf("Clear:  \t%d\n",T1-T0);

        i = 8;
        s = 0;
        while(i--){
            T0 = LPC_TIM0->TC;
            xifo16_write(&x16,T0);
            T1 = LPC_TIM0->TC;
            s+= T1-T0;
        }
        pc.printf("Write 8:\t%d\n",s/8);

        i = 8;
        s = 0;
        while(i--){
            T0 = LPC_TIM0->TC;
            T1 = xifo16_read_mr(&x16,i);
            T1 = LPC_TIM0->TC;
            s+= T1-T0;
            T0 = LPC_TIM0->TC;
            T1 = xifo16_read_lr(&x16,i);
            T1 = LPC_TIM0->TC;
            s+= T1-T0;
        }
        pc.printf("Read 8: \t%d\n",s/8);

        i = 8;
        s = 0;
        while(i--){
            T0 = LPC_TIM0->TC;
            xifo16_write(&x16,T0);
            T1 = LPC_TIM0->TC;
            s+= T1-T0;
        }
        pc.printf("Write 4:\t%d\n",s/8);

        i = 8;
        s = 0;
        while(i--){
            T0 = LPC_TIM0->TC;
            T1 = xifo16_pop_mr(&x16);
            T1 = LPC_TIM0->TC;
            s+= T1-T0;
            T0 = LPC_TIM0->TC;
            T1 = xifo16_pop_lr(&x16);
            T1 = LPC_TIM0->TC;
            s+= T1-T0;
        }
        pc.printf("Pop 8:  \t%d\n",s/8);
    }
    //**********************************
    {
        pc.printf("\nTesting xifo32\n");

        T0 = LPC_TIM0->TC;
        xifo32_init(&x32,10,data32);
        T1 = LPC_TIM0->TC;
        pc.printf("Init:   \t%d\n",T1-T0);

        T0 = LPC_TIM0->TC;
        xifo32_clear(&x32);
        T1 = LPC_TIM0->TC;
        pc.printf("Clear:  \t%d\n",T1-T0);

        i = 8;
        s = 0;
        while(i--){
            T0 = LPC_TIM0->TC;
            xifo32_write(&x32,T0);
            T1 = LPC_TIM0->TC;
            s+= T1-T0;
        }
        pc.printf("Write 8:\t%d\n",s/8);

        i = 8;
        s = 0;
        while(i--){
            T0 = LPC_TIM0->TC;
            T1 = xifo32_read_mr(&x32,i);
            T1 = LPC_TIM0->TC;
            s+= T1-T0;
            T0 = LPC_TIM0->TC;
            T1 = xifo32_read_lr(&x32,i);
            T1 = LPC_TIM0->TC;
            s+= T1-T0;
        }
        pc.printf("Read 8: \t%d\n",s/8);

        i = 8;
        s = 0;
        while(i--){
            T0 = LPC_TIM0->TC;
            xifo32_write(&x32,T0);
            T1 = LPC_TIM0->TC;
            s+= T1-T0;
        }
        pc.printf("Write 4:\t%d\n",s/8);

        i = 8;
        s = 0;
        while(i--){
            T0 = LPC_TIM0->TC;
            T1 = xifo32_pop_mr(&x32);
            T1 = LPC_TIM0->TC;
            s+= T1-T0;
            T0 = LPC_TIM0->TC;
            T1 = xifo32_pop_lr(&x32);
            T1 = LPC_TIM0->TC;
            s+= T1-T0;
        }
        pc.printf("Pop 8:  \t%d\n",s/8);
    }
    //**********************************
    {
        pc.printf("\nTesting xifo64\n");

        T0 = LPC_TIM0->TC;
        xifo64_init(&x64,10,data64);
        T1 = LPC_TIM0->TC;
        pc.printf("Init:   \t%d\n",T1-T0);

        T0 = LPC_TIM0->TC;
        xifo64_clear(&x64);
        T1 = LPC_TIM0->TC;
        pc.printf("Clear:  \t%d\n",T1-T0);

        i = 8;
        s = 0;
        while(i--){
            T0 = LPC_TIM0->TC;
            xifo64_write(&x64,T0);
            T1 = LPC_TIM0->TC;
            s+= T1-T0;
        }
        pc.printf("Write 8:\t%d\n",s/8);

        i = 8;
        s = 0;
        while(i--){
            register uint64_t d = 0x5555555;
            T0 = LPC_TIM0->TC;
            d = xifo64_read_mr(&x64,i);
            T1 = LPC_TIM0->TC;
            s+= T1-T0;
						d++;
            T0 = LPC_TIM0->TC;
            d = xifo64_read_lr(&x64,i);
            T1 = LPC_TIM0->TC;
            s+= T1-T0;
						d++;
        }
        pc.printf("Read 8: \t%d\n",s/8);

        i = 8;
        s = 0;
        while(i--){
            T0 = LPC_TIM0->TC;
            xifo64_write(&x64,T0);
            T1 = LPC_TIM0->TC;
            s+= T1-T0;
        }
        pc.printf("Write 4:\t%d\n",s/8);

        i = 8;
        s = 0;
        while(i--){
            register uint64_t d = 0x5555555;
            T0 = LPC_TIM0->TC;
            d = xifo64_pop_mr(&x64);
            T1 = LPC_TIM0->TC;
            s+= T1-T0;
            d++;
            T0 = LPC_TIM0->TC;
            d = xifo64_pop_lr(&x64);
            T1 = LPC_TIM0->TC;
            s+= T1-T0;
            d++;
        }
        pc.printf("Pop 8:  \t%d\n",s/8);
    }
    //**********************************
    while(1) {
        myled = 1;
        wait(0.2);
        myled = 0;
        wait(0.2);
    }
}




