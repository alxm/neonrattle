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

#include "util_camera.h"

#include "util_coords.h"
#include "util_timer.h"

static ZVectorFix g_coords;
static ZVectorInt g_shake;
static uint8_t g_shakeFrames;

void z_camera_reset(void)
{
    g_shake = (ZVectorInt){0, 0};
    g_shakeFrames = 0;
}

void z_camera_tick(const ZSnake* Snake)
{
    ZVectorFix coords = z_snake_coordsGet(Snake);

    g_coords.x = z_math_clamp(coords.x,
                              z_fix_fromInt(Z_SCREEN_W / 2),
                              z_fix_fromInt(Z_MAP_W * Z_TILE_DIM
                                                - Z_SCREEN_W / 2));
    g_coords.y = z_math_clamp(coords.y,
                              z_fix_fromInt(Z_SCREEN_H / 2),
                              z_fix_fromInt(Z_MAP_H * Z_TILE_DIM
                                                - Z_SCREEN_H / 2));

    if(g_shakeFrames) {
        g_shakeFrames--;
        g_shake = (ZVectorInt){-1 + z_random_int(3), -1 + z_random_int(3)};
    } else {
        g_shake = (ZVectorInt){0, 0};
    }
}

ZVectorFix z_camera_originGet(void)
{
    return g_coords;
}

ZVectorInt z_camera_coordsToScreen(ZVectorFix WorldCoords)
{
    return (ZVectorInt){
        Z_SCREEN_W / 2 + z_fix_toInt(WorldCoords.x - g_coords.x),
        Z_SCREEN_H / 2 + z_fix_toInt(WorldCoords.y - g_coords.y)
    };
}

ZVectorInt z_camera_shakeGet(void)
{
    return g_shake;
}

void z_camera_shakeSet(uint8_t Ds)
{
    g_shakeFrames = z_timer_dsToTicks(Ds);
}
