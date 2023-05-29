/*
 * ch343 application example
 *
 * Copyright (C) 2022 Nanjing Qinheng Microelectronics Co., Ltd.
 * Web: http://wch.cn
 * Author: WCH <tech@wch.cn>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 * Cross-compile with cross-gcc -I /path/to/cross-kernel/include
 *
 * Version: V1.2
 *
 * Update Log:
 * V1.0 - initial version
 * V1.1 - added support of ch344, ch9103, ch9101
 * V1.2 - added support of ch9104
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "ch343_lib.h"

static const char *device = "/dev/ch343_iodev0";

CHIPTYPE chiptype;
int gpiocount;
uint32_t gpioenable, gpiodir, gpioval;

int ch343_gpio_setval(int fd, uint32_t index, uint32_t level)
{
    int ret;
    int bit;
    // int i, bit;

    // ret = libch343_get_chiptype(fd, &chiptype);
    // if (ret) {
    //     return ret;
    // }

    // gpiocount = libch343_get_gpio_count(chiptype);
    // if (gpiocount <= 0) {
    //     printf("get gpio count error.\n");
    //     goto exit;
    // }
    // printf("current chip has %d gpios.\n", gpiocount);

    ret = libch343_gpioinfo(fd, &gpioenable, &gpiodir, &gpioval);
    if (ret) {
        printf("get gpio information error.\n");
        goto exit;
    }
    if (index > 31 || level > 1) {
        printf("%s: index is big than 31 or level is error!\n", __func__);
        return -1;
    }

    printf("\n********** GPIO Status Dump **********\n");

    if ((chiptype == CHIP_CH9102X) && (index >= 4))
        bit = index + 1;
    else if ((chiptype == CHIP_CH9101UH) && (index >= 5))
        bit = index + 1;
    else
        bit = index;

    gpioenable |= (1 << bit);
    gpiodir |= (1 << bit);
    gpioval = (gpioval & ~(1 << bit)) | (level << bit);

    ret = libch343_gpioenable(fd, gpioenable, gpiodir);
    if (ret) {
        printf("%s: libch343_gpioenable error.\n", __func__);
        goto exit;
    }

    ret = libch343_gpioset(fd, BIT(index), gpioval);
    if (ret) {
        printf("%s: libch343_gpioset error.\n", __func__);
        goto exit;
    }
    printf("\n");
    printf("\n********** GPIO Status End **********\n\n");

exit:
    return ret;
}

int ch343_gpioinfo(int fd)
{
    int ret;
    int i, bit;

    ret = libch343_get_chiptype(fd, &chiptype);
    if (ret) {
        return ret;
    }

    gpiocount = libch343_get_gpio_count(chiptype);
    if (gpiocount <= 0) {
        printf("get gpio count error.\n");
        goto exit;
    }
    printf("current chip has %d gpios.\n", gpiocount);

    ret = libch343_gpioinfo(fd, &gpioenable, &gpiodir, &gpioval);
    if (ret) {
        printf("get gpio information error.\n");
        goto exit;
    }

    printf("\n********** GPIO Status Dump **********\n");
    printf("\nGPIO Number\t");
    for (i = 0; i < gpiocount; i++) {
        if ((chiptype == CHIP_CH9102X) && (i >= 4))
            bit = i + 1;
        else if ((chiptype == CHIP_CH9101UH) && (i >= 5))
            bit = i + 1;
        else
            bit = i;
        printf("---%d", bit);
    }
    printf("\nGPIO Enable\t");
    for (i = 0; i < gpiocount; i++) {
        if ((chiptype == CHIP_CH9102X) && (i >= 4))
            bit = i + 1;
        else if ((chiptype == CHIP_CH9101UH) && (i >= 5))
            bit = i + 1;
        else
            bit = i;
        if (i > 9)
            printf("----%c", (gpioenable & (1 << bit)) ? 'Y' : 'N');
        else
            printf("---%c", (gpioenable & (1 << bit)) ? 'Y' : 'N');
    }
    printf("\nGPIO DirBit\t");
    for (i = 0; i < gpiocount; i++) {
        if ((chiptype == CHIP_CH9102X) && (i >= 4))
            bit = i + 1;
        else if ((chiptype == CHIP_CH9101UH) && (i >= 5))
            bit = i + 1;
        else
            bit = i;
        if (i > 9)
            printf("----%c", (gpiodir & (1 << bit)) ? 'O' : 'I');
        else
            printf("---%c", (gpiodir & (1 << bit)) ? 'O' : 'I');
    }

    printf("\nGPIO ValBit\t");
    for (i = 0; i < gpiocount; i++) {
        if ((chiptype == CHIP_CH9102X) && (i >= 4))
            bit = i + 1;
        else if ((chiptype == CHIP_CH9101UH) && (i >= 5))
            bit = i + 1;
        else
            bit = i;
        if (i > 9)
            printf("----%c", (gpioval & (1 << bit)) ? 'H' : 'L');
        else
            printf("---%c", (gpioval & (1 << bit)) ? 'H' : 'L');
    }
    printf("\n");
    printf("\n********** GPIO Status End **********\n\n");

exit:
    return ret;
}

void ch343_gpio_out_test(int fd)
{
    int ret;
    int i, j, bit;
    uint32_t lgpioenable = 0x00, lgpiodir = 0x00, lgpioval = 0x00;
    uint32_t lgpiobits = 0x00, lgpiolevelbits = 0x00;

    /* set all gpios to input first */
    ret = libch343_gpioenable(fd, lgpioenable, lgpiodir);
    if (ret) {
        printf("libch343_gpioenable error.\n");
        return;
    }

    for (i = 0; i < gpiocount; i++) {
        if ((chiptype == CHIP_CH9102X) && (i >= 4))
            bit = i + 1;
        else if ((chiptype == CHIP_CH9101UH) && (i >= 5))
            bit = i + 1;
        else
            bit = i;
        lgpioenable |= 1 << bit;
        lgpiodir |= 1 << bit;
    }

    for (i = 0; i < gpiocount; i++) {
        if ((chiptype == CHIP_CH9102X) && (i >= 4))
            bit = i + 1;
        else if ((chiptype == CHIP_CH9101UH) && (i >= 5))
            bit = i + 1;
        else
            bit = i;
        lgpiobits |= 1 << bit;
    }

    ret = libch343_gpioenable(fd, lgpioenable, lgpiodir);
    if (ret) {
        printf("libch343_gpioenable error.\n");
        return;
    }

    /* analog leds here */
    libch343_gpioset(fd, lgpiobits, 0x00);

    printf("\n********** GPIO Output Start **********\n");
    for (i = 0; i < gpiocount; i++) {
        if ((chiptype == CHIP_CH9102X) && (i >= 4))
            bit = i + 1;
        else if ((chiptype == CHIP_CH9101UH) && (i >= 5))
            bit = i + 1;
        else
            bit = i;
        lgpiolevelbits = 1 << bit;
        ret = libch343_gpioset(fd, lgpiobits, lgpiolevelbits);
        if (ret) {
            printf("libch343_gpioset error.\n");
            return;
        }
        printf("\n");
        for (j = 0; j < gpiocount; j++) {
            if (j == i)
                printf("H");
            else
                printf("L");
        }
        printf("\n");
        usleep(200 * 1000);
    }
    libch343_gpioset(fd, lgpiobits, 0x00);
    printf("\n********** GPIO Output End **********\n\n");
}

void ch343_gpio_in_test(int fd)
{
    int ret;
    int i, bit;
    uint32_t lgpioenable = 0x00, lgpiodir = 0x00, lgpioval = 0x00;

    for (i = 0; i < gpiocount; i++) {
        if ((chiptype == CHIP_CH9102X) && (i >= 4))
            bit = i + 1;
        else if ((chiptype == CHIP_CH9101UH) && (i >= 5))
            bit = i + 1;
        else
            bit = i;
        lgpioenable |= 1 << bit;
    }
    for (i = 0; i < gpiocount; i++) {
        if ((chiptype == CHIP_CH9102X) && (i >= 4))
            bit = i + 1;
        else if ((chiptype == CHIP_CH9101UH) && (i >= 5))
            bit = i + 1;
        else
            bit = i;
        lgpiodir &= ~(1 << bit);
    }
    ret = libch343_gpioenable(fd, lgpioenable, lgpiodir);
    if (ret) {
        printf("libch343_gpioenable error.\n");
        return;
    }
    ret = libch343_gpioget(fd, &lgpioval);
    if (ret) {
        printf("libch343_gpioget error.\n");
        return;
    }

    printf("\n********** GPIO Input Status **********\n");
    printf("\nGPIO Number\t");
    for (i = 0; i < gpiocount; i++) {
        if ((chiptype == CHIP_CH9102X) && (i >= 4))
            bit = i + 1;
        else if ((chiptype == CHIP_CH9101UH) && (i >= 5))
            bit = i + 1;
        else
            bit = i;
        printf("---%d", bit);
    }
    printf("\nGPIO ValBit\t");

    for (i = 0; i < gpiocount; i++) {
        if ((chiptype == CHIP_CH9102X) && (i >= 4))
            bit = i + 1;
        else if ((chiptype == CHIP_CH9101UH) && (i >= 5))
            bit = i + 1;
        else
            bit = i;
        if (i > 9)
            printf("----%c", (lgpioval & (1 << bit)) ? 'H' : 'L');
        else
            printf("---%c", (lgpioval & (1 << bit)) ? 'H' : 'L');
    }
    printf("\n");
    printf("\n********** GPIO Input End **********\n\n");
}

void ch343_gpiotest(int fd)
{
    char c;

    while (1) {
        if (c != '\n')
            printf(
                "g to get gpio information, press o to test gpio output, "
                "i to get gpio status, q for quit.\n");
        scanf("%c", &c);
        if (c == 'q') break;
        switch (c) {
        case 'g':
            ch343_gpioinfo(fd);
            break;
        case 'o':
            ch343_gpio_out_test(fd);
            break;
        case 'i':
            ch343_gpio_in_test(fd);
            break;
        default:
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    int fd;
    int ret;
    int i;

    fd = libch343_open(device);
    if (fd < 0) {
        printf("libch343_open error.\n");
        exit(0);
    }

    ret = ch343_gpioinfo(fd);
    if (ret) {
        printf("ch343_gpioinfo error.\n");
    }
    printf("argc: %d\n", argc);
    for (i = 1; i < argc; i++) {
        printf("argc[%d]: %d\n", i, atoi(argv[i]));
    }
    if (argc >= 3) {
        ch343_gpio_setval(fd, atoi(argv[1]), atoi(argv[2]));
    }

    ret = ch343_gpioinfo(fd);
    if (ret) {
        printf("ch343_gpioinfo error.\n");
    }
    // ch343_gpiotest(fd);

    ret = libch343_close(fd);
    if (ret != 0) {
        printf("libch343_close error.\n");
        exit(0);
    }
}
