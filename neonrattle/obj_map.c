/*
    Copyright 2018, 2019 Alex Margarit <alex@alxm.org>

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

#include "obj_camera.h"
#include "util_coords.h"
#include "util_fps.h"
#include "util_graphics.h"

#define O_TILE_APPLES_MAX 4

typedef struct {
    uint8_t wall;
    uint8_t numApples;
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

void o_map_init(unsigned Level, ZFix* StartX, ZFix* StartY)
{
    g_map.totalApples = 0;

    for(int y = Z_COORDS_GRID_H; y--; ) {
        for(int x = Z_COORDS_GRID_W; x--; ) {
            z_list_init(&g_map.grid[y][x].apples,
                        o_apple_listNodeOffsets[O_APPLE_LIST_GRID]);
        }
    }

    const ZPixel* pixels = z_sprite_pixelsGet(Z_SPRITE_MAPS, Level);

    for(int y = 0; y < Z_COORDS_MAP_H; y++) {
        for(int x = 0; x < Z_COORDS_MAP_W; x++) {
            ZPixel p = *pixels++;

            g_map.tiles[y][x].wall = (p == z_colors[Z_COLOR_MAP_WHITE].pixel);
            g_map.tiles[y][x].numApples = 0;

            if(p == z_colors[Z_COLOR_MAP_GREEN].pixel) {
                *StartX = z_fix_fromInt(x) + Z_FIX_ONE / 2;
                *StartY = z_fix_fromInt(y) + Z_FIX_ONE / 2;
            }
        }
    }

    pixels = z_sprite_pixelsGet(Z_SPRITE_MAPS, Level);

    for(int y = 0; y < Z_COORDS_MAP_H; y++) {
        for(int x = 0; x < Z_COORDS_MAP_W; x++) {
            ZPixel p = *pixels++;
            ZRgb rgb = z_pixel_toRgb(p);

            if(rgb.r == 0 || rgb.g > 0 || rgb.b > 0) {
                continue;
            }

            int div = 255 / (O_TILE_APPLES_MAX - 1);
            int numApples = 1 + (rgb.r + div / 2) / div;

            ZVectorInt grid = z_coords_tileToGrid((ZVectorInt){x, y});
            ZVectorInt gridTileOffset = z_coords_tileToGridOffset(
                                            (ZVectorInt){x, y});

            int w = z_sprite_widthGet(Z_SPRITE_APPLE_MASK);
            int h = z_sprite_heightGet(Z_SPRITE_APPLE_MASK);

            ZFix startX = z_coords_pixelsToUnits(w / 2);
            ZFix endX = Z_FIX_ONE - z_coords_pixelsToUnits(w / 2 - 1);
            ZFix startY = z_coords_pixelsToUnits(h / 2);
            ZFix endY = Z_FIX_ONE - z_coords_pixelsToUnits(h / 2 - 1);

            if(gridTileOffset.x > 0 && !g_map.tiles[y][x - 1].wall) {
                startX = 0;
            }

            if(gridTileOffset.x < Z_COORDS_TILES_PER_GRID - 1
                && !g_map.tiles[y][x + 1].wall) {

                endX = Z_FIX_ONE;
            }

            if(gridTileOffset.y > 0 && !g_map.tiles[y - 1][x].wall) {
                startY = 0;
            }

            if(gridTileOffset.y < Z_COORDS_TILES_PER_GRID - 1
                && !g_map.tiles[y + 1][x].wall) {

                endY = Z_FIX_ONE;
            }

            while(numApples--) {
                ZFix ax = z_fix_fromInt(x) + z_random_range(startX, endX);
                ZFix ay = z_fix_fromInt(y) + z_random_range(startY, endY);

                OApple* a = o_apple_new(ax, ay);

                if(a != NULL) {
                    z_list_addLast(&g_map.grid[grid.y][grid.x].apples, a);
                    g_map.tiles[y][x].numApples++;
                    g_map.totalApples++;
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

void o_map_drawMinimap(ZVectorFix PlayerSnake)
{
    int drawXStart = Z_SCREEN_W - Z_COORDS_MAP_W - 2;
    int drawYStart = Z_SCREEN_H - Z_COORDS_MAP_H - 2;

    z_draw_rectangleAlpha(drawXStart,
                          drawYStart - 1,
                          Z_COORDS_MAP_W,
                          1,
                          Z_COLOR_SNAKE_01,
                          48);
    z_draw_rectangleAlpha(drawXStart,
                          drawYStart + Z_COORDS_MAP_H,
                          Z_COORDS_MAP_W,
                          1,
                          Z_COLOR_SNAKE_01,
                          48);
    z_draw_rectangleAlpha(drawXStart - 1,
                          drawYStart,
                          1,
                          Z_COORDS_MAP_H,
                          Z_COLOR_SNAKE_01,
                          48);
    z_draw_rectangleAlpha(drawXStart + Z_COORDS_MAP_W,
                          drawYStart,
                          1,
                          Z_COORDS_MAP_H,
                          Z_COLOR_SNAKE_01,
                          48);

    for(int y = 0, drawY = drawYStart; y < Z_COORDS_MAP_H; y++, drawY++) {
        for(int x = 0, drawX = drawXStart; x < Z_COORDS_MAP_W; x++, drawX++) {
            if(g_map.tiles[y][x].wall) {
                z_draw_pixelCoordsIdAlpha(drawX, drawY, Z_COLOR_SNAKE_01, 128);
            } else {
                int numApples = g_map.tiles[y][x].numApples;

                if(numApples == 0) {
                    z_draw_pixelCoordsIdAlpha(
                        drawX, drawY, Z_COLOR_BG_GREEN_01, 192);
                } else {
                    ZRgb base = z_colors[Z_COLOR_BG_GREEN_01].rgb;
                    const ZRgb* target = &z_colors[Z_COLOR_APPLE_03].rgb;
                    int alpha = 160 + 96 * numApples / O_TILE_APPLES_MAX;

                    base.r += ((target->r - base.r) * alpha) >> 8;
                    base.g += ((target->g - base.g) * alpha) >> 8;
                    base.b += ((target->b - base.b) * alpha) >> 8;

                    z_draw_pixelCoordsRgbAlpha(drawX, drawY, &base, 192);
                }
            }
        }
    }

    ZVectorInt tile = z_vectorfix_toInt(PlayerSnake);

    z_sprite_blitAlphaMask(Z_SPRITE_SNAKE_MINIMAP,
                           drawXStart + tile.x,
                           drawYStart + tile.y,
                           0,
                           Z_COLOR_SNAKE_03,
                           256);
}

void o_map_visibleGet(ZVectorInt* TileStart, ZVectorInt* TileEnd, ZVectorInt* GridStart, ZVectorInt* GridEnd, ZVectorInt* ScreenStart)
{
    ZVectorFix origin = o_camera_originGet();

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
        ZVectorInt shake = o_camera_shakeGet();

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

int o_map_appleEat(OApple* Apple)
{
    ZVectorInt tile = z_vectorfix_toInt(o_apple_coordsGet(Apple));

    g_map.tiles[tile.y][tile.x].numApples--;

    return o_apple_eatSet(Apple);
}

bool o_map_isWall(ZVectorInt Tile)
{
    return g_map.tiles[Tile.y][Tile.x].wall;
}
