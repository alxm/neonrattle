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

#include "util_pool.h"

void z_pool_reset(ZPool* Pool)
{
    ZPoolFreeObject* current = (ZPoolFreeObject*)(Pool + 1);

    Pool->freeList = current;

    for(size_t numObjects = Pool->capacity; numObjects > 1; numObjects--) {
        ZPoolFreeObject* next = (ZPoolFreeObject*)(void*)
                                    ((uint8_t*)current + Pool->objSize);

        current->next = next;
        current = next;
    }

    current->next = NULL;
}

void* z_pool_alloc(ZPool* Pool)
{
    if(Pool->freeList == NULL) {
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

    ZPoolFreeObject* object = Pool->freeList;

    Pool->freeList = object->next;

    return object;
}

void z_pool_free(ZPool* Pool, void* Object)
{
    ZPoolFreeObject* block = Object;

    block->next = Pool->freeList;
    Pool->freeList = block;
}
