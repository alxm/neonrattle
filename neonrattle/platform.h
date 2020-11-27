/*
    Copyright 2018-2019 Alex Margarit <alex@alxm.org>
    This file is part of Neonrattle, a video game.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#ifdef __SAMD21G18A__
    #define Z_PLATFORM_META 1
#else
    #define Z_PLATFORM_FAUR 1
#endif

#define Z_DEBUG F_CONFIG_DEBUG

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

typedef struct {
    int r, g, b;
} ZRgb;

#if Z_PLATFORM_FAUR
    #include "platform_a2x.h"
#elif Z_PLATFORM_META
    #include "platform_meta.h"
#endif

Z_EXTERN_C_START

#define Z_UNUSED(X) (X = X)
#define Z_ARRAY_LEN(A) (sizeof(A) / sizeof(A[0]))

#define Z_FLAG_BIT(N) (1 << (N))
#define Z_FLAG_SET(Value, Mask) ((Value) |= (Mask))
#define Z_FLAG_CLEAR(Value, Mask) ((Value) &= ~(unsigned)(Mask))
#define Z_FLAG_TEST_ANY(Value, Mask) !!((Value) & (Mask))
#define Z_FLAG_TEST_ALL(Value, Mask) (((Value) & (Mask)) == (Mask))

extern void z_platform_spriteBlit(int Sprite, int X, int Y, unsigned Frame);

extern bool z_platform_buttonPressGet(int Button);

extern bool z_file_readOnce(const char* FileName, void* Buffer, size_t Size);
extern bool z_file_writeOnce(const char* FileName, const void* Buffer, size_t Size);

Z_EXTERN_C_END
