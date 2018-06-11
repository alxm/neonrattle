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

#include "platform.h"
#include "util_pool.h"

static ZPoolHeader* g_pools[Z_POOL_NUM];

void z_pool_register(ZPoolId Id, ZPoolHeader* Pool)
{
    g_pools[Id] = Pool;
    z_pool_reset(Id);
}

void z_pool_reset(ZPoolId Pool)
{
    ZPoolHeader* pool = g_pools[Pool];
    ZPoolFreeObject* current = (ZPoolFreeObject*)(pool + 1);

    pool->freeList = current;

    for(size_t numObjects = pool->capacity; numObjects > 1; numObjects--) {
        ZPoolFreeObject* next = (ZPoolFreeObject*)
                                    ((uint8_t*)current + pool->objSize);

        current->next = next;
        current = next;
    }

    current->next = NULL;
}

void* z_pool_alloc(ZPoolId Pool)
{
    ZPoolHeader* pool = g_pools[Pool];

    if(pool->freeList == NULL) {
        #if Z_DEBUG_STATS && A_PLATFORM_SYSTEM_DESKTOP
            static uint32_t fails[Z_POOL_NUM];
            static const char* names[Z_POOL_NUM] = {
                [Z_POOL_APPLE] = "Z_POOL_APPLE",
                [Z_POOL_CIRCLE] = "Z_POOL_CIRCLE",
                [Z_POOL_PARTICLE] = "Z_POOL_PARTICLE",
                [Z_POOL_SNAKE] = "Z_POOL_SNAKE",
            };

            printf("Can't allocate from %s (%d)\n", names[Pool], ++fails[Pool]);
        #endif

        return NULL;
    }

    ZPoolFreeObject* object = pool->freeList;

    pool->freeList = object->next;

    return object;
}

void z_pool_free(ZPoolId Pool, void* Object)
{
    ZPoolHeader* pool = g_pools[Pool];
    ZPoolFreeObject* block = Object;

    block->next = pool->freeList;
    pool->freeList = block;
}
