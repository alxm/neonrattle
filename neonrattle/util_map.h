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

#include "util_list.h"

extern void z_map_setup(void);
extern void z_map_init(ZFix* StartX, ZFix* StartY);
extern void z_map_tick(void);
extern void z_map_draw(void);

extern void z_map_getVisibleBounds(
    int* TileStartX, int* TileStartY, int* TileEndX, int* TileEndY,
    int* GridStartX, int* GridStartY, int* GridEndX, int* GridEndY,
    int* ScreenStartX, int* ScreenStartY);
extern ZList* z_map_getApples(int GridX, int GridY);
extern bool z_map_isWall(int TileX, int TileY);
