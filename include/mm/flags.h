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

#ifndef _MM_FLAGS_H_
#define _MM_FLAGS_H_

#include <stdint.h>
#include <sys/bitops_generic.h>

#define M_WAIT_BIT 0
#define M_HIGH_BIT 1
#define M_DMA_BIT  2
#define M_ZERO_BIT 3
#define MFLAGS_GOOD_MASK (GENMASK(4, 0))
#define BAD_MFLAGS(f) ((f) & ~MFLAGS_GOOD_MASK)

#define M_WAIT     (1 << M_WAIT_BIT)
#define M_HIGH     (1 << M_HIGH_BIT)
#define M_DMA      (1 << M_DMA_BIT)
#define M_ZERO     (1 << M_ZERO_BIT)

#define M_BUFFER   (         M_WAIT)
#define M_ATOMIC   (0)
#define M_USER     (M_HIGH | M_WAIT | M_ZERO)
#define M_KERNEL   (         M_WAIT)

typedef uint32_t mflags_t;

#endif
