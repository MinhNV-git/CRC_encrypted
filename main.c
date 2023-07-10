#include "stdio.h"
#include "stdint.h"
#include "time.h"
#include "crc.h"

#define POLIMONIAL          0x8005
#define INPUT_REFLECT       1
#define OUTPUT_REFLECT      1
#define SIZE                9
#define INIT_VALUE          0xFFFF
#define XOR_VALUE           0xFFFF

#define TEST                1      // 0: 8bit, 1: 16bit

uint8_t input[10] = {'1','2','3','4','5','6','7','8','9'};
uint8_t input_hex[9] = {0x01,0x02,0x03,0x04,0x05,0x06,0x08,0x09};

clock_t begin;
clock_t end;
double time_spent ;
void time_excute()
{
    end = clock();
    time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\r\nTime excute: %f",time_spent);
}
int main()
{
    uint8_t a;
    uint16_t b;

#if (TEST == 0)
    InitLockupTable_8(POLIMONIAL);
    //display_CRC_8_table();
    a= Calculator_CRC8_table(input,SIZE, INIT_VALUE, INPUT_REFLECT, OUTPUT_REFLECT, XOR_VALUE);
    printf("\r\nKet qua table: 0x%x\r\n",a);
    a = Calculator_CRC8_sw(input, SIZE, INIT_VALUE, POLIMONIAL, INPUT_REFLECT, OUTPUT_REFLECT, XOR_VALUE);
    printf("Ket qua SW: 0x%x",a);
#elif (TEST == 1)
    InitLockupTable_16(POLIMONIAL);
    //display_CRC_16_table();
    b= Calculator_CRC16_table(input, SIZE, INIT_VALUE, INPUT_REFLECT, OUTPUT_REFLECT, XOR_VALUE);
    printf("\r\nKet qua table: 0x%x\r\n",b);
    b = Calculator_CRC16_sw(input, SIZE, POLIMONIAL, INIT_VALUE, INPUT_REFLECT, OUTPUT_REFLECT, XOR_VALUE);
    printf("Ket qua SW: 0x%x",b);
#endif
}
