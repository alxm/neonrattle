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
#include "obj_apple.h"

#include "obj_snake.h"
#include "util_camera.h"
#include "util_pixel.h"

#define Z_APPLE_ALPHA_STEP 8
#define Z_APPLE_ALPHA_MIN 128
#define Z_APPLE_ALPHA_MAX 256

struct ZApple {
    ZPoolObjHeader poolObject;
    ZFix x, y;
    ZFix dim;
    int alpha, alphaDir;
};

#define Z_APPLE_NUM_MAX (256)

Z_POOL_DECLARE(ZApple, Z_APPLE_NUM_MAX, g_pool);

void z_apple_setup(void)
{
    z_pool_register(Z_POOL_APPLE, g_pool);
}

ZApple* z_apple_new(ZFix X, ZFix Y)
{
    ZApple* a = z_pool_alloc(Z_POOL_APPLE);

    if(a != NULL) {
        a->x = X;
        a->y = Y;
        a->dim = 2;
        a->alpha = Z_APPLE_ALPHA_MIN
            + (z_random_int(Z_APPLE_ALPHA_MAX - Z_APPLE_ALPHA_MIN)
                & ~(Z_APPLE_ALPHA_STEP - 1));
        a->alphaDir = -1 + z_random_int(2) * 2;
    }

    return a;
}

bool z_apple_tick(ZPoolObjHeader* Apple, void* Context)
{
    Z_UNUSED(Context);

    ZApple* apple = (ZApple*)Apple;

    apple->alpha += apple->alphaDir * Z_APPLE_ALPHA_STEP;

    if(apple->alpha <= Z_APPLE_ALPHA_MIN) {
        apple->alpha = Z_APPLE_ALPHA_MIN;
        apple->alphaDir = 1;
    } else if(apple->alpha >= Z_APPLE_ALPHA_MAX) {
        apple->alpha = Z_APPLE_ALPHA_MAX;
        apple->alphaDir = -1;
    }

    return !z_snake_collides(apple->x, apple->y, 2);
}

static void safePixel(ZPixel* Buffer, int X, int Y, int R, int G, int B, int A)
{
    if(X < 0 || X >= Z_SCREEN_W || Y < 0 || Y >= Z_SCREEN_H) {
        return;
    }

    z_pixel_drawAlpha(Buffer + Y * Z_SCREEN_W + X, R, G, B, A);
}

void z_apple_draw(ZPoolObjHeader* Apple)
{
    ZApple* apple = (ZApple*)Apple;

    ZFix originX, originY;
    z_camera_getOrigin(&originX, &originY);

    const int x = Z_SCREEN_W / 2 + z_fix_toInt(apple->x - originX);
    const int y = Z_SCREEN_H / 2 + z_fix_toInt(apple->y - originY);

    ZPixel* const buffer = z_screen_getPixels();

    const int a0 = apple->alpha;
    const int a1 = (192 * apple->alpha) >> 8;
    const int a2 = (80 * apple->alpha) >> 8;
    const int a3 = (32 * apple->alpha) >> 8;

    safePixel(buffer, x, y, Z_APPLE_R, Z_APPLE_G, Z_APPLE_B, a1);
    safePixel(buffer, x-1, y, Z_APPLE_R, Z_APPLE_G, Z_APPLE_B, a1);
    safePixel(buffer, x, y-1, Z_APPLE_R, Z_APPLE_G, Z_APPLE_B, a0);
    safePixel(buffer, x-1, y-1, Z_APPLE_R, Z_APPLE_G, Z_APPLE_B, a1);

    safePixel(buffer, x-1, y-2, Z_APPLE_R, Z_APPLE_G, Z_APPLE_B, a2);
    safePixel(buffer, x, y-2, Z_APPLE_R, Z_APPLE_G, Z_APPLE_B, a2);

    safePixel(buffer, x-1, y+1, Z_APPLE_R, Z_APPLE_G, Z_APPLE_B, a2);
    safePixel(buffer, x, y+1, Z_APPLE_R, Z_APPLE_G, Z_APPLE_B, a2);

    safePixel(buffer, x-2, y-1, Z_APPLE_R, Z_APPLE_G, Z_APPLE_B, a2);
    safePixel(buffer, x-2, y, Z_APPLE_R, Z_APPLE_G, Z_APPLE_B, a2);

    safePixel(buffer, x+1, y-1, Z_APPLE_R, Z_APPLE_G, Z_APPLE_B, a2);
    safePixel(buffer, x+1, y, Z_APPLE_R, Z_APPLE_G, Z_APPLE_B, a2);

    safePixel(buffer, x+1, y-2, Z_APPLE_R, Z_APPLE_G, Z_APPLE_B, a3);
    safePixel(buffer, x+1, y+1, Z_APPLE_R, Z_APPLE_G, Z_APPLE_B, a3);
    safePixel(buffer, x-2, y+1, Z_APPLE_R, Z_APPLE_G, Z_APPLE_B, a3);
    safePixel(buffer, x-2, y-2, Z_APPLE_R, Z_APPLE_G, Z_APPLE_B, a3);
}
