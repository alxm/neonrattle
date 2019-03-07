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

#include "util_fix.h"

#define Z_COORDS_PIXELS_PER_UNIT 16
#define Z_COORDS_TILES_PER_GRID_SHIFT 1
#define Z_COORDS_TILES_PER_GRID (1 << Z_COORDS_TILES_PER_GRID_SHIFT)
#define Z_COORDS_GRID_W (N_MAP_W / Z_COORDS_TILES_PER_GRID)
#define Z_COORDS_GRID_H (N_MAP_H / Z_COORDS_TILES_PER_GRID)

static inline ZFix z_coords_pixelsToUnits(int Pixels)
{
    return z_fix_fromInt(Pixels) / Z_COORDS_PIXELS_PER_UNIT;
}

static inline ZVectorInt z_coords_unitsToPixels(ZVectorFix Units)
{
    return (ZVectorInt){z_fix_toInt(Units.x * Z_COORDS_PIXELS_PER_UNIT),
                        z_fix_toInt(Units.y * Z_COORDS_PIXELS_PER_UNIT)};
}

static inline ZVectorInt z_coords_unitsToGrid(ZVectorFix Units)
{
    return (ZVectorInt){z_fix_toInt(Units.x) >> Z_COORDS_TILES_PER_GRID_SHIFT,
                        z_fix_toInt(Units.y) >> Z_COORDS_TILES_PER_GRID_SHIFT};
}

static inline ZVectorFix z_coords_unitsToGridOffset(ZVectorFix Units)
{
    ZVectorInt grid = z_coords_unitsToGrid(Units);

    return (ZVectorFix){
        Units.x - z_fix_fromInt(grid.x * Z_COORDS_TILES_PER_GRID),
        Units.y - z_fix_fromInt(grid.y * Z_COORDS_TILES_PER_GRID)
    };
}

static inline ZVectorInt z_coords_tileToGrid(ZVectorInt Tile)
{
    return (ZVectorInt){Tile.x >> Z_COORDS_TILES_PER_GRID_SHIFT,
                        Tile.y >> Z_COORDS_TILES_PER_GRID_SHIFT};
}

static inline ZVectorInt z_coords_tileToGridOffset(ZVectorInt Tile)
{
    ZVectorInt grid = z_coords_tileToGrid(Tile);

    return (ZVectorInt){Tile.x - grid.x * Z_COORDS_TILES_PER_GRID,
                        Tile.y - grid.y * Z_COORDS_TILES_PER_GRID};
}

static inline bool z_coords_collideSquares(ZVectorFix Coords1, ZFix Dim1, ZVectorFix Coords2, ZFix Dim2)
{
    Coords1.x -= Dim1 / 2;
    Coords1.y -= Dim1 / 2;

    Coords2.x -= Dim2 / 2;
    Coords2.y -= Dim2 / 2;

    return !(Coords1.y >= Coords2.y + Dim2
          || Coords2.y >= Coords1.y + Dim1
          || Coords1.x >= Coords2.x + Dim2
          || Coords2.x >= Coords1.x + Dim1);
}
