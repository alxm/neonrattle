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
#include "util_camera.h"

#include "util_map.h"

static struct {
    ZFix x, y;
} g_camera;

void z_camera_tick(const ZSnake* Snake)
{
    ZFix x, y;
    z_snake_getCoords(Snake, &x, &y);

    g_camera.x = z_math_clamp(x,
                              z_fix_fromInt(Z_SCREEN_W / 2),
                              z_fix_fromInt(Z_MAP_W * Z_TILE_DIM
                                                - Z_SCREEN_W / 2));
    g_camera.y = z_math_clamp(y,
                              z_fix_fromInt(Z_SCREEN_H / 2),
                              z_fix_fromInt(Z_MAP_H * Z_TILE_DIM
                                                - Z_SCREEN_H / 2));
}

void z_camera_getOrigin(ZFix* X, ZFix* Y)
{
    *X = g_camera.x;
    *Y = g_camera.y;
}
