#include "crc.h"

static uint8_t LockupTable_8[256] = {};
static uint16_t LockupTable_16[256] = {};
static uint32_t LockupTable_32[256] = {};

static uint8_t Reflect_8(uint8_t);
static uint16_t Reflect_16(uint16_t);
static uint32_t Reflect_32(uint32_t);


#define _CHECK_BIT(byte,bit_num)        ( ((byte>>bit_num) & 0x01) ? 1:0 )

void display_CRC_16_table()
{
    int i;
    for(i=0;i<256;i++)
    {
        printf(" 0x%x",LockupTable_16[i]);
        if(i%8==7)
            printf("\r\n");
    }  
}
void display_CRC_8_table()
{
    int i;
    for(i=0;i<256;i++)
    {
        printf(" 0x%x",LockupTable_8[i]);
        if(i%8==7)
            printf("\r\n");
    }  
}

void test()
{
    uint8_t val=0x01;
    uint8_t a = 0x12, b= 0x23;

}
/* *************** CRC 8 *************** */
void InitLockupTable_8(uint8_t polinomial)
{
    uint8_t i,result;
    uint16_t val;
    for(val = 0; val <= 0xFF; val++)
    {
        result = (uint8_t)val;

        for(i=0;i<8;i++)
        {
            if(_CHECK_BIT(result,7) != 0)
            {
                result = result << 1;
                result ^= polinomial;
            }
            else
            {
                result = result << 1;
            }
        }

        LockupTable_8[val] = result;
    }
}
uint8_t Calculator_CRC8_table(uint8_t* input, uint16_t size, CRC_cfg_t cfg)
{
    uint8_t result=0, re_input;
    uint16_t i;

    InitLockupTable_8(cfg.polimonial);  //init lockup table     

    result = cfg.init_value;
    for(i=0;i<size;++i)
    {
        re_input = cfg.input_reflect ? Reflect_8(*(input+i)) : *(input+i);
        result ^= re_input;
        result = GetLockupTable_8(result);
    }
    result = cfg.output_reflect ? Reflect_8(result) : result;
    result ^= cfg.xor_value;
    return result;
}
uint8_t Calculator_CRC8_sw(uint8_t* input, uint16_t size, CRC_cfg_t cfg)
{
    uint16_t val;
    uint8_t i, result=0, next_byte=0;
     
    result = cfg.init_value ^ (cfg.input_reflect ? Reflect_8(*(input+0)) : *(input+0));
    val = 1;
    while(val<size+1)
    {
        next_byte = cfg.input_reflect ? Reflect_8(*(input+val)) : *(input+val);
        if(val ==size)
            next_byte = 0;

        for(i=0;i<8;i++)
        {
            if(_CHECK_BIT(result,7) != 0)
            {
                result = result << 1;
                if( _CHECK_BIT(next_byte,(7-i)) != 0 )
                {
                    result |= 0x01;
                }
                result ^= cfg.polimonial;
            }
            else
            {
                result = result << 1;
                if( _CHECK_BIT(next_byte,(7-i)) != 0 )
                {
                    result |= 0x01;
                }
            }
            
        }
        val ++;
    }
    if(cfg.output_reflect == true)
    {
        result = Reflect_8(result);
    }
    result ^= cfg.xor_value;
    return result;
}
/***************************************************************/

/* ********************* CRC 16 ********************* */
void InitLockupTable_16(uint16_t polinomial)
{
    uint8_t i, val_tg;
    uint16_t result, val;
    for(val = 0; val <= 0xFF; val++)
    {
        result = (uint16_t)val;

        for(i=0; i<16; i++)
        {
            if(_CHECK_BIT(result,15) != 0)
            {
                result = result << 1;
                result ^= polinomial;
            }
            else
            {
                result = result << 1;
            }
        }

        LockupTable_16[val] = result;
    }
}
uint16_t Calculator_CRC16_table(uint8_t* input, uint16_t size, CRC_cfg_t cfg)
{
    uint16_t result=0,i;
    uint8_t pos,Re_input;

    InitLockupTable_16(cfg.polimonial);

    result = cfg.init_value;
    for(i=0;i<size;++i)
    {
        //printf("0x%x ",*(input+i));
        Re_input = (cfg.input_reflect) ? Reflect_8( *(input+i)) : *(input+i) ;

        pos = (uint8_t) ((uint8_t)(result>>8) ^ Re_input);
        result = (result<<8) ^ GetLockupTable_16(pos);
    }
    result = (cfg.output_reflect) ? Reflect_16(result) : result;
    result ^= cfg.xor_value;
    return result;
}
uint16_t Calculator_CRC16_sw(uint8_t* input, uint16_t size, CRC_cfg_t cfg)
{
    uint8_t next_byte = 0;
    uint16_t result = 0, val=0;
    uint8_t i;
    
    result = (cfg.input_reflect ? Reflect_8(*(input+0)) : *(input+0)) << 8;
    result |= cfg.input_reflect ? Reflect_8(*(input+1)) : *(input+1);
    result ^= cfg.init_value;
    val = 2;
    while(val < size+2)
    {
        next_byte = *(input+val);

        if(val >= size)
            next_byte = 0;

        if(cfg.input_reflect == 1)
        {
            next_byte = Reflect_8(next_byte);
        }
        for(i=0;i<8;i++)
        {
            if(_CHECK_BIT(result,15) != 0)
            {
                result = result << 1;
                if( _CHECK_BIT(next_byte, (7-i)) != 0 )
                {
                    result |= 0x0001;
                }
                result ^= cfg.polimonial;
            }
            else
            {
                result = result << 1;
                if( _CHECK_BIT(next_byte, (7-i)) != 0 )
                {
                    result |= 0x0001;
                } 
            }
        }
        
        val++;
    }

    if(cfg.output_reflect == 1)
    {
        result = Reflect_16(result);
    }
    result ^= cfg.xor_value;
    return result;
}
/***************************************************************/

/* *********************** CRC 32 ************************** */
void InitLockupTable_32(uint32_t polinomial)
{
    uint8_t i;
    uint16_t val;
    uint32_t result;

    for(val = 0 ; val <= 0xFF ; val++)
    {
        result = (uint32_t)val;
        for( i = 0 ; i < 32 ; i++)
        {
            if(_CHECK_BIT(result,31) != 0)
            {
                result = result << 1;
                result ^= polinomial;
            }
            else
            {
                result = result << 1;
            }
        }
        LockupTable_32[val] = result;
    }
    // printf("\r\nTable 32 bit:\r\n");
    // for(val=0;val<=0xFF;val++)
    // {
    //     printf(" %x ",LockupTable_32[val]);
    //     if(val%8==7)
    //         printf("\r\n");
    // }
}
uint32_t Calculator_CRC32_table(uint8_t* input, uint16_t size, CRC_cfg_t cfg)
{
    uint32_t result;
    uint16_t i;
    uint8_t pos,Re_input;

    InitLockupTable_32(cfg.polimonial);

    result = cfg.init_value;
    for(i=0;i<size;i++)
    {
        Re_input = (cfg.input_reflect) ? Reflect_8( *(input+i)) : *(input+i) ;
        pos = (uint8_t)( (uint8_t)(result>>24) ^ Re_input );
        result = (result<<8) ^ GetLockupTable_32(pos);
    }
    result = (cfg.output_reflect) ? Reflect_32(result) : result;
    result ^= cfg.xor_value;
    return result;
}
uint32_t Calculator_CRC32_sw(uint8_t* input, uint16_t size, CRC_cfg_t cfg)
{
    uint8_t next_byte, i;
    uint32_t result;
    uint16_t val;

    result = (cfg.input_reflect ? Reflect_8(*(input+0)) : *(input+0)) << 24;
    result |= (cfg.input_reflect ? Reflect_8(*(input+1)) : *(input+1)) << 16;
    result |= (cfg.input_reflect ? Reflect_8(*(input+2)) : *(input+2)) << 8;
    result |= (cfg.input_reflect ? Reflect_8(*(input+3)) : *(input+3)) << 0;
    result ^= cfg.init_value;
    val = 4;
    while (val < size + 4)
    {
        next_byte = *(input+val);

        if(val >= size)
            next_byte = 0;

        next_byte = (cfg.input_reflect) ? Reflect_8(next_byte): next_byte;

        for(i=0;i<8;i++)
        {
            if(_CHECK_BIT(result,31) != 0)
            {
                result = result << 1;
                if( _CHECK_BIT(next_byte, (7-i)) != 0 )
                {
                    result |= 0x000001;
                }
                result ^= cfg.polimonial;
            }
            else
            {
                result = result << 1;
                if( _CHECK_BIT(next_byte, (7-i)) != 0 )
                {
                    result |= 0x000001;
                } 
            }
        }

        val++;
    }
    if(cfg.output_reflect == 1)
    {
        result = Reflect_32(result);
    }
    result ^= cfg.xor_value;
    return result;
}
/*************************************************************/

/* */
uint8_t GetLockupTable_8(uint8_t val)
{
    return LockupTable_8[val];
}
uint16_t GetLockupTable_16(uint8_t val)
{
    return LockupTable_16[val];
}
uint32_t GetLockupTable_32(uint8_t val)
{
    return LockupTable_32[val];
}


/* */
static uint8_t Reflect_8(uint8_t val)
{
    uint8_t result_val=0, i;
    for(i=0;i<8;i++)
    {
        if(((val>>i)&0x01) != 0)
        {
            result_val |= (1<<(7-i));
        }
    }    
    return result_val;
}
static uint16_t Reflect_16(uint16_t val)
{
    uint16_t result_val=0, i;
    for(i=0;i<16;i++)
    {
        if(((val>>i)&0x0001) != 0)
        {
            result_val |= (1<<(15-i));
        }
        else
        {
            result_val &= ~(1<<(15-i));
        }
    }    
    return result_val;
}
static uint32_t Reflect_32(uint32_t val)
{
    uint32_t result;
    uint8_t i;
    for(i=0;i<32;i++)
    {
        if(_CHECK_BIT(val,i))
        {
            result |= (1<<(31-i));
        }
        else
        {
            result &= ~(1<<(31-i));
        }
    }
    return result;
}
