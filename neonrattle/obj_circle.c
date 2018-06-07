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
#include "obj_circle.h"

#include "util_light.h"
#include "util_pool.h"
#include "util_screen.h"

#define Z_CIRCLE_RADIUS_MAX (4)

struct ZCircle {
    ZFix x, y;
    uint8_t radius;
};

Z_POOL_DECLARE(ZCircle, 2, g_pool);

void z_circle_setup(void)
{
    z_pool_register(Z_POOL_CIRCLE, g_pool);
}

void z_circle_init(ZCircle* Circle, ZFix X, ZFix Y)
{
    Circle->x = X;
    Circle->y = Y;
    Circle->radius = 0;

    z_light_setPulse(Z_LIGHT_EXPLOSION);
}

bool z_circle_tick(ZCircle* Circle)
{
    return Circle->radius++ < Z_CIRCLE_RADIUS_MAX;
}

void z_circle_draw(ZCircle* Circle)
{
    int x = z_fix_toInt(Circle->x) + z_screen_getXShake();
    int y = z_fix_toInt(Circle->y) + z_screen_getYShake();

    z_draw_circle(x, y, Circle->radius, Z_COLOR_BG_GREEN_04);
    z_draw_circle(x, y, u8(Circle->radius * 2), Z_COLOR_BG_GREEN_04);
}
