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
#include "obj_circle.h"

#include "util_camera.h"
#include "util_list.h"
#include "util_pool.h"
#include "util_screen.h"

#define Z_CIRCLE_ALPHA_START 256
#define Z_CIRCLE_ALPHA_DEC 32

struct ZCircle {
    ZListNode circlesList;
    ZVectorFix coords;
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

        c->coords = (ZVectorFix){X, Y};
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
    ZVectorInt screen = z_camera_coordsToScreen(Circle->coords);
    ZVectorInt shake = z_screen_shakeGet();

    screen.x += shake.x;
    screen.y += shake.y;

    z_sprite_blitAlphaMask(Z_SPRITE_APPLE_HALO,
                           screen.x,
                           screen.y,
                           0,
                           Z_COLOR_BG_GREEN_03,
                           Circle->alpha);
}
