#ifndef _CRC_H_
#define _CRC_H_
#include "stdio.h"
#include "stdint.h"
#include "stdbool.h"

// typedef enum CRC_type
// {
//     CRC_8=0,
//     CRC_16,
//     CRC_32
// }CRC_type;

typedef struct CRC_cfg_t
{
    // CRC_type type;
    uint16_t polimonial;
    uint16_t init_value;
    uint16_t xor_value;
    bool input_reflect;
    bool output_reflect;
}CRC_cfg_t;




void display_CRC_8_table();
void display_CRC_16_table();

void InitLockupTable_8(uint8_t);
void InitLockupTable_16(uint16_t);
void InitLockupTable_32(uint32_t);

uint8_t Calculator_CRC8_table(uint8_t* input, uint16_t size, CRC_cfg_t cfg);
uint8_t Calculator_CRC8_sw(uint8_t* input, uint16_t size, CRC_cfg_t cfg);

uint16_t Calculator_CRC16_table(uint8_t* input, uint16_t size, CRC_cfg_t cfg);
uint16_t Calculator_CRC16_sw(uint8_t* input, uint16_t size, CRC_cfg_t cfg);

uint8_t GetLockupTable_8(uint8_t);
uint16_t GetLockupTable_16(uint8_t);

void test();

#endif