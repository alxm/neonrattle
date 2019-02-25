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

#include "util_list.h"

void z_list_new(ZList* List, size_t NodeOffset)
{
    List->root.prev = &List->root;
    List->root.next = &List->root;

    List->nodeOffset = NodeOffset;
}

void z_list_clear(ZList* List)
{
    List->root.prev = &List->root;
    List->root.next = &List->root;
}

void z_list_addFirst(ZList* List, void* Object)
{
    ZListNode* rootNode = &List->root;
    ZListNode* objectNode = (ZListNode*)(void*)
                                ((uint8_t*)Object + List->nodeOffset);

    objectNode->prev = rootNode;
    objectNode->next = rootNode->next;

    objectNode->prev->next = objectNode;
    objectNode->next->prev = objectNode;
}

void z_list_addLast(ZList* List, void* Object)
{
    ZListNode* rootNode = &List->root;
    ZListNode* objectNode = (ZListNode*)(void*)
                                ((uint8_t*)Object + List->nodeOffset);

    objectNode->prev = rootNode->prev;
    objectNode->next = rootNode;

    objectNode->prev->next = objectNode;
    objectNode->next->prev = objectNode;
}

void z_list_remove(ZListNode* Node)
{
    if(Node->prev == NULL) {
        return;
    }

    Node->prev->next = Node->next;
    Node->next->prev = Node->prev;

    Node->prev = NULL;
}

bool z_list_isEmpty(const ZList* List)
{
    return List->root.next == &List->root;
}

ZListIt z_listit__new(ZList* List)
{
    return (ZListIt){List, List->root.next};
}

bool z_listit__getNext(ZListIt* Iterator, void* UserPtrAddress)
{
    ZListNode* current = Iterator->next;
    Iterator->next = current->next;

    *(void**)UserPtrAddress = (uint8_t*)current - Iterator->list->nodeOffset;

    return current != &Iterator->list->root;
}
