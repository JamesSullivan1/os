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

#ifndef _MACHINE_SPINLOCK_H_
#define _MACHINE_SPINLOCK_H_

#define SPINLOCK_HELD 1
#define SPINLOCK_FREE 0

typedef volatile uintptr_t spinlock_t;

static inline void
spin_lock(spinlock_t *lock)
{
        __asm__ __volatile__(
                "mov %1, %%eax\n"
                "xchg %%eax, (%0)\n"
                "test %%eax, %%eax\n"
                "je .locked\n"
                ".loop:\n"
                "pause\n"
                "xchg (%0), %%eax\n"
                "test %%eax, %%eax\n"
                "jne .loop\n"
                ".locked:\n"
                :
                : "r" (lock), "a" (SPINLOCK_HELD)
                : "eax"
        );
}

static inline void
spin_unlock(spinlock_t *lock)
{
        __asm__ __volatile__(
                "mov %1, %%eax\n"
                "xchg %%eax, (%0)\n"
                :
                : "r" (lock), "a" (SPINLOCK_FREE)
                : "eax"
        );
}

#endif
