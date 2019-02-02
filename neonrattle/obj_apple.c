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

#include "obj_apple.h"

#include "obj_camera.h"
#include "util_coords.h"
#include "util_list.h"
#include "util_pool.h"

#define O_APPLE_BOUNCE_DEG_STEP (8 * Z_DEG_001_FIX)
#define O_APPLE_ALPHA_DEG_STEP (4 * Z_DEG_001_FIX)
#define O_APPLE_ALPHA_DEG_STEP_FADE (Z_DEG_001_FIX)
#define O_APPLE_ALPHA_MIN 192
#define O_APPLE_ALPHA_MAX 256

struct OApple {
    ZListNode nodeGrid;
    ZVectorFix coords;
    ZFixu bounceAngle;
    ZFixu alphaAngle;
    ZColorId color;
    bool eaten;
};

const size_t o_apple_listNodeOffsets[O_APPLE_LIST_NUM] = {
    [O_APPLE_LIST_GRID] = offsetof(OApple, nodeGrid),
};

Z_POOL_DECLARE(OApple, O_APPLE_NUM_MAX, g_pool);

void o_apple_setup(void)
{
    z_pool_reset(g_pool);
}

OApple* o_apple_new(ZFix X, ZFix Y)
{
    OApple* a = z_pool_alloc(g_pool);

    if(a != NULL) {
        z_list_clearNode(&a->nodeGrid);

        a->coords = (ZVectorFix){X, Y};
        a->bounceAngle = z_random_intu(z_fixu_fromInt(Z_ANGLES_NUM));
        a->alphaAngle = z_random_intu(z_fixu_fromInt(Z_ANGLES_NUM));
        a->color = z_color_appleGet();
        a->eaten = false;
    }

    return a;
}

void o_apple_free(OApple* Apple)
{
    z_list_remove(&Apple->nodeGrid);
    z_pool_free(g_pool, Apple);
}

ZVectorFix o_apple_coordsGet(const OApple* Apple)
{
    return Apple->coords;
}

ZFix o_apple_dimGet(const OApple* Apple)
{
    Z_UNUSED(Apple);

    return z_coords_pixelsToUnits(
            z_sprite_widthGet(Z_SPRITE_APPLE_MASK) - 2);
}

ZColorId o_apple_colorGet(const OApple* Apple)
{
    return Apple->color;
}

bool o_apple_eatGet(const OApple* Apple)
{
    return Apple->eaten;
}

void o_apple_eatSet(OApple* Apple)
{
    Apple->alphaAngle = 0;
    Apple->eaten = true;
}

void o_apple_tick(OApple* Apple)
{
    if(Apple->eaten) {
        Apple->alphaAngle += O_APPLE_ALPHA_DEG_STEP_FADE;

        if(Apple->alphaAngle >= Z_DEG_090_FIX) {
            o_apple_free(Apple);
        }
    } else {
        Apple->bounceAngle += O_APPLE_BOUNCE_DEG_STEP;
        Apple->alphaAngle += O_APPLE_ALPHA_DEG_STEP;
    }
}

void o_apple_draw(const OApple* Apple)
{
    ZVectorInt screen = o_camera_coordsToScreen(Apple->coords);
    ZSpriteId sprite;
    int alpha;

    if(Apple->eaten) {
        sprite = Z_SPRITE_APPLE_HALO;
        alpha = z_fix_toInt(z_fix_sinf(Z_DEG_090_FIX - Apple->alphaAngle)
                                * O_APPLE_ALPHA_MIN);
    } else {
        sprite = Z_SPRITE_APPLE_MASK;
        alpha =
            O_APPLE_ALPHA_MIN + (O_APPLE_ALPHA_MAX - O_APPLE_ALPHA_MIN) / 2
                + z_fix_toInt(z_fix_sinf(Apple->alphaAngle)
                                * (O_APPLE_ALPHA_MAX - O_APPLE_ALPHA_MIN) / 2);

        screen.x += z_fix_toInt(z_fix_sinf(Apple->bounceAngle + Z_DEG_090_FIX));
        screen.y += z_fix_toInt(z_fix_sinf(Apple->bounceAngle) * 3 / 2);
    }

    z_sprite_blitAlphaMask(sprite, screen.x, screen.y, 0, Apple->color, alpha);
}
