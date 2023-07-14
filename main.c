#include "stdio.h"
#include "stdint.h"
#include "crc.h"

#define POLIMONIAL_8          0x1D
#define INPUT_REFLECT_8       1
#define OUTPUT_REFLECT_8      1
#define INIT_VALUE_8          0xFF
#define XOR_VALUE_8           0

#define POLIMONIAL_16          0x8005
#define INPUT_REFLECT_16       1
#define OUTPUT_REFLECT_16      1
#define INIT_VALUE_16          0xFFFF
#define XOR_VALUE_16           0xFFFF

#define SIZE                9

#define TEST                1      // 0: 8bit, 1: 16bit

uint8_t input[10] = {'1','2','3','4','5','6','7','8','9'};
uint8_t input_hex[9] = {0x01,0x02,0x03,0x04,0x05,0x06,0x08,0x09};


int main()
{
    uint8_t a;
    uint16_t b;

#if (TEST == 0 )
    CRC_cfg_t CRC_8 = {
        .polimonial = POLIMONIAL_8,
        .init_value = INIT_VALUE_8,
        .input_reflect = INPUT_REFLECT_8,
        .output_reflect = OUTPUT_REFLECT_8,
        .xor_value = XOR_VALUE_8,
    };
    a= Calculator_CRC8_table(input,SIZE, CRC_8);
    printf("\r\nKet qua table: 0x%x\r\n",a);
    a = Calculator_CRC8_sw(input, SIZE, CRC_8);
    printf("Ket qua SW: 0x%x",a);
#elif (TEST == 1)
    CRC_cfg_t CRC_16 = {
        .polimonial = POLIMONIAL_16,
        .init_value = INIT_VALUE_16,
        .input_reflect = INPUT_REFLECT_16,
        .output_reflect = OUTPUT_REFLECT_16,
        .xor_value = XOR_VALUE_16,
    };
    b= Calculator_CRC16_table(input, SIZE, CRC_16);
    printf("\r\nKet qua table: 0x%x\r\n",b);
    b = Calculator_CRC16_sw(input, SIZE, CRC_16);
    printf("Ket qua SW: 0x%x",b);
#endif
}
