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

#define Z_MAP_W 32
#define Z_MAP_H 16

#define Z_TILE_SHIFT 4
#define Z_TILE_DIM (1 << Z_TILE_SHIFT)
#define Z_TILE_MASK (Z_TILE_DIM - 1)

extern void z_map_setup(void);
extern void z_map_init(void);
extern void z_map_draw(void);
