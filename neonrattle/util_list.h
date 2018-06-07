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
    ZListNode* current;
} ZListIt;

extern void z_list_init(ZList* List, size_t NodeOffset);
extern void z_list_addFirst(ZList* List, void* Object);
extern void z_list_addLast(ZList* List, void* Object);
extern void z_list_remove(ZListNode* Node);

extern ZListIt z_listit__new(ZList* List);
extern bool z_listit__getNext(ZListIt* Iterator);
extern void z_listit__removeCurrent(ZListIt* Iterator);

#define Z_LIST_ITERATE(List, PtrType, VarName)                 \
    for(PtrType VarName = (PtrType)1; VarName; VarName = NULL) \
        for(ZListIt z__it = z_listit__new(List);               \
            z_listit__getNext(&z__it)                          \
                && (VarName =                                  \
                        (PtrType)((uint8_t*)z__it.current      \
                                    - (List)->nodeOffset),     \
                    true); )

#define Z_LIST_REMOVE_CURRENT() z_listit__removeCurrent(&z__it)
