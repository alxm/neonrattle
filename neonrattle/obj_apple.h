/*
    Copyright 2018 Alex Margarit <alex@alxm.org>

    Neon Rattle is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Neon Rattle is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Neon Rattle.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include "util_fix.h"

#define Z_APPLE_NUM_MAX 128
#define Z_APPLE_GROW_PER 1

typedef struct ZApple ZApple;

typedef enum {
    Z_APPLE_LIST_INVALID = -1,
    Z_APPLE_LIST_GRID,
    Z_APPLE_LIST_NUM
} ZAppleListId;

extern const size_t z_apple_listNodeOffsets[Z_APPLE_LIST_NUM];

extern void z_apple_setup(void);

extern ZApple* z_apple_new(ZFix X, ZFix Y);
extern void z_apple_free(ZApple* Apple);

extern void z_apple_getCoords(const ZApple* Apple, ZFix* X, ZFix* Y);
extern int z_apple_getDim(const ZApple* Apple);

extern void z_apple_tick(ZApple* Apple);
extern void z_apple_draw(const ZApple* Apple);
