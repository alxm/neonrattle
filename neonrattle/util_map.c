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
#include "util_collision.h"
#include "util_coords.h"
#include "util_pool.h"

typedef struct {
    bool wall;
} ZTile;

typedef struct {
    ZTile tiles[Z_MAP_H][Z_MAP_W];
    ZList grid[Z_GRID_H][Z_GRID_W];
} ZMap;

static ZMap g_map;

void z_map_setup(void)
{
    const ZPixel white = z_pixel_fromRGB(255, 255, 255);

    const ZPixel* pixels = z_sprite_getPixels(Z_SPRITE_MAP0, 0);

    for(int y = 0; y < Z_MAP_H; y++) {
        for(int x = 0; x < Z_MAP_W; x++) {
            g_map.tiles[y][x].wall = *pixels++ == white;
        }
    }

    for(int y = Z_GRID_H; y--; ) {
        for(int x = Z_GRID_W; x--; ) {
            z_list_init(&g_map.grid[y][x],
                        z_apple_listNodeOffsets[Z_APPLE_LIST_GRID]);
        }
    }
}

void z_map_init(ZSnake** Snake)
{
    z_pool_reset(Z_POOL_APPLE);

    for(int y = Z_GRID_H; y--; ) {
        for(int x = Z_GRID_W; x--; ) {
            z_list_reset(&g_map.grid[y][x]);
        }
    }

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
                int w = z_sprite_getWidth(Z_SPRITE_APPLE_ALPHAMASK);
                int h = z_sprite_getHeight(Z_SPRITE_APPLE_ALPHAMASK);
                int ax = x * Z_TILE_DIM + w / 2 + z_random_int(Z_TILE_DIM - w);
                int ay = y * Z_TILE_DIM + h / 2 + z_random_int(Z_TILE_DIM - h);

                ZApple* a = z_apple_new(z_fix_fromInt(ax), z_fix_fromInt(ay));

                if(a != NULL) {
                    int gridX, gridY;
                    z_coords_tileToGrid(x, y, &gridX, &gridY);

                    z_list_addLast(&g_map.grid[gridY][gridX], a);
                }
            }
        }
    }
}

static void z_map_getVisibleBounds(
    int* TileStartX, int* TileStartY, int* TileEndX, int* TileEndY,
    int* GridStartX, int* GridStartY, int* GridEndX, int* GridEndY,
    int* ScreenStartX, int* ScreenStartY)
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

    if(TileStartX != NULL) {
        *TileStartX = topLeftTileX;
        *TileStartY = topLeftTileY;
        *TileEndX = tileEndX;
        *TileEndY = tileEndY;
    }

    if(GridStartX != NULL) {
        z_coords_tileToGrid(topLeftTileX, topLeftTileY, GridStartX, GridStartY);
        z_coords_tileToGrid(tileEndX, tileEndY, GridEndX, GridEndY);

        if(tileEndX & ((1 << Z_GRID_MAP_SHIFT_DIFF) - 1)) {
            *GridEndX += 1;
        }

        if(tileEndY & ((1 << Z_GRID_MAP_SHIFT_DIFF) - 1)) {
            *GridEndY += 1;
        }
    }

    if(ScreenStartX != NULL) {
        *ScreenStartX = topLeftScreenX;
        *ScreenStartY = topLeftScreenY;
    }
}

void z_map_tick(ZSnake* Snake)
{
    int gridStartX, gridStartY, gridEndX, gridEndY;
    z_map_getVisibleBounds(NULL,
                           NULL,
                           NULL,
                           NULL,
                           &gridStartX,
                           &gridStartY,
                           &gridEndX,
                           &gridEndY,
                           NULL,
                           NULL);

    for(int gridY = gridStartY; gridY < gridEndY; gridY++) {
        for(int gridX = gridStartX; gridX < gridEndX; gridX++) {
            Z_LIST_ITERATE(&g_map.grid[gridY][gridX], ZApple*, apple) {
                z_apple_tick(apple);
            }
        }
    }

    ZFix snakeX, snakeY;
    z_snake_getCoords(Snake, &snakeX, &snakeY);

    int snakeGridX, snakeGridY;
    z_coords_fixToGrid(snakeX, snakeY, &snakeGridX, &snakeGridY);

    int snakeGridXOffset, snakeGridYOffset;
    z_coords_fixToGridOffset(snakeX, snakeY, &snakeGridXOffset, &snakeGridYOffset);

    if(snakeGridX > 0 && snakeGridXOffset < Z_TILE_DIM / 2) {
        gridStartX = snakeGridX - 1;
    } else {
        gridStartX = snakeGridX;
    }

    if(snakeGridY > 0 && snakeGridYOffset < Z_TILE_DIM / 2) {
        gridStartY = snakeGridY - 1;
    } else {
        gridStartY = snakeGridY;
    }

    if(snakeGridX < Z_GRID_W - 1
        && snakeGridXOffset > Z_CELL_DIM - Z_TILE_DIM / 2) {

        gridEndX = snakeGridX + 1;
    } else {
        gridEndX = snakeGridX;
    }

    if(snakeGridY < Z_GRID_H - 1
        && snakeGridYOffset > Z_CELL_DIM - Z_TILE_DIM / 2) {

        gridEndY = snakeGridY + 1;
    } else {
        gridEndY = snakeGridY;
    }

    for(int gridY = gridStartY; gridY <= gridEndY; gridY++) {
        for(int gridX = gridStartX; gridX <= gridEndX; gridX++) {
            Z_LIST_ITERATE(&g_map.grid[gridY][gridX], ZApple*, apple) {
                ZFix appleX, appleY;
                z_apple_getCoords(apple, &appleX, &appleY);

                if(z_collision_sqAndSq(z_fix_toInt(snakeX),
                                       z_fix_toInt(snakeY),
                                       z_snake_getDim(Snake),
                                       z_fix_toInt(appleX),
                                       z_fix_toInt(appleY),
                                       z_apple_getDim(apple))) {

                    z_snake_grow(Snake, Z_APPLE_GROW_PER);

                    Z_LIST_REMOVE_CURRENT();
                    z_apple_free(apple);
                }
            }
        }
    }
}

void z_map_draw(void)
{
    int tileStartX, tileStartY, tileEndX, tileEndY;
    int gridStartX, gridStartY, gridEndX, gridEndY;
    int screenStartX, screenStartY;
    z_map_getVisibleBounds(&tileStartX,
                           &tileStartY,
                           &tileEndX,
                           &tileEndY,
                           &gridStartX,
                           &gridStartY,
                           &gridEndX,
                           &gridEndY,
                           &screenStartX,
                           &screenStartY);

    for(int tileY = tileStartY, screenY = screenStartY;
        tileY < tileEndY;
        tileY++, screenY += Z_TILE_DIM) {

        for(int tileX = tileStartX, screenX = screenStartX;
            tileX < tileEndX;
            tileX++, screenX += Z_TILE_DIM) {

            z_sprite_blit(Z_SPRITE_TILES,
                          screenX,
                          screenY,
                          (unsigned)(g_map.tiles[tileY][tileX].wall * 4
                                        + (tileY & 1) * 2 + (tileX & 1)));
        }
    }

    for(int gridY = gridStartY; gridY < gridEndY; gridY++) {
        for(int gridX = gridStartX; gridX < gridEndX; gridX++) {
            Z_LIST_ITERATE(&g_map.grid[gridY][gridX], ZApple*, apple) {
                z_apple_draw(apple);
            }
        }
    }
}
