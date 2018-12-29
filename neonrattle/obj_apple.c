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
#include "obj_apple.h"

#include "util_camera.h"
#include "util_list.h"
#include "util_pool.h"

#define Z_APPLE_BOUNCE_DEG_STEP (8 * Z_FIX_DEG_001)
#define Z_APPLE_ALPHA_DEG_STEP (8 * Z_FIX_DEG_001)
#define Z_APPLE_ALPHA_MIN 128
#define Z_APPLE_ALPHA_MAX 256

struct ZApple {
    ZListNode nodeGrid;
    ZVectorFix coords;
    ZFixu bounceAngle;
    ZFixu alphaAngle;
    ZColorId color;
};

const size_t z_apple_listNodeOffsets[Z_APPLE_LIST_NUM] = {
    [Z_APPLE_LIST_GRID] = offsetof(ZApple, nodeGrid),
};

Z_POOL_DECLARE(ZApple, Z_APPLE_NUM_MAX, g_pool);

void z_apple_setup(void)
{
    z_pool_register(Z_POOL_APPLE, g_pool);
}

ZApple* z_apple_new(ZFix X, ZFix Y)
{
    ZApple* a = z_pool_alloc(Z_POOL_APPLE);

    if(a != NULL) {
        z_list_clearNode(&a->nodeGrid);

        a->coords = (ZVectorFix){X, Y};
        a->bounceAngle = z_random_intu(z_fixu_fromInt(Z_ANGLES_NUM));
        a->alphaAngle = z_random_intu(z_fixu_fromInt(Z_ANGLES_NUM));
        a->color = z_color_appleGet();
    }

    return a;
}

void z_apple_free(ZApple* Apple)
{
    z_list_remove(&Apple->nodeGrid);
    z_pool_free(Z_POOL_APPLE, Apple);
}

ZVectorFix z_apple_coordsGet(const ZApple* Apple)
{
    return Apple->coords;
}

int z_apple_dimGet(const ZApple* Apple)
{
    Z_UNUSED(Apple);

    return z_sprite_widthGet(Z_SPRITE_APPLE_ALPHAMASK);
}

void z_apple_tick(ZApple* Apple)
{
    Apple->bounceAngle += Z_APPLE_BOUNCE_DEG_STEP;
    Apple->alphaAngle += Z_APPLE_ALPHA_DEG_STEP;
}

void z_apple_draw(const ZApple* Apple)
{
    ZVectorInt screen = z_camera_coordsToScreen(Apple->coords);
    ZVectorInt shake = z_screen_shakeGet();

    screen.x += shake.x;
    screen.y += shake.y;

    screen.x += z_fix_toInt(z_fix_sinf(Apple->bounceAngle + Z_FIX_DEG_090));
    screen.y += z_fix_toInt(z_fix_sinf(Apple->bounceAngle) * 3 / 2);

    int alpha = Z_APPLE_ALPHA_MIN
        + (Z_APPLE_ALPHA_MAX - Z_APPLE_ALPHA_MIN) / 2
        + z_fix_toInt(z_fix_sinf(Apple->alphaAngle)
                        * (Z_APPLE_ALPHA_MAX - Z_APPLE_ALPHA_MIN) / 2);

    z_sprite_blitAlphaMask(
        Z_SPRITE_APPLE_ALPHAMASK, screen.x, screen.y, 0, Apple->color, alpha);
}
