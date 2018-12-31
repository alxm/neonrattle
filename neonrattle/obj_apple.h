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

#pragma once

#include "platform.h"

#include "util_fix.h"

#define O_APPLE_NUM_MAX 128
#define O_APPLE_GROW_PER 1

typedef struct OApple OApple;

typedef enum {
    O_APPLE_LIST_INVALID = -1,
    O_APPLE_LIST_GRID,
    O_APPLE_LIST_NUM
} OAppleListId;

extern const size_t o_apple_listNodeOffsets[O_APPLE_LIST_NUM];

extern void o_apple_setup(void);

extern OApple* o_apple_new(ZFix X, ZFix Y);
extern void o_apple_free(OApple* Apple);

extern ZVectorFix o_apple_coordsGet(const OApple* Apple);
extern ZFix o_apple_dimGet(const OApple* Apple);

extern void o_apple_tick(OApple* Apple);
extern void o_apple_draw(const OApple* Apple);
