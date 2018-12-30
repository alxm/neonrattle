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

typedef struct ZPoolFreeObject {
    struct ZPoolFreeObject* next;
} ZPoolFreeObject;

typedef struct {
    ZPoolFreeObject* freeList;
    const size_t capacity;
    const size_t objSize;
} ZPool;

#define Z_POOL_DECLARE(ObjectType, NumObjects, VarName)              \
    static struct {                                                  \
        ZPool header;                                                \
        ObjectType data[NumObjects];                                 \
    } g_pool__##ObjectType = {                                       \
        .header = {                                                  \
            .capacity = NumObjects,                                  \
            .objSize = sizeof(ObjectType)                            \
        }                                                            \
    };                                                               \
    static ZPool* const VarName = &g_pool__##ObjectType.header

extern void z_pool_reset(ZPool* Pool);

extern void* z_pool_alloc(ZPool* Pool);
extern void z_pool_free(ZPool* Pool, void* Object);
