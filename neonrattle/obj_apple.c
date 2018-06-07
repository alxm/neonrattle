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
#include "util_list.h"
#include "util_pixel.h"
#include "util_pool.h"

#define Z_APPLE_ALPHA_STEP 8
#define Z_APPLE_ALPHA_MIN 128
#define Z_APPLE_ALPHA_MAX 256

struct ZApple {
    ZListNode applesListNode;
    ZFix x, y;
    int dim;
    int alpha, alphaDir;
    ZColorId color;
};

const size_t z_apple_listNodeOffset0 = offsetof(ZApple, applesListNode);

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
        a->dim = 4;
        a->alpha = Z_APPLE_ALPHA_MIN
            + (z_random_int(Z_APPLE_ALPHA_MAX - Z_APPLE_ALPHA_MIN)
                & ~(Z_APPLE_ALPHA_STEP - 1));
        a->alphaDir = -1 + z_random_int(2) * 2;
        a->color = z_color_getRandomApple();
    }

    return a;
}

static void z_apple_free(ZApple* Apple)
{
    z_list_remove(&Apple->applesListNode);
    z_pool_free(Z_POOL_APPLE, Apple);
}

void z_apple_tick(ZApple* Apple, ZSnake* Snake)
{
    Apple->alpha += Apple->alphaDir * Z_APPLE_ALPHA_STEP;

    if(Apple->alpha <= Z_APPLE_ALPHA_MIN) {
        Apple->alpha = Z_APPLE_ALPHA_MIN;
        Apple->alphaDir = 1;
    } else if(Apple->alpha >= Z_APPLE_ALPHA_MAX) {
        Apple->alpha = Z_APPLE_ALPHA_MAX;
        Apple->alphaDir = -1;
    }

    if(z_snake_collides(Snake, Apple->x, Apple->y, Apple->dim)) {
        z_apple_free(Apple);
    }
}

static void pixelAlpha(ZPixel* Buffer, int X, int Y, int R, int G, int B, int A)
{
    if(X < 0 || X >= Z_SCREEN_W || Y < 0 || Y >= Z_SCREEN_H) {
        return;
    }

    z_pixel_drawAlpha(Buffer + Y * Z_SCREEN_W + X, R, G, B, A);
}

static void hlineAlpha(ZPixel* Buffer, int X1, int X2, int Y, int R, int G, int B, int A)
{
    if(Y < 0 || Y >= Z_SCREEN_H || X2 < 0 || X1 >= Z_SCREEN_W) {
        return;
    }

    if(X1 < 0) {
        X1 = 0;
    }

    if(X2 >= Z_SCREEN_W) {
        X2 = Z_SCREEN_W - 1;
    }

    Buffer += Y * Z_SCREEN_W + X1;

    for(int x = X2 - X1; x >= 0; x--, Buffer++) {
        z_pixel_drawAlpha(Buffer, R, G, B, A);
    }
}

void z_apple_draw(ZApple* Apple)
{
    int x, y;
    z_camera_coordsToScreen(Apple->x, Apple->y, &x, &y);

    ZPixel* const buffer = z_screen_getPixels();

    const int a100 = Apple->alpha;
    const int a050 = (128 * a100) >> 8;
    const int r = z_colors[Apple->color].r;
    const int g = z_colors[Apple->color].g;
    const int b = z_colors[Apple->color].b;

    hlineAlpha(buffer, x - 1, x, y - 3, r, g, b, a100);
    hlineAlpha(buffer, x - 2, x + 1, y - 2, r, g, b, a100);
    hlineAlpha(buffer, x - 3, x - 2, y - 1, r, g, b, a100);
    hlineAlpha(buffer, x + 1, x + 2, y - 1, r, g, b, a100);
    hlineAlpha(buffer, x - 3, x - 2, y, r, g, b, a100);
    hlineAlpha(buffer, x + 1, x + 2, y, r, g, b, a100);
    hlineAlpha(buffer, x - 2, x + 1, y + 1, r, g, b, a100);
    hlineAlpha(buffer, x - 1, x, y + 2, r, g, b, a100);
    pixelAlpha(buffer, x - 2, y - 3, r, g, b, a050);
    pixelAlpha(buffer, x + 1, y - 3, r, g, b, a050);
    pixelAlpha(buffer, x - 3, y - 2, r, g, b, a050);
    pixelAlpha(buffer, x + 2, y - 2, r, g, b, a050);
    pixelAlpha(buffer, x - 3, y + 1, r, g, b, a050);
    pixelAlpha(buffer, x + 2, y + 1, r, g, b, a050);
    pixelAlpha(buffer, x - 2, y + 2, r, g, b, a050);
    pixelAlpha(buffer, x + 1, y + 2, r, g, b, a050);
    hlineAlpha(buffer, x - 1, x, y - 1, r, g, b, a050);
    hlineAlpha(buffer, x - 1, x, y, r, g, b, a050);
}
