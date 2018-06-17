/*
    Copyright 2018 Alex Margarit <alex@alxm.org>

    Neonrattle is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Neonrattle is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Neonrattle.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef A2X
    #define Z_PLATFORM_A2X 1
#endif

#ifdef ARDUINO_SAMD_ZERO
    #define Z_PLATFORM_META 1
#endif

#define Z_DEBUG_STATS 0

#ifdef __cplusplus
#define Z_EXTERN_C_START extern "C" {
#define Z_EXTERN_C_END }
#else
#define Z_EXTERN_C_START
#define Z_EXTERN_C_END
#endif

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>

#define Z_ENUM_PACK

#if Z_PLATFORM_A2X
    #include "platform_a2x.h"
#elif Z_PLATFORM_META
    #include "platform_meta.h"
#endif

Z_EXTERN_C_START

#define uN(X, Bits) (uint8_t)((X) & ((1 << (Bits)) - 1))
#define u1(X) uN(X, 1)
#define u2(X) uN(X, 2)
#define u3(X) uN(X, 3)
#define u4(X) uN(X, 4)
#define u5(X) uN(X, 5)
#define u6(X) uN(X, 6)
#define u7(X) uN(X, 7)
#define u8(X) (uint8_t)(X)
#define u16(X) (uint16_t)(X)

#define Z_UNUSED(X) (X = X)
#define Z_ARRAY_LEN(A) (sizeof(A) / sizeof(A[0]))

Z_EXTERN_C_END
