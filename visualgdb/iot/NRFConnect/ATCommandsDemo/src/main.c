/*
 * Copyright (c) 2018 Nordic Semiconductor ASA
 *
 * SPDX-License-Identifier: LicenseRef-BSD-5-Clause-Nordic
 */

#include <zephyr.h>
#include <stdio.h>
#include <drivers/uart.h>
#include <string.h>

/**@brief Recoverable BSD library error. */
void bsd_recoverable_error_handler(uint32_t err)
{
	printk("bsdlib recoverable error: %u\n", err);
}

void __attribute__((noinline)) SysprogsWaitForDebugger()
{
    static volatile int SysprogsDebuggerAttached = 0;
    while (!SysprogsDebuggerAttached)
    {
    }
}

void main(void)
{
    SysprogsWaitForDebugger();
    printk("The AT host sample started (with reset fix)\n");
}
