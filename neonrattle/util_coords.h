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

static inline void z_coords_fixToTile(ZFix X, ZFix Y, int* TileX, int* TileY)
{
    *TileX = z_fix_toInt(X) >> Z_TILE_SHIFT;
    *TileY = z_fix_toInt(Y) >> Z_TILE_SHIFT;
}

static inline void z_coords_tileToGrid(int TileX, int TileY, int* GridX, int* GridY)
{
    *GridX = TileX >> Z_GRID_MAP_SHIFT_DIFF;
    *GridY = TileY >> Z_GRID_MAP_SHIFT_DIFF;
}

static inline void z_coords_tileToGridTileOffset(int TileX, int TileY, int* GridTileOffsetX, int* GridTileOffsetY)
{
    *GridTileOffsetX = TileX & ((1 << Z_GRID_MAP_SHIFT_DIFF) - 1);
    *GridTileOffsetY = TileY & ((1 << Z_GRID_MAP_SHIFT_DIFF) - 1);
}

static inline void z_coords_fixToGrid(ZFix X, ZFix Y, int* GridX, int* GridY)
{
    *GridX = z_fix_toInt(X) >> Z_CELL_SHIFT;
    *GridY = z_fix_toInt(Y) >> Z_CELL_SHIFT;
}

static inline void z_coords_fixToGridOffset(ZFix X, ZFix Y, int* GridOffsetX, int* GridOffsetY)
{
    *GridOffsetX = z_fix_toInt(X) & Z_CELL_MASK;
    *GridOffsetY = z_fix_toInt(Y) & Z_CELL_MASK;
}
