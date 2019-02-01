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

#include "platform.h"

typedef struct ZListNode {
    struct ZListNode* prev;
    struct ZListNode* next;
} ZListNode;

typedef struct ZList {
    ZListNode root;
    size_t nodeOffset;
} ZList;

typedef struct ZListIt {
    ZList* list;
    ZListNode* next;
} ZListIt;

extern void z_list_init(ZList* List, size_t NodeOffset);
extern void z_list_reset(ZList* List);
extern void z_list_addFirst(ZList* List, void* Object);
extern void z_list_addLast(ZList* List, void* Object);
extern void z_list_remove(ZListNode* Node);
extern bool z_list_isEmpty(const ZList* List);

extern ZListIt z_listit__new(ZList* List);
extern bool z_listit__getNext(ZListIt* Iterator, void* UserPtrAddress);

#define Z_LIST_ITERATE(List, PtrType, VarName)                              \
    for(ZListIt z__it = z_listit__new(List); z__it.list; z__it.list = NULL) \
        for(PtrType VarName; z_listit__getNext(&z__it, (void*)&VarName); )

#define Z_LIST_REMOVE_CURRENT() z_list_remove(z__it.current)

static inline void z_list_clearNode(ZListNode* Node)
{
    *Node = (ZListNode){NULL, NULL};
}
