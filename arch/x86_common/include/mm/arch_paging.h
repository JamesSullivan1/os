/*
Copyright (c) 2016, James Sullivan <sullivan.james.f@gmail.com>
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of the <organization> nor the
      names of its contributors may be used to endorse or promote
      products derived from this software without specific prior
      written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER>
BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef _MM_ARCH_PAGING_H_
#define _MM_ARCH_PAGING_H_

#include <machine/types.h>
#include <machine/params.h>
#include <stddef.h>
#include <stdint.h>

#if WORD_SIZE == 64
#include <mm/arch_paging64.h>
#else
#include <mm/arch_paging32.h>
#endif

#define ALL_BITS (PG_BITS + PTE_BITS + PMD_BITS + PUD_BITS + PGD_BITS)

#define PAGE_SIZE  (1 << PG_BITS)
#define PAGE_SHIFT PG_BITS

#define _pa(x)  ((paddr_t)x-KERN_BASE)
#define _va(x)  ((vaddr_t)x+KERN_BASE)

#define paddr_of(pfn) ((paddr_t)((pfn) << PAGE_SHIFT))

#define PFN_UP(x)       (((x) + PAGE_SIZE-1) >> PAGE_SHIFT)
#define PFN_DOWN(x)     ( (x)                >> PAGE_SHIFT)

#define PGD_NUM         (1 << PGD_BITS)
#define PUD_NUM         (PUD_BITS == 0 ? 0 : (1 << PUD_BITS))
#define PMD_NUM         (PMD_BITS == 0 ? 0 : (1 << PMD_BITS))
#define PTE_NUM         (1 << PTE_BITS)

#define PGD_SIZE        PGD_NUM * sizeof(pgent_t)
#define PUD_SIZE        PUD_NUM * sizeof(pgent_t)
#define PMD_SIZE        PMD_NUM * sizeof(pgent_t)
#define PTE_SIZE        PTE_NUM * sizeof(pgent_t)

#define PUD_SHIFT       (PG_BITS + PTE_BITS + PMD_BITS + PUD_BITS)
#define PMD_SHIFT       (PG_BITS + PTE_BITS + PMD_BITS)
#define PTE_SHIFT       (PG_BITS + PTE_BITS)

/* This is the bytes addressable by a full page table of each type. */
#define PUD_REGN        ((size_t)1 << PUD_SHIFT)
#define PMD_REGN        ((size_t)1 << PMD_SHIFT)
#define PTE_REGN        ((size_t)1 << PTE_SHIFT)

/* Returns the number of a given page table type needed to hold sz bytes */
#define PUDS_NEEDED(sz) (PUD_BITS == 0 ? 0 : (((sz) + PUD_REGN) >> PUD_SHIFT))
#define PMDS_NEEDED(sz) (PMD_BITS == 0 ? 0 : (((sz) + PMD_REGN) >> PMD_SHIFT))
#define PTES_NEEDED(sz) (((sz) + PTE_REGN) >> PTE_SHIFT)

#define PGD_IND(pa)     (((pa) >> PUD_SHIFT ) & (PGD_NUM - 1))
#define PUD_IND(pa)     (((pa) >> PMD_SHIFT ) & (PUD_NUM - 1))
#define PMD_IND(pa)     (((pa) >> PTE_SHIFT ) & (PMD_NUM - 1))
#define PTE_IND(pa)     (((pa) >> PAGE_SHIFT) & (PTE_NUM - 1))

static const size_t PGD_COUNT = (1);
static const size_t PUD_COUNT = (PUD_BITS == 0 ? 0
                              : (1 << (ALL_BITS - PUD_SHIFT)));
static const size_t PMD_COUNT = (PMD_BITS == 0 ? 0
                              : (1 << (ALL_BITS - PMD_SHIFT)));
static const size_t PTE_COUNT = (1 << (ALL_BITS - PTE_SHIFT));

typedef struct {
        pgent_t ents[PGD_NUM];
} pgd_t __attribute__((aligned(0x20)));

typedef struct {
        pgent_t ents[PUD_NUM];
} pud_t __attribute__((aligned(0x1000)));

typedef struct {
        pgent_t ents[PMD_NUM];
} pmd_t __attribute__((aligned(0x1000)));

typedef struct {
        pgent_t ents[PTE_NUM];
} pte_t __attribute__((aligned(0x1000)));

#define pgent_paddr(ent) ((ent) & (~(PAGE_SIZE - 1)))

static inline void
_tlb_flush(vaddr_t va)
{
        __asm__ __volatile__("invlpg (%0)" : : "r"(va));
}

#endif
