
#include "progmem_ihex.h"

bool g_p_hex_recored_end = false;
bool g_p_hex_recored_start = false;
bool g_p_hex_recored_completed = false;
bool g_p_hex_ext_segment_address = false;
bool g_p_hex_ext_linear_address = false;

uint8_t g_p_hex_recored_checksum = 0;
uint8_t g_p_hex_recored_type = 0;
uint8_t g_p_hex_recored[PROGMEM_FLASH_CF_WRITE_SIZE] = {0};
uint8_t g_p_hex_flash_code[PROGMEM_FLASH_CF_SECTOR_SIZE] = {0};

uint16_t g_p_hex_recored_char_counts = 0;
uint16_t g_p_hex_flash_code_length = 0;
uint16_t g_p_hex_recored_length = 0;
uint16_t g_p_hex_recored_address = 0;

uint32_t g_p_flash_last_address = 0;
uint32_t g_p_flash_rec_base_address = 0;
uint32_t g_p_rec_segment_address = 0;
uint32_t g_p_rec_liner_address = 0;
uint32_t g_p_hex_recored_code_seg_address = 0;
uint32_t g_p_hex_recored_ins_ptr_address = 0;
uint32_t g_p_hex_recored_exe_address = 0;

uint8_t ProgMem_Hex2Byte(uint8_t ch)
{
    if ((ch >= '0') && (ch <= '9'))
        return ((uint8_t)(ch - '0'));
    if ((ch >= 'A') && (ch <= 'F'))
        return ((uint8_t)(ch - 'A' + 10));
    if ((ch >= 'a') && (ch <= 'f'))
        return ((uint8_t)(ch - 'a' + 10));

    return ((uint8_t)0xFF);
}

progmem_rt ProgMem_HexRecoredProcess(void *flash_ctrl, uint8_t *buffer)
{
    // hex recored length
    uint8_t p_data = ProgMem_Hex2Byte(*buffer++);
    p_data = ((p_data << 4) + ProgMem_Hex2Byte(*buffer++));
    g_p_hex_recored_checksum += p_data;
    g_p_hex_recored_length = p_data;
    // hex recored address high
    p_data = ProgMem_Hex2Byte(*buffer++);
    p_data = ((p_data << 4) + ProgMem_Hex2Byte(*buffer++));
    g_p_hex_recored_checksum += p_data;
    g_p_hex_recored_address = p_data;
    // hex recored address low
    p_data = ProgMem_Hex2Byte(*buffer++);
    p_data = ((p_data << 4) + ProgMem_Hex2Byte(*buffer++));
    g_p_hex_recored_checksum += p_data;
    g_p_hex_recored_address = ((p_data << 8) + p_data);
    // hex recored types
    p_data = ProgMem_Hex2Byte(*buffer++);
    p_data = ((p_data << 4) + ProgMem_Hex2Byte(*buffer++));
    g_p_hex_recored_checksum += p_data;
    g_p_hex_recored_type = p_data;

    switch (g_p_hex_recored_type)
    {
    case HEX_RECORED_TYPE_DATA:
        if (g_p_hex_ext_segment_address)
        {
            g_p_flash_rec_base_address = (uint32_t)((g_p_rec_segment_address * 16) + g_p_hex_recored_address);
            g_p_hex_ext_segment_address = false;
        }
        else if (g_p_hex_ext_linear_address)
        {
            g_p_flash_rec_base_address = (uint32_t)(g_p_rec_liner_address + g_p_hex_recored_address);
            g_p_hex_ext_linear_address = false;
        }

        for (int i = 0; i < g_p_hex_recored_length; i++)
        {
            p_data = ProgMem_Hex2Byte(*buffer++);
            p_data = ((p_data << 4) + ProgMem_Hex2Byte(*buffer++));
            g_p_hex_recored_checksum += p_data;
            g_p_hex_flash_code[g_p_hex_flash_code_length++] = p_data;
            if (g_p_hex_flash_code_length >= PROGMEM_FLASH_CF_SECTOR_SIZE)
            {
                if (ProgMem_FlashWriteAndVerfiy(flash_ctrl, g_p_hex_flash_code, g_p_flash_rec_base_address, PROGMEM_FLASH_CF_SECTOR_SIZE))
                {
                    return ((progmem_rt)-1);
                }
                // copy remaing bytes to start of the buffer
                g_p_flash_rec_base_address += g_p_hex_flash_code_length;
                uint16_t g_p_flash_rec_remaining_data = g_p_hex_flash_code_length - PROGMEM_FLASH_CF_SECTOR_SIZE;
                memcpy(&g_p_hex_flash_code[0], &g_p_hex_flash_code[g_p_hex_flash_code_length], g_p_flash_rec_remaining_data);
                g_p_hex_flash_code_length = g_p_flash_rec_remaining_data;
            }
        }
        // Record Checksum
        p_data = ProgMem_Hex2Byte(*buffer++);
        p_data = ((p_data << 4) + ProgMem_Hex2Byte(*buffer++));
        g_p_hex_recored_checksum += p_data;

        break;
    case HEX_RECORED_TYPE_EOF:
        // Record Checksum
        p_data = ProgMem_Hex2Byte(*buffer++);
        p_data = ((p_data << 4) + ProgMem_Hex2Byte(*buffer++));
        g_p_hex_recored_checksum += p_data;
        // eof records
        return ((progmem_rt)1);
    case HEX_RECORED_TYPE_ESA:
        g_p_hex_ext_segment_address = true;
        g_p_hex_ext_linear_address = false;
        // Extended Segment Address High
        p_data = ProgMem_Hex2Byte(*buffer++);
        p_data = ((p_data << 4) + ProgMem_Hex2Byte(*buffer++));
        g_p_hex_recored_checksum += p_data;
        g_p_hex_ext_segment_address = p_data;
        // Extended Segment Address Low
        p_data = ProgMem_Hex2Byte(*buffer++);
        p_data = ((p_data << 4) + ProgMem_Hex2Byte(*buffer++));
        g_p_hex_recored_checksum += p_data;
        g_p_hex_ext_segment_address((g_p_hex_ext_segment_address << 8) + p_data);
        // Record Checksum
        p_data = ProgMem_Hex2Byte(*buffer++);
        p_data = ((p_data << 4) + ProgMem_Hex2Byte(*buffer++));
        g_p_hex_recored_checksum += p_data;

        break;
    case HEX_RECORED_TYPE_SSA:

        // Start Segment Address High
        p_data = ProgMem_Hex2Byte(*buffer++);
        p_data = ((p_data << 4) + ProgMem_Hex2Byte(*buffer++));
        g_p_hex_recored_checksum += p_data;
        g_p_hex_recored_code_seg_address = p_data;
        // Start Segment Address Low
        p_data = ProgMem_Hex2Byte(*buffer++);
        p_data = ((p_data << 4) + ProgMem_Hex2Byte(*buffer++));
        g_p_hex_recored_checksum += p_data;
        g_p_hex_recored_code_seg_address = ((g_p_hex_recored_code_seg_address << 8) + p_data);
        // Instruction Pointer Address High
        p_data = ProgMem_Hex2Byte(*buffer++);
        p_data = ((p_data << 4) + ProgMem_Hex2Byte(*buffer++));
        g_p_hex_recored_checksum += p_data;
        g_p_hex_recored_ins_ptr_address = p_data;
        // Instrcution Pointer Address Low
        p_data = ProgMem_Hex2Byte(*buffer++);
        p_data = ((p_data << 4) + ProgMem_Hex2Byte(*buffer++));
        g_p_hex_recored_checksum += p_data;
        g_p_hex_recored_ins_ptr_address = ((g_p_hex_recored_ins_ptr_address << 8)+p_data;
        // Record Checksum
        p_data = ProgMem_Hex2Byte(*buffer++);
        p_data = ((p_data << 4) + ProgMem_Hex2Byte(*buffer++));
        g_p_hex_recored_checksum += p_data;

        break;
    case HEX_RECORED_TYPE_ELA:
        g_p_hex_ext_segment_address = false;
        g_p_hex_ext_linear_address  = true;
        //all remaining data should be written to the flash before extended linear address started  
        if(g_p_hex_flash_code_length)
        {
            if (ProgMem_FlashWriteAndVerfiy(flash_ctrl, g_p_hex_flash_code, g_p_flash_rec_base_address, PROGMEM_FLASH_CF_SECTOR_SIZE))
            {
                return ((progmem_rt)-1);
            }
            g_p_hex_flash_code_length = 0; 
        }
        // Extended Linear Address High
        p_data = ProgMem_Hex2Byte(*buffer++);
        p_data = ((p_data << 4) + ProgMem_Hex2Byte(*buffer++));
        g_p_hex_recored_checksum += p_data;
        g_p_rec_liner_address = p_data;
        // Extended Linear Address Low
        p_data = ProgMem_Hex2Byte(*buffer++);
        p_data = ((p_data << 4) + ProgMem_Hex2Byte(*buffer++));
        g_p_hex_recored_checksum += p_data;
        g_p_rec_liner_address = ((g_p_rec_liner_address << 24) + (p_data << 16));
        // Record Checksum
        p_data = ProgMem_Hex2Byte(*buffer++);
        p_data = ((p_data << 4) + ProgMem_Hex2Byte(*buffer++));
        g_p_hex_recored_checksum += p_data;

        break;
    case HEX_RECORED_TYPE_SLA:
       //if cpu supported get the exceution address from the record
        p_data = ProgMem_Hex2Byte(*buffer++);
        p_data = ((p_data << 4) + ProgMem_Hex2Byte(*buffer++));
        g_p_hex_recored_checksum += p_data;
        g_p_hex_recored_exe_address = (pdata << 24);
        p_data = ProgMem_Hex2Byte(*buffer++);
        p_data = ((p_data << 4) + ProgMem_Hex2Byte(*buffer++));
        g_p_hex_recored_checksum += p_data;
        g_p_hex_recored_exe_address = g_p_hex_recored_exe_address + (pdata << 16);
         p_data = ProgMem_Hex2Byte(*buffer++);
        p_data = ((p_data << 4) + ProgMem_Hex2Byte(*buffer++));
        g_p_hex_recored_checksum += p_data;
        g_p_hex_recored_exe_address = g_p_hex_recored_exe_address + (pdata << 8);
        p_data = ProgMem_Hex2Byte(*buffer++);
        p_data = ((p_data << 4) + ProgMem_Hex2Byte(*buffer++));
        g_p_hex_recored_checksum += p_data;
        g_p_hex_recored_exe_address = g_p_hex_recored_exe_address + (pdata);
        // Record Checksum
        p_data = ProgMem_Hex2Byte(*buffer++);
        p_data = ((p_data << 4) + ProgMem_Hex2Byte(*buffer++));
        g_p_hex_recored_checksum += p_data;
        break;
    }

    if(g_p_hex_recored_checksum == 0) //record checksum is ok
        return ((progmem_rt)0);
    else
        return ((progmem_rt)-1);
}

progmem_rt ProgMem_HexRowDataProcess(void *flash_ctrl.uint8_t p_data)
{
    switch (p_data)
    {
    case HEX_RECORED_START:
        g_p_hex_recored_char_counts = 0;
        g_p_hex_recored_end = false;
        g_p_hex_recored_completed = false;
        g_p_hex_recored_start = true;
        break;
    case HEX_RECORED_ER:
        g_p_hex_recored_end = true;
        break;
    case HEX_RECORED_END:
        if (g_p_hex_recored_start && g_p_hex_recored_end)
        {
            g_p_hex_recored_checksum = 0;
            g_p_hex_recored_completed = true;
        }
        break;
    default:
        g_p_hex_recored[g_p_hex_recored_char_counts++] = p_data;
        break;
    }

    if (g_p_hex_recored_completed)
    {
        g_p_hex_recored_completed = false;

        if (ProgMem_HexRecoredProcess(flash_ctrl, g_p_hex_recored))
        {

            return ((progmem_rt)-1);
        }
    }

    return ((progmem_rt)0);
}
progmem_rt ProgMem_ProcessBuffer(void *flash_ctrl, const uint8_t *buffer, uint32_t address, uint32_t size)
{
    g_p_flash_last_address = address;

    for (uint32_t i = 0; i < size; i++)
    {

        if (ProgMem_HexRowDataProcess(flash_ctrl, buffer[i]))
        {

            return ((progmem_rt)-1);
        }
    }
    return ((progmem_rt)0);
}
