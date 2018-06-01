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

static struct {
    ZFix x, y;
} g_camera;

void z_camera_tick(const ZSnake* Snake)
{
    ZFix x, y;
    z_snake_getCoords(Snake, &x, &y);

    g_camera.x = x;
    g_camera.y = y;
}

void z_camera_getOrigin(ZFix* X, ZFix* Y)
{
    *X = g_camera.x;
    *Y = g_camera.y;
}
