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

#include "obj_apple.h"

#include "util_camera.h"
#include "util_graphics.h"
#include "util_list.h"
#include "util_pool.h"

#define O_APPLE_BOUNCE_DEG_STEP (8 * Z_FIX_DEG_001)
#define O_APPLE_ALPHA_DEG_STEP (8 * Z_FIX_DEG_001)
#define O_APPLE_ALPHA_MIN 128
#define O_APPLE_ALPHA_MAX 256

struct OApple {
    ZListNode nodeGrid;
    ZVectorFix coords;
    ZFixu bounceAngle;
    ZFixu alphaAngle;
    ZColorId color;
};

const size_t o_apple_listNodeOffsets[O_APPLE_LIST_NUM] = {
    [O_APPLE_LIST_GRID] = offsetof(OApple, nodeGrid),
};

Z_POOL_DECLARE(OApple, O_APPLE_NUM_MAX, g_pool);

void o_apple_setup(void)
{
    z_pool_setup(Z_POOL_APPLE, g_pool);
}

OApple* o_apple_new(ZFix X, ZFix Y)
{
    OApple* a = z_pool_alloc(Z_POOL_APPLE);

    if(a != NULL) {
        z_list_clearNode(&a->nodeGrid);

        a->coords = (ZVectorFix){X, Y};
        a->bounceAngle = z_random_intu(z_fixu_fromInt(Z_ANGLES_NUM));
        a->alphaAngle = z_random_intu(z_fixu_fromInt(Z_ANGLES_NUM));
        a->color = z_color_appleGet();
    }

    return a;
}

void o_apple_free(OApple* Apple)
{
    z_list_remove(&Apple->nodeGrid);
    z_pool_free(Z_POOL_APPLE, Apple);
}

ZVectorFix o_apple_coordsGet(const OApple* Apple)
{
    return Apple->coords;
}

int o_apple_dimGet(const OApple* Apple)
{
    Z_UNUSED(Apple);

    return z_sprite_widthGet(Z_SPRITE_APPLE_ALPHAMASK);
}

void o_apple_tick(OApple* Apple)
{
    Apple->bounceAngle += O_APPLE_BOUNCE_DEG_STEP;
    Apple->alphaAngle += O_APPLE_ALPHA_DEG_STEP;
}

void o_apple_draw(const OApple* Apple)
{
    ZVectorInt screen = z_camera_coordsToScreen(Apple->coords);
    ZVectorInt shake = z_camera_shakeGet();

    screen.x += shake.x;
    screen.y += shake.y;

    screen.x += z_fix_toInt(z_fix_sinf(Apple->bounceAngle + Z_FIX_DEG_090));
    screen.y += z_fix_toInt(z_fix_sinf(Apple->bounceAngle) * 3 / 2);

    int alpha = O_APPLE_ALPHA_MIN
        + (O_APPLE_ALPHA_MAX - O_APPLE_ALPHA_MIN) / 2
        + z_fix_toInt(z_fix_sinf(Apple->alphaAngle)
                        * (O_APPLE_ALPHA_MAX - O_APPLE_ALPHA_MIN) / 2);

    z_sprite_blitAlphaMask(
        Z_SPRITE_APPLE_ALPHAMASK, screen.x, screen.y, 0, Apple->color, alpha);
}
