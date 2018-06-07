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

#include "obj_apple.h"
#include "obj_snake.h"
#include "util_camera.h"
#include "util_pool.h"

typedef struct {
    bool wall;
} ZTile;

static ZTile g_map[Z_MAP_H][Z_MAP_W];

void z_map_setup(void)
{
    const ZPixel white = z_pixel_fromRGB(255, 255, 255);

    const ZPixel* pixels = z_sprite_getPixels(Z_SPRITE_MAP0, 0);

    for(int y = 0; y < Z_MAP_H; y++) {
        for(int x = 0; x < Z_MAP_W; x++) {
            g_map[y][x].wall = *pixels++ == white;
        }
    }
}

void z_map_init(ZList* Apples, ZSnake** Snake)
{
    z_pool_reset(Z_POOL_APPLE);

    const ZPixel red25 = z_pixel_fromRGB(63, 0, 0);
    const ZPixel red50 = z_pixel_fromRGB(127, 0, 0);
    const ZPixel red100 = z_pixel_fromRGB(255, 0, 0);
    const ZPixel green100 = z_pixel_fromRGB(0, 255, 0);

    const ZPixel* pixels = z_sprite_getPixels(Z_SPRITE_MAP0, 0);

    for(int y = 0; y < Z_MAP_H; y++) {
        for(int x = 0; x < Z_MAP_W; x++) {
            const ZPixel p = *pixels++;
            int num = 0;

            if(p == green100) {
                int sx = x * Z_TILE_DIM + Z_TILE_DIM / 2;
                int sy = y * Z_TILE_DIM + Z_TILE_DIM / 2;

                *Snake = z_snake_new(z_fix_fromInt(sx), z_fix_fromInt(sy));
            } else if(p == red25) {
                num = 1;
            } else if(p == red50) {
                num = 2;
            } else if(p == red100) {
                num = 4;
            }

            while(num--) {
                int ax = x * Z_TILE_DIM + 2 + z_random_int(Z_TILE_DIM - 4);
                int ay = y * Z_TILE_DIM + 2 + z_random_int(Z_TILE_DIM - 4);

                ZApple* a = z_apple_new(z_fix_fromInt(ax), z_fix_fromInt(ay));

                if(a != NULL) {
                    z_list_addLast(Apples, a);
                }
            }
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
