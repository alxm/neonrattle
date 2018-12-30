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

typedef enum {
    Z_POOL_INVALID = -1,
    Z_POOL_APPLE,
    Z_POOL_CIRCLE,
    Z_POOL_PARTICLE,
    Z_POOL_SNAKE,
    Z_POOL_NUM
} ZPoolId;

typedef struct ZPoolFreeObject {
    struct ZPoolFreeObject* next;
} ZPoolFreeObject;

typedef struct ZPoolHeader {
    ZPoolFreeObject* freeList;
    const size_t capacity;
    const size_t objSize;
} ZPoolHeader;

#define Z_POOL_DECLARE(ObjectType, NumObjects, VarName)              \
    static struct {                                                  \
        ZPoolHeader header;                                          \
        ObjectType data[NumObjects];                                 \
    } g_pool__##ObjectType = {                                       \
        .header = {                                                  \
            .capacity = NumObjects,                                  \
            .objSize = sizeof(ObjectType)                            \
        }                                                            \
    };                                                               \
    static ZPoolHeader* const VarName = &g_pool__##ObjectType.header

extern void z_pool_setup(ZPoolId Id, ZPoolHeader* Pool);
extern void z_pool_reset(ZPoolId Pool);

extern void* z_pool_alloc(ZPoolId Pool);
extern void z_pool_free(ZPoolId Pool, void* Object);
