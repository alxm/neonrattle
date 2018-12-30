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

#include "obj_circle.h"

#include "util_camera.h"
#include "util_graphics.h"
#include "util_list.h"
#include "util_pool.h"

#define O_CIRCLE_ALPHA_START 256
#define O_CIRCLE_ALPHA_DEC 32

struct OCircle {
    ZListNode circlesList;
    ZVectorFix coords;
    int alpha;
};

Z_POOL_DECLARE(OCircle, 4, g_pool);

void o_circle_setup(void)
{
    z_pool_reset(g_pool);
}

OCircle* o_circle_new(ZFix X, ZFix Y)
{
    OCircle* c = z_pool_alloc(g_pool);

    if(c) {
        z_list_clearNode(&c->circlesList);

        c->coords = (ZVectorFix){X, Y};
        c->alpha = O_CIRCLE_ALPHA_START;
    }

    return c;
}

void o_circle_free(OCircle* Circle)
{
    z_list_remove(&Circle->circlesList);
    z_pool_free(g_pool, Circle);
}

bool o_circle_tick(OCircle* Circle)
{
    Circle->alpha -= O_CIRCLE_ALPHA_DEC;

    return Circle->alpha <= 0;
}

void o_circle_draw(const OCircle* Circle)
{
    ZVectorInt screen = z_camera_coordsToScreen(Circle->coords);
    ZVectorInt shake = z_camera_shakeGet();

    screen.x += shake.x;
    screen.y += shake.y;

    z_sprite_blitAlphaMask(Z_SPRITE_APPLE_HALO,
                           screen.x,
                           screen.y,
                           0,
                           Z_COLOR_BG_GREEN_03,
                           Circle->alpha);
}
