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

#ifndef _MACHINE_REGS_H_
#define _MACHINE_REGS_H_

#include <stdint.h>

/* Register context */
struct regs
{
        uint64_t savfp, savpc; /* Copy of rbp, rip */
        uint64_t rdi, rsi, rdx, rcx;
        uint64_t r8,  r9,  rax, rbx;
        uint64_t rbp, r10, r11, r12;
        uint64_t r13, r14, r15;
        uint64_t ds, es, fs, gs;
        uint64_t int_no, err_code;
        uint64_t rip, cs, flags, rsp, ss;
};

static inline unsigned long
get_cr2(void)
{
        unsigned long ret;
        __asm__ __volatile__(
                "mov %%cr2, %0"
                : "=a" (ret));
        return ret;
}

static inline void
set_cr3(uint64_t val)
{
        __asm__ __volatile__(
                "mov %0, %%cr3"
                : : "r" (val));
}

void dump_regs_from(struct regs *r);
void dump_regs(void);
void get_regs(struct regs *to);
void backtrace(unsigned int max);

#endif