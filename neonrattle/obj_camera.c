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

#include "obj_camera.h"

#include "obj_map.h"
#include "util_coords.h"
#include "util_graphics.h"
#include "util_timer.h"

typedef struct {
    ZVectorFix coords;
    ZVectorInt shake;
} NCamera;

static NCamera g_camera;

void n_camera_new(ZVectorFix Coords)
{
    g_camera.coords = Coords;
    g_camera.shake = (ZVectorInt){0, 0};

    z_timer_stop(Z_TIMER_CAMERA_SHAKE);
}

void n_camera_tick(ZVectorFix Origin)
{
    #define Z_SHIFT 3

    g_camera.coords.x = z_math_clamp(
        (Origin.x >> Z_SHIFT)
            + (g_camera.coords.x - (g_camera.coords.x >> Z_SHIFT)),
        z_coords_pixelsToUnits(Z_SCREEN_W / 2),
        z_fix_fromInt(N_MAP_W) - z_coords_pixelsToUnits(Z_SCREEN_W / 2));

    g_camera.coords.y = z_math_clamp(
        (Origin.y >> Z_SHIFT)
            + (g_camera.coords.y - (g_camera.coords.y >> Z_SHIFT)),
        z_coords_pixelsToUnits(Z_SCREEN_H / 2),
        z_fix_fromInt(N_MAP_H) - z_coords_pixelsToUnits(Z_SCREEN_H / 2));

    if(z_timer_isRunning(Z_TIMER_CAMERA_SHAKE)) {
        g_camera.shake = (ZVectorInt){z_random_range(-1, 2),
                                      z_random_range(-1, 2)};
    } else {
        g_camera.shake = (ZVectorInt){0, 0};
    }
}

ZVectorFix n_camera_originGet(void)
{
    return g_camera.coords;
}

ZVectorInt n_camera_coordsToScreen(ZVectorFix WorldCoords)
{
    ZVectorFix relative = {WorldCoords.x - g_camera.coords.x,
                           WorldCoords.y - g_camera.coords.y};

    ZVectorInt coords = z_coords_unitsToPixels(relative);

    coords.x += Z_SCREEN_W / 2 + g_camera.shake.x;
    coords.y += Z_SCREEN_H / 2 + g_camera.shake.y;

    return coords;
}

void n_camera_boundsGet(ZVectorInt* TileStart, ZVectorInt* TileEnd, ZVectorInt* GridStart, ZVectorInt* GridEnd, ZVectorInt* ScreenStart)
{
    ZVectorFix origin = n_camera_originGet();

    ZVectorFix topLeftCoords = {
        origin.x - z_coords_pixelsToUnits(Z_SCREEN_W / 2),
        origin.y - z_coords_pixelsToUnits(Z_SCREEN_H / 2)
    };

    ZVectorInt topLeftTile = z_vectorfix_toInt(topLeftCoords);
    ZVectorInt topLeftPixels = z_coords_unitsToPixels(topLeftCoords);

    ZVectorInt topLeftScreen = {
        0 - (topLeftPixels.x & (Z_COORDS_PIXELS_PER_UNIT - 1)),
        0 - (topLeftPixels.y & (Z_COORDS_PIXELS_PER_UNIT - 1))
    };

    if(topLeftCoords.x < 0) {
        topLeftScreen.x += -topLeftTile.x * Z_COORDS_PIXELS_PER_UNIT;
        topLeftTile.x = 0;
    }

    if(topLeftCoords.y < 0) {
        topLeftScreen.y += -topLeftTile.y * Z_COORDS_PIXELS_PER_UNIT;
        topLeftTile.y = 0;
    }

    #define Z_X_TILES \
        ((Z_SCREEN_W + (Z_COORDS_PIXELS_PER_UNIT - 1)) \
            / Z_COORDS_PIXELS_PER_UNIT + 1)

    #define Z_Y_TILES \
        ((Z_SCREEN_H + (Z_COORDS_PIXELS_PER_UNIT - 1)) \
            / Z_COORDS_PIXELS_PER_UNIT + 1)

    ZVectorInt tileEnd = {
        z_math_min(topLeftTile.x + Z_X_TILES, N_MAP_W),
        z_math_min(topLeftTile.y + Z_Y_TILES, N_MAP_H)
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
        ZVectorInt shake = n_camera_shakeGet();

        topLeftScreen.x += shake.x;
        topLeftScreen.y += shake.y;

        *ScreenStart = topLeftScreen;
    }
}

ZVectorInt n_camera_shakeGet(void)
{
    return g_camera.shake;
}

void n_camera_shakeSet(unsigned Ms)
{
    z_timer_start(Z_TIMER_CAMERA_SHAKE, Ms, false);
}
