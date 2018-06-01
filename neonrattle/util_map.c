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
#include "util_map.h"

#define Z_MAP_W 64
#define Z_MAP_H 64

#define Z_TILE_DIM 16

typedef struct {
    bool wall;
} ZTile;

static ZTile g_map[Z_MAP_H][Z_MAP_W];

void z_map_setup(void)
{
    ZPixel* pixels = z_sprite_getPixels(Z_SPRITE_MAP0, 0);

    for(int y = 0; y < Z_MAP_H; y++) {
        for(int x = 0; x < Z_MAP_W; x++) {
            g_map[y][x].wall = *pixels++ != 0;
        }
    }
}

void z_map_draw(void)
{
    for(int y = 0; y < Z_MAP_H; y++) {
        for(int x = 0; x < Z_MAP_W; x++) {
            z_sprite_blit(Z_SPRITE_TILES,
                          x * Z_TILE_DIM,
                          y * Z_TILE_DIM,
                          (unsigned)(g_map[y][x].wall * 4 + (y & 1) * 2 + (x & 1)));
        }
    }
}
