/*
 * Copyright (C) 2017-2020 Alibaba Group Holding Limited
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#include <asm/types.h>
#include <asm/asm.h>
#include <common.h>
#include <cpu_func.h>


#define L1_CACHE_SHIFT      6
#define L1_CACHE_BYTES      (1 << L1_CACHE_SHIFT)


#define csr_write(csr, val)                                        \
    ({                                                         \
        unsigned long __v = (unsigned long)(val);          \
        __asm__ __volatile__("csrw " __ASM_STR(csr) ", %0" \
                    :                             \
                    : "rK"(__v)                   \
                    : "memory");                  \
    })

#define sync_is()   asm volatile (".long 0x01b0000b")


void flush_dcache_all(void)
{
    flush_dcache_range((u64)(0x0), (u64)(0x60000000));
}

void flush_dcache_range(unsigned long start, unsigned long end)
{
    register unsigned long i asm("a0") = start & ~(L1_CACHE_BYTES - 1);

    for (; i < end; i += L1_CACHE_BYTES)
        asm volatile (".long 0x0255000b"); /* dcache.cva a0 */

    sync_is();
}

void invalidate_dcache_range(unsigned long start, unsigned long end)
{
    register unsigned long i asm("a0") = start & ~(L1_CACHE_BYTES - 1);

    for (; i < end; i += L1_CACHE_BYTES)
        asm volatile (".long 0x0275000b"); /* dcache.civa a0 */

    sync_is();
}

void icache_enable(void)
{
}

void icache_disable(void)
{
}

int icache_status(void)
{
    return 0;
}

void dcache_enable(void)
{
}

void dcache_disable(void)
{
}

int dcache_status(void)
{
    return 0;
}

