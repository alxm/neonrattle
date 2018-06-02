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

#include "util_camera.h"

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
    ZFix originX, originY;
    z_camera_getOrigin(&originX, &originY);

    int topLeftMapPixelX = z_fix_toInt(originX) - Z_SCREEN_W / 2;
    int topLeftMapPixelY = z_fix_toInt(originY) - Z_SCREEN_H / 2;

    int topLeftTileX = topLeftMapPixelX >> Z_TILE_SHIFT;
    int topLeftTileY = topLeftMapPixelY >> Z_TILE_SHIFT;

    int topLeftScreenX = 0 - (topLeftMapPixelX & Z_TILE_MASK);
    int topLeftScreenY = 0 - (topLeftMapPixelY & Z_TILE_MASK);

    if(topLeftMapPixelX < 0) {
        topLeftScreenX += -topLeftTileX * Z_TILE_DIM;
        topLeftTileX = 0;
    }

    if(topLeftMapPixelY < 0) {
        topLeftScreenY += -topLeftTileY * Z_TILE_DIM;
        topLeftTileY = 0;
    }

    int tileEndX = z_math_min(topLeftTileX + (Z_SCREEN_W / Z_TILE_DIM + 1),
                              Z_MAP_W);
    int tileEndY = z_math_min(topLeftTileY + (Z_SCREEN_H / Z_TILE_DIM + 1),
                              Z_MAP_H);

    for(int tileY = topLeftTileY, screenY = topLeftScreenY;
        tileY < tileEndY;
        tileY++, screenY += Z_TILE_DIM) {

        for(int tileX = topLeftTileX, screenX = topLeftScreenX;
            tileX < tileEndX;
            tileX++, screenX += Z_TILE_DIM) {

            z_sprite_blit(Z_SPRITE_TILES,
                          screenX,
                          screenY,
                          (unsigned)(g_map[tileY][tileX].wall * 4 + (tileY & 1) * 2 + (tileX & 1)));
        }
    }
}
