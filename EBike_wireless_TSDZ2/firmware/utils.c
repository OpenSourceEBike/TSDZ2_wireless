/*
 * Bafang LCD 850C firmware
 *
 * Copyright (C) Casainho, 2018.
 *
 * Released under the GPL License, Version 3
 */

#include <stdint.h>
#include <stdio.h>
#include "utils.h"

int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min,
		int32_t out_max) {
	// if input is smaller/bigger than expected return the min/max out ranges value
	if (x < in_min)
		return out_min;
	else if (x > in_max)
		return out_max;

	// map the input to the output range.
	// round up if mapping bigger ranges to smaller ranges
	else if ((in_max - in_min) > (out_max - out_min))
		return (x - in_min) * (out_max - out_min + 1) / (in_max - in_min + 1)
				+ out_min;
	// round down if mapping smaller ranges to bigger ranges
	else
		return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

uint8_t ui8_min(uint8_t value_a, uint8_t value_b) {
	if (value_a < value_b)
		return value_a;
	else
		return value_b;
}

uint8_t ui8_max(uint8_t value_a, uint8_t value_b) {
	if (value_a > value_b)
		return value_a;
	else
		return value_b;
}

// from here: https://github.com/FxDev/PetitModbus/blob/master/PetitModbus.c
/*
 * Function Name        : CRC16
 * @param[in]           : ui8_data  - Data to Calculate CRC
 * @param[in/out]       : ui16_crc   - Anlik CRC degeri
 * @How to use          : First initial data has to be 0xFFFF.
 */
void crc16(uint8_t ui8_data, uint16_t *ui16_crc) {
	unsigned int i;

	*ui16_crc = *ui16_crc ^ (uint16_t) ui8_data;
	for (i = 8; i > 0; i--) {
		if (*ui16_crc & 0x0001)
			*ui16_crc = (*ui16_crc >> 1) ^ 0xA001;
		else
			*ui16_crc >>= 1;
	}
}

//// reverses a string 'str' of length 'len'
//void reverse(char *str, int len)
//{
//    int i=0, j=len-1, temp;
//    while (i<j)
//    {
//        temp = str[i];
//        str[i] = str[j];
//        str[j] = temp;
//        i++; j--;
//    }
//}
//
// // Converts a given integer x to string str[].  d is the number
// // of digits required in output. If d is more than the number
// // of digits in x, then 0s are added at the beginning.
//int intToStr(int x, char str[], int d)
//{
//    int i = 0;
//    while (x)
//    {
//        str[i++] = (x%10) + '0';
//        x = x/10;
//    }
//
//    // If number of digits required is more, then
//    // add 0s at the beginning
//    while (i < d)
//        str[i++] = '0';
//
//    reverse(str, i);
//    str[i] = '\0';
//    return i;
//}
//
//// Converts a floating point number to string.
//void ftoa(float n, char *res, int afterpoint)
//{
//    // Extract integer part
//    int ipart = (int)n;
//
//    // Extract floating part
//    float fpart = n - (float)ipart;
//
//    // convert integer part to string
//    int i = intToStr(ipart, res, 0);
//
//    // check for display option after point
//    if (afterpoint != 0)
//    {
//        res[i] = '.';  // add dot
//
//        // Get the value of fraction part upto given no.
//        // of points after dot. The third parameter is needed
//        // to handle cases like 233.007
//        fpart = fpart * pow(10, afterpoint);
//
//        intToStr((int)fpart, res + i + 1, afterpoint);
//    }
//}

#define INT_DIGITS 19   /* enough for 64 bit integer */
uint8_t* itoa(uint32_t ui32_i) {
	/* Room for INT_DIGITS digits, - and '\0' */
	static uint8_t buf[INT_DIGITS + 2];
	uint8_t *p = buf + INT_DIGITS + 1; /* points to terminating '\0' */
	if (ui32_i >= 0) {
		do {
			*--p = '0' + (ui32_i % 10);
			ui32_i /= 10;
		} while (ui32_i != 0);
		return p;
	} else { /* i < 0 */
		do {
			*--p = '0' - (ui32_i % 10);
			ui32_i /= 10;
		} while (ui32_i != 0);
		*--p = '-';
	}
	return p;
}
