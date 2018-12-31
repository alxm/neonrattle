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

#include "obj_map.h"

#include "obj_apple.h"
#include "util_camera.h"
#include "util_coords.h"
#include "util_graphics.h"

typedef struct {
    bool wall;
} OMapTile;

typedef struct {
    ZList apples;
} OMapCell;

typedef struct {
    OMapTile tiles[Z_COORDS_MAP_H][Z_COORDS_MAP_W];
    OMapCell grid[Z_COORDS_GRID_H][Z_COORDS_GRID_W];
    int totalApples;
} OMap;

static OMap g_map;

void o_map_setup(void)
{
    const ZPixel white = z_pixel_fromRGB(255, 255, 255);
    const ZPixel* pixels = z_sprite_pixelsGet(Z_SPRITE_MAP0, 0);

    for(int y = 0; y < Z_COORDS_MAP_H; y++) {
        for(int x = 0; x < Z_COORDS_MAP_W; x++) {
            g_map.tiles[y][x].wall = *pixels++ == white;
        }
    }

    for(int y = Z_COORDS_GRID_H; y--; ) {
        for(int x = Z_COORDS_GRID_W; x--; ) {
            z_list_init(&g_map.grid[y][x].apples,
                        o_apple_listNodeOffsets[O_APPLE_LIST_GRID]);
        }
    }
}

void o_map_init(ZFix* StartX, ZFix* StartY)
{
    g_map.totalApples = 0;

    for(int y = Z_COORDS_GRID_H; y--; ) {
        for(int x = Z_COORDS_GRID_W; x--; ) {
            z_list_reset(&g_map.grid[y][x].apples);
        }
    }

    const ZPixel red25 = z_pixel_fromRGB(63, 0, 0);
    const ZPixel red50 = z_pixel_fromRGB(127, 0, 0);
    const ZPixel red100 = z_pixel_fromRGB(255, 0, 0);
    const ZPixel green100 = z_pixel_fromRGB(0, 255, 0);

    const ZPixel* pixels = z_sprite_pixelsGet(Z_SPRITE_MAP0, 0);

    for(int y = 0; y < Z_COORDS_MAP_H; y++) {
        for(int x = 0; x < Z_COORDS_MAP_W; x++) {
            const ZPixel p = *pixels++;
            int num = 0;

            if(p == green100) {
                *StartX = z_fix_fromInt(x) + Z_FIX_ONE / 2;
                *StartY = z_fix_fromInt(y) + Z_FIX_ONE / 2;
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

            ZVectorInt grid = z_coords_tileToGrid((ZVectorInt){x, y});
            ZVectorInt gridTileOffset = z_coords_tileToGridOffset(
                                            (ZVectorInt){x, y});

            int w = z_sprite_widthGet(Z_SPRITE_APPLE_ALPHAMASK);
            int h = z_sprite_heightGet(Z_SPRITE_APPLE_ALPHAMASK);

            ZFix startX = z_coords_pixelsToUnits(w / 2);
            ZFix endX = Z_FIX_ONE - z_coords_pixelsToUnits(w / 2 - 1);
            ZFix startY = z_coords_pixelsToUnits(h / 2);
            ZFix endY = Z_FIX_ONE - z_coords_pixelsToUnits(h / 2 - 1);

            if(gridTileOffset.x > 0 && !g_map.tiles[y][x - 1].wall) {
                startX = 0;
            } else if(gridTileOffset.x < Z_COORDS_TILES_PER_GRID - 1
                && !g_map.tiles[y][x + 1].wall) {

                endX = Z_FIX_ONE + z_coords_pixelsToUnits(1);
            }

            if(gridTileOffset.y > 0 && !g_map.tiles[y - 1][x].wall) {
                startY = 0;
            } else if(gridTileOffset.y < Z_COORDS_TILES_PER_GRID - 1
                && !g_map.tiles[y + 1][x].wall) {

                endY = Z_FIX_ONE + z_coords_pixelsToUnits(1);
            }

            while(num--) {
                ZFix ax = z_fix_fromInt(x) + z_random_range(startX, endX);
                ZFix ay = z_fix_fromInt(y) + z_random_range(startY, endY);

                OApple* a = o_apple_new(ax, ay);

                if(a != NULL) {
                    z_list_addLast(&g_map.grid[grid.y][grid.x].apples, a);
                }
            }
        }
    }

    #if Z_DEBUG_INSTRUMENT
        printf("Placed %d apples\n", g_map.totalApples);
    #endif
}

void o_map_tick(void)
{
    ZVectorInt gridStart, gridEnd;
    o_map_visibleGet(NULL, NULL, &gridStart, &gridEnd, NULL);

    for(int gridY = gridStart.y; gridY < gridEnd.y; gridY++) {
        for(int gridX = gridStart.x; gridX < gridEnd.x; gridX++) {
            Z_LIST_ITERATE(o_map_applesListGet(gridX, gridY), OApple*, apple) {
                o_apple_tick(apple);
            }
        }
    }
}

void o_map_draw(void)
{
    ZVectorInt tileStart, tileEnd;
    ZVectorInt gridStart, gridEnd;
    ZVectorInt screenStart;
    o_map_visibleGet(&tileStart, &tileEnd, &gridStart, &gridEnd, &screenStart);

    for(int tileY = tileStart.y, screenY = screenStart.y;
        tileY < tileEnd.y;
        tileY++, screenY += Z_COORDS_UNIT_PIXELS) {

        for(int tileX = tileStart.x, screenX = screenStart.x;
            tileX < tileEnd.x;
            tileX++, screenX += Z_COORDS_UNIT_PIXELS) {

            z_sprite_blit(Z_SPRITE_TILES,
                          screenX,
                          screenY,
                          (unsigned)(g_map.tiles[tileY][tileX].wall * 4
                                        + !(tileY & 1) * 2 + !(tileX & 1)));
        }
    }

    for(int gridY = gridStart.y; gridY < gridEnd.y; gridY++) {
        for(int gridX = gridStart.x; gridX < gridEnd.x; gridX++) {
            Z_LIST_ITERATE(&g_map.grid[gridY][gridX].apples, OApple*, apple) {
                o_apple_draw(apple);
            }
        }
    }
}

void o_map_visibleGet(ZVectorInt* TileStart, ZVectorInt* TileEnd, ZVectorInt* GridStart, ZVectorInt* GridEnd, ZVectorInt* ScreenStart)
{
    ZVectorFix origin = z_camera_originGet();

    ZVectorFix topLeftCoords = {
        origin.x - z_coords_pixelsToUnits(Z_SCREEN_W / 2),
        origin.y - z_coords_pixelsToUnits(Z_SCREEN_H / 2)
    };

    ZVectorInt topLeftTile = z_vectorfix_toInt(topLeftCoords);
    ZVectorInt topLeftPixels = z_coords_unitsToPixels(topLeftCoords);

    ZVectorInt topLeftScreen = {
        0 - (topLeftPixels.x & (Z_COORDS_UNIT_PIXELS - 1)),
        0 - (topLeftPixels.y & (Z_COORDS_UNIT_PIXELS - 1))
    };

    if(topLeftCoords.x < 0) {
        topLeftScreen.x += -topLeftTile.x * Z_COORDS_UNIT_PIXELS;
        topLeftTile.x = 0;
    }

    if(topLeftCoords.y < 0) {
        topLeftScreen.y += -topLeftTile.y * Z_COORDS_UNIT_PIXELS;
        topLeftTile.y = 0;
    }

    ZVectorInt tileEnd = {
        z_math_min(topLeftTile.x + (Z_SCREEN_W / Z_COORDS_UNIT_PIXELS + 1),
                   Z_COORDS_MAP_W),
        z_math_min(topLeftTile.y + (Z_SCREEN_H / Z_COORDS_UNIT_PIXELS + 1),
                   Z_COORDS_MAP_H)
    };

    if(TileStart != NULL) {
        *TileStart = topLeftTile;
        *TileEnd = tileEnd;
    }

    if(GridStart != NULL) {
        *GridStart = z_coords_tileToGrid(topLeftTile);
        *GridEnd = z_coords_tileToGrid(tileEnd);

        ZVectorInt gridTileOffset = z_coords_tileToGridOffset(tileEnd);

        if(gridTileOffset.x > 0) {
            GridEnd->x += 1;
        }

        if(gridTileOffset.y > 0) {
            GridEnd->y += 1;
        }
    }

    if(ScreenStart != NULL) {
        ZVectorInt shake = z_camera_shakeGet();

        topLeftScreen.x += shake.x;
        topLeftScreen.y += shake.y;

        *ScreenStart = topLeftScreen;
    }
}

ZList* o_map_applesListGet(int GridX, int GridY)
{
    return &g_map.grid[GridY][GridX].apples;
}

int o_map_applesNumGet(void)
{
    return g_map.totalApples;
}

bool o_map_isWall(ZVectorInt Tile)
{
    return g_map.tiles[Tile.y][Tile.x].wall;
}
