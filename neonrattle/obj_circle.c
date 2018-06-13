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

#include "util_camera.h"
#include "util_list.h"
#include "util_pool.h"
#include "util_screen.h"

#define Z_CIRCLE_ALPHA_START 256
#define Z_CIRCLE_ALPHA_DEC 32

struct ZCircle {
    ZListNode circlesList;
    ZFix x, y;
    int alpha;
};

Z_POOL_DECLARE(ZCircle, 4, g_pool);

void z_circle_setup(void)
{
    z_pool_register(Z_POOL_CIRCLE, g_pool);
}

ZCircle* z_circle_new(ZFix X, ZFix Y)
{
    ZCircle* c = z_pool_alloc(Z_POOL_CIRCLE);

    if(c) {
        z_list_clearNode(&c->circlesList);

        c->x = X;
        c->y = Y;
        c->alpha = Z_CIRCLE_ALPHA_START;
    }

    return c;
}

void z_circle_free(ZCircle* Circle)
{
    z_list_remove(&Circle->circlesList);
    z_pool_free(Z_POOL_CIRCLE, Circle);
}

bool z_circle_tick(ZCircle* Circle)
{
    Circle->alpha -= Z_CIRCLE_ALPHA_DEC;

    return Circle->alpha <= 0;
}

void z_circle_draw(const ZCircle* Circle)
{
    int x, y;
    z_camera_coordsToScreen(Circle->x, Circle->y, &x, &y);

    x += z_screen_getXShake();
    y += z_screen_getYShake();

    z_sprite_blitAlphaMask(Z_SPRITE_APPLE_HALO,
                           x,
                           y,
                           0,
                           Z_COLOR_BG_GREEN_03,
                           Circle->alpha);
}
