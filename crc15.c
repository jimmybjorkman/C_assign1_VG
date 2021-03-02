/**
 * @file crc15.c
 * @author Jimmy Björkman (jimmy.bjorkman@yaelev.se)
 * @brief A program that calclulates a CRC for a message, adds it to the message 
 *  and then validates that the message has not been corrupted.
 * @version 0.1
 * @date 2021-03-02
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <stdio.h>
#include <stdint.h>

#define POLYNOMIAL 0xC599U // The CAN protocol uses the CRC-15 with this polynomial
#define FIRSTBITSET (1 << 15) //

/*Calc_CRC 
is a function to calculate the CRC for message consisting of an array of char:s with a specified length 
*/

uint16_t calc_CRC(uint8_t message[], uint8_t length){  
    uint16_t CRC = 0x0000U;                                     
    for (int character = 0; character < (length); character++)  //looping through all the characters in the array
    {
        for (int bit = 0; bit < 8; bit++)                       //looping through each bit of the char                       
        {
            CRC = CRC << 1;                                     //Algorithm starts by making space for the next bit in the remainder
            CRC = CRC | ((message[character] >> bit) & 1);      //The bits are inserted in the remainder from LSB to MSB
            if (CRC & FIRSTBITSET)                              //if the leftmost bit is a one
            {
                CRC = CRC ^ POLYNOMIAL;                         //The remainder is divided by the polynomial and an new remainder is received
            }
        }
    }
    return CRC;                                                 //When all the bits in the message have passed the algorithm the remainder which is the CRC is returned.
}
/*rev_bit_order 
is a function that takes a byte and returns another byte in reversed bit order
*/
uint8_t rev_bit_order(uint8_t data){
    uint8_t rev_data = 0;
    for (int bit = 0; bit < 8; bit++)
    {
        rev_data = rev_data << 1;
        rev_data = rev_data | ((data >> bit) & 1);
    }
    return rev_data;
}

    int main(void)
{
    uint8_t message[] = {'H', 'e','l', 'l', 'o', ' ', 'W', 'o', 'r', 'l', 'd', '!', 0, 0}; // 15 zeros have been appended to the message
    uint8_t length = sizeof(message) / sizeof(message[0]);

    uint16_t CRC = calc_CRC(message, length);   //the for the message is received by calling the calc_CRC function
    printf("CRC is %04x\n", CRC);

    //Before the message can be sent the CRC shall be added in reversed bit order since the algorithm works from LSB to MSB
    message[length - 2] = rev_bit_order((uint8_t)(CRC >> 8));   //First half of the CRC is reversed and converted to 8-bits
    message[length - 1] = rev_bit_order((uint8_t)CRC);          //Second half of the CRC is reversed and converted to 8-bits

    message[1] = 'a';   //To check that the validation detects an error the message is changed here

    // Validate the message.
    if (calc_CRC(message, length)==0x0000U){
        printf("The message is correct");
    }
    else{
        printf("The message has been corrupted");
    }

    return 0;
}
