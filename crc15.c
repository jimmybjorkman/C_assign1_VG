#include <stdio.h>
#include <stdint.h>

#define POLYNOMIAL 0xC599U // The CAN protocol uses the CRC-15 with this polynomial
#define FIRSTBITSET (1 << 15) //

uint16_t calc_CRC(uint8_t message[], uint8_t length){
    uint16_t CRC = 0x0000U;

    for (int character = 0; character < (length); character++)
    {
        for (int bit = 0; bit < 8; bit++)
        {
            CRC = CRC << 1;
            CRC = CRC | ((message[character] >> bit) & 1);
            if (CRC & FIRSTBITSET)
            {
                CRC = CRC ^ POLYNOMIAL;
            }
        }
    }
    return CRC;
}

    int main(void)
{
    //uint8_t message[] = {'A', 'B', 0, 0};
    uint8_t message[] = {'H', 'e','l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!', 0, 0}; // 15 zeros have been appended to the message
    uint8_t length = sizeof(message) / sizeof(message[0]);

    uint16_t CRC = calc_CRC(message, length);

    printf("CRC is %04x\n", CRC);
    uint16_t revCRC = 0x0000U;

    for (int bit = 8; bit < 16; bit++)
    {
        revCRC = revCRC << 1;
        revCRC = revCRC | ((CRC >> bit) & 1); 
    }
    printf("reverse CRC first is %04x\n", revCRC);
    for (int bit = 0; bit < 8; bit++)
    {
        revCRC = revCRC << 1;
        revCRC = revCRC | ((CRC >> bit) & 1);
    }
    printf("reverse CRC second is %04x\n", revCRC);
    //Send the message
    message[length - 2] =     (uint8_t)(revCRC >> 8);
    message[length - 1] =     (uint8_t)(revCRC); 

    //message[1] = 'a';

        for (int character = 0; character < (length); character++)
    {
        printf("%d: %02x\n", character, message[character]);
    }
    
    // Validate the message.
    CRC = CRC = calc_CRC(message, length);

    printf("CRC is %04x\n", CRC);

    return 0;
}
