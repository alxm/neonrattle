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

#include "platform.h"
#include "util_map.h"

#include "obj_apple.h"
#include "util_camera.h"
#include "util_coords.h"
#include "util_pool.h"

typedef struct {
    bool wall;
} ZTile;

typedef struct {
    ZList apples;
} ZCell;

typedef struct {
    ZTile tiles[Z_MAP_H][Z_MAP_W];
    ZCell grid[Z_GRID_H][Z_GRID_W];
    int totalApples;
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
            z_list_init(&g_map.grid[y][x].apples,
                        z_apple_listNodeOffsets[Z_APPLE_LIST_GRID]);
        }
    }
}

void z_map_init(ZFix* StartX, ZFix* StartY)
{
    g_map.totalApples = 0;

    for(int y = Z_GRID_H; y--; ) {
        for(int x = Z_GRID_W; x--; ) {
            z_list_reset(&g_map.grid[y][x].apples);
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
                *StartX = z_fix_fromInt(x * Z_TILE_DIM + Z_TILE_DIM / 2);
                *StartY = z_fix_fromInt(y * Z_TILE_DIM + Z_TILE_DIM / 2);
            } else if(p == red25) {
                num = 1;
            } else if(p == red50) {
                num = 2;
            } else if(p == red100) {
                num = 4;
            }

            if(num == 0) {
                continue;
            }

            g_map.totalApples += num;

            int gridX, gridY;
            z_coords_tileToGrid(x, y, &gridX, &gridY);

            int gridTileOffsetX, gridTileOffsetY;
            z_coords_tileToGridTileOffset(
                x, y, &gridTileOffsetX, &gridTileOffsetY);

            int w = z_sprite_getWidth(Z_SPRITE_APPLE_ALPHAMASK);
            int h = z_sprite_getHeight(Z_SPRITE_APPLE_ALPHAMASK);

            int startX = w / 2;
            int endX = Z_TILE_DIM - w / 2 + 1;
            int startY = h / 2;
            int endY = Z_TILE_DIM - h / 2 + 1;

            if(gridTileOffsetX > 0 && !g_map.tiles[y][x - 1].wall) {
                startX = 0;
            } else if(gridTileOffsetX < Z_TILES_PER_CELL - 1
                && !g_map.tiles[y][x + 1].wall) {

                endX = Z_TILE_DIM + 1;
            }

            if(gridTileOffsetY > 0 && !g_map.tiles[y - 1][x].wall) {
                startY = 0;
            } else if(gridTileOffsetY < Z_TILES_PER_CELL - 1
                && !g_map.tiles[y + 1][x].wall) {

                endY = Z_TILE_DIM + 1;
            }

            while(num--) {
                int ax = x * Z_TILE_DIM + z_random_range(startX, endX);
                int ay = y * Z_TILE_DIM + z_random_range(startY, endY);

                ZApple* a = z_apple_new(z_fix_fromInt(ax), z_fix_fromInt(ay));

                if(a != NULL) {
                    z_list_addLast(&g_map.grid[gridY][gridX].apples, a);
                }
            }
        }
    }

    #if Z_DEBUG_STATS && A_PLATFORM_SYSTEM_DESKTOP
        printf("Placed %d apples\n", g_map.totalApples);
    #endif
}

void z_map_tick(void)
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
            Z_LIST_ITERATE(z_map_getApples(gridX, gridY), ZApple*, apple) {
                z_apple_tick(apple);
            }
        }
    }
}

void z_map_draw(void)
{
    ZVectorInt shake = z_screen_shakeGet();
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

    for(int tileY = tileStartY, screenY = screenStartY + shake.y;
        tileY < tileEndY;
        tileY++, screenY += Z_TILE_DIM) {

        for(int tileX = tileStartX, screenX = screenStartX + shake.x;
            tileX < tileEndX;
            tileX++, screenX += Z_TILE_DIM) {

            z_sprite_blit(Z_SPRITE_TILES,
                          screenX,
                          screenY,
                          (unsigned)(g_map.tiles[tileY][tileX].wall * 4
                                        + !(tileY & 1) * 2 + !(tileX & 1)));
        }
    }

    for(int gridY = gridStartY; gridY < gridEndY; gridY++) {
        for(int gridX = gridStartX; gridX < gridEndX; gridX++) {
            Z_LIST_ITERATE(&g_map.grid[gridY][gridX].apples, ZApple*, apple) {
                z_apple_draw(apple);
            }
        }
    }
}

void z_map_getVisibleBounds(
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

        int gridTileOffsetX, gridTileOffsetY;
        z_coords_tileToGridTileOffset(
            tileEndX, tileEndY, &gridTileOffsetX, &gridTileOffsetY);

        if(gridTileOffsetX > 0) {
            *GridEndX += 1;
        }

        if(gridTileOffsetY > 0) {
            *GridEndY += 1;
        }
    }

    if(ScreenStartX != NULL) {
        *ScreenStartX = topLeftScreenX;
        *ScreenStartY = topLeftScreenY;
    }
}

ZList* z_map_getApples(int GridX, int GridY)
{
    return &g_map.grid[GridY][GridX].apples;
}

int z_map_getApplesNum(void)
{
    return g_map.totalApples;
}

bool z_map_isWall(int TileX, int TileY)
{
    return g_map.tiles[TileY][TileX].wall;
}
