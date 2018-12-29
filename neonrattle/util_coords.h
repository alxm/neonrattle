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

#define Z_MAP_W_SHIFT (4)
#define Z_MAP_H_SHIFT (4)
#define Z_MAP_W (1 << Z_MAP_W_SHIFT)
#define Z_MAP_H (1 << Z_MAP_H_SHIFT)

#define Z_TILE_SHIFT 4
#define Z_TILE_DIM (1 << Z_TILE_SHIFT)
#define Z_TILE_MASK (Z_TILE_DIM - 1)

#define Z_GRID_MAP_SHIFT_DIFF 1

#define Z_GRID_W_SHIFT (Z_MAP_W_SHIFT - Z_GRID_MAP_SHIFT_DIFF)
#define Z_GRID_H_SHIFT (Z_MAP_H_SHIFT - Z_GRID_MAP_SHIFT_DIFF)
#define Z_GRID_W (1 << Z_GRID_W_SHIFT)
#define Z_GRID_H (1 << Z_GRID_H_SHIFT)

#define Z_CELL_SHIFT (Z_TILE_SHIFT + Z_GRID_MAP_SHIFT_DIFF)
#define Z_CELL_DIM (1 << Z_CELL_SHIFT)
#define Z_CELL_MASK (Z_CELL_DIM - 1)

#define Z_TILES_PER_CELL (1 << Z_GRID_MAP_SHIFT_DIFF)

static inline ZVectorInt z_coords_fixToTile(ZVectorFix Coords)
{
    return (ZVectorInt){z_fix_toInt(Coords.x) >> Z_TILE_SHIFT,
                        z_fix_toInt(Coords.y) >> Z_TILE_SHIFT};
}

static inline ZVectorInt z_coords_tileToGrid(ZVectorInt Tile)
{
    return (ZVectorInt){Tile.x >> Z_GRID_MAP_SHIFT_DIFF,
                        Tile.y >> Z_GRID_MAP_SHIFT_DIFF};
}

static inline ZVectorInt z_coords_tileToGridTileOffset(ZVectorInt Tile)
{
    return (ZVectorInt){Tile.x & ((1 << Z_GRID_MAP_SHIFT_DIFF) - 1),
                        Tile.y & ((1 << Z_GRID_MAP_SHIFT_DIFF) - 1)};
}

static inline ZVectorInt z_coords_fixToGrid(ZVectorFix Coords)
{
    return (ZVectorInt){z_fix_toInt(Coords.x) >> Z_CELL_SHIFT,
                        z_fix_toInt(Coords.y) >> Z_CELL_SHIFT};
}

static inline ZVectorInt z_coords_fixToGridOffset(ZVectorFix Coords)
{
    return (ZVectorInt){z_fix_toInt(Coords.x) & Z_CELL_MASK,
                        z_fix_toInt(Coords.y) & Z_CELL_MASK};
}
