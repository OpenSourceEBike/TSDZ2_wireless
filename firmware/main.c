/*
 * TSDZ2 wireless firmware
 *
 * Copyright (C) Casainho, 2020
 *
 * Released under the GPL License, Version 3
 */

#include <stdio.h>

uint16_t cnt_1;

int main(void)
{
    static uint16_t cnt_2;

    while (1)
    {
        cnt_1 += 3;
        cnt_2++;
        __asm__("nop");
    }
}

