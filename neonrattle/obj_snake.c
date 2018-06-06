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
#include "obj_snake.h"

#include "obj_apple.h"
#include "util_camera.h"
#include "util_collision.h"
#include "util_input.h"
#include "util_pixel.h"

#define Z_SNAKE_LEN 256
#define Z_SNAKE_LEN_MASK (Z_SNAKE_LEN - 1)

typedef struct {
    ZFix x, y;
    int r, g, b;
    ZColorId targetColor;
} ZSegment;

struct ZSnake {
    ZPoolObjHeader poolObject;
    ZSegment body[Z_SNAKE_LEN];
    unsigned head, tail;
    ZFixu angle;
    int grow;
};

#define Z_SNAKE_NUM_MAX (1)

Z_POOL_DECLARE(ZSnake, Z_SNAKE_NUM_MAX, g_pool);

void z_snake_setup(void)
{
    z_pool_register(Z_POOL_SNAKE, g_pool);
}

ZSnake* z_snake_new(ZFix X, ZFix Y)
{
    ZSnake* s = z_pool_alloc(Z_POOL_SNAKE);

    if(s != NULL) {
        s->head = 0;
        s->tail = 0;
        s->angle = Z_FIX_DEG_022;
        s->grow = 4;

        ZColorId c = z_color_getRandomSnake();

        s->body[0].x = X;
        s->body[0].y = Y;
        s->body[0].r = z_colors[c].r;
        s->body[0].g = z_colors[c].g;
        s->body[0].b = z_colors[c].b;
        s->body[0].targetColor = c;
    }

    return s;
}

void z_snake_getCoords(const ZSnake* Snake, ZFix* X, ZFix* Y)
{
    const ZSegment* head = &Snake->body[Snake->head];

    *X = head->x;
    *Y = head->y;
}

bool z_snake_tick(ZPoolObjHeader* Snake, void* Context)
{
    static bool move = false;

    if(z_button_pressed(Z_BUTTON_A)) {
        move = true;
    }

    if(!move) {
        return true;
    }

    Z_UNUSED(Context);

    ZSnake* snake = (ZSnake*)Snake;
    ZSegment* head = &snake->body[snake->head];

    ZFix x = head->x + z_fix_cosf(snake->angle);
    ZFix y = head->y - z_fix_sinf(snake->angle);

    if(snake->grow > 0) {
        if(((snake->head - snake->tail) & Z_SNAKE_LEN_MASK) == Z_SNAKE_LEN_MASK) {
            snake->grow = 0;
        } else {
            snake->grow--;
            snake->head = (snake->head + 1) & Z_SNAKE_LEN_MASK;

            ZSegment* s = &snake->body[snake->head];
            ZColorId c = z_color_getRandomApple();

            s->x = x;
            s->y = y;
            s->r = z_colors[c].r;
            s->g = z_colors[c].g;
            s->b = z_colors[c].b;
            s->targetColor = z_color_getRandomSnake();
        }
    } else {
        snake->tail = (snake->tail + 1) & Z_SNAKE_LEN_MASK;
        snake->head = (snake->head + 1) & Z_SNAKE_LEN_MASK;

        ZSegment* s = &snake->body[snake->head];
        ZColorId c = z_color_getRandomSnake();

        s->x = x;
        s->y = y;
        s->r = z_colors[c].r;
        s->g = z_colors[c].g;
        s->b = z_colors[c].b;
        s->targetColor = c;
    }

    if(z_button_pressed(Z_BUTTON_LEFT)) {
        snake->angle += Z_FIX_DEG_001 * 8;
    }

    if(z_button_pressed(Z_BUTTON_RIGHT)) {
        snake->angle -= Z_FIX_DEG_001 * 8;
    }

    unsigned len = ((snake->head - snake->tail) & Z_SNAKE_LEN_MASK) + 1;

    for(unsigned i = snake->tail; len--; i = (i + 1) & Z_SNAKE_LEN_MASK) {
        ZSegment* s = &snake->body[i];
        ZColor* targetColor = &z_colors[s->targetColor];

        if(s->r != targetColor->r) {
            s->r += (targetColor->r - s->r) / 16;
        }

        if(s->g != targetColor->g) {
            s->g += (targetColor->g - s->g) / 16;
        }

        if(s->b != targetColor->b) {
            s->b += (targetColor->b - s->b) / 16;
        }
    }

    return true;
}

static void safePixel(ZPixel* Buffer, int X, int Y, int R, int G, int B, int A)
{
    if(X < 0 || X >= Z_SCREEN_W || Y < 0 || Y >= Z_SCREEN_H) {
        return;
    }

    z_pixel_drawAlpha(Buffer + Y * Z_SCREEN_W + X, R, G, B, A);
}

void z_snake_draw(ZPoolObjHeader* Snake)
{
    ZSnake* snake = (ZSnake*)Snake;
    unsigned len = ((snake->head - snake->tail) & Z_SNAKE_LEN_MASK) + 1;
    ZPixel* const buffer = z_screen_getPixels();

    ZFix originX, originY;
    z_camera_getOrigin(&originX, &originY);

    for(unsigned i = snake->tail; len--; i = (i + 1) & Z_SNAKE_LEN_MASK) {
        ZSegment* s = &snake->body[i];

        const int x = Z_SCREEN_W / 2 + z_fix_toInt(s->x - originX);
        const int y = Z_SCREEN_H / 2 + z_fix_toInt(s->y - originY);

        safePixel(buffer, x, y, s->r, s->g, s->b, 192);
        safePixel(buffer, x-1, y, s->r, s->g, s->b, 192);
        safePixel(buffer, x, y-1, s->r, s->g, s->b, 192);
        safePixel(buffer, x-1, y-1, s->r, s->g, s->b, 192);

        safePixel(buffer, x-1, y-2, s->r, s->g, s->b, 80);
        safePixel(buffer, x, y-2, s->r, s->g, s->b, 80);

        safePixel(buffer, x-1, y+1, s->r, s->g, s->b, 80);
        safePixel(buffer, x, y+1, s->r, s->g, s->b, 80);

        safePixel(buffer, x-2, y-1, s->r, s->g, s->b, 80);
        safePixel(buffer, x-2, y, s->r, s->g, s->b, 80);

        safePixel(buffer, x+1, y-1, s->r, s->g, s->b, 80);
        safePixel(buffer, x+1, y, s->r, s->g, s->b, 80);

        safePixel(buffer, x+1, y-2, s->r, s->g, s->b, 32);
        safePixel(buffer, x+1, y+1, s->r, s->g, s->b, 32);
        safePixel(buffer, x-2, y+1, s->r, s->g, s->b, 32);
        safePixel(buffer, x-2, y-2, s->r, s->g, s->b, 32);
    }
}

typedef struct {
    int x, y;
    int dim;
    bool hit;
} ZCollisionContext;

static bool collisionCheck(ZPoolObjHeader* Snake, void* Context)
{
    ZCollisionContext* context = (ZCollisionContext*)Context;

    if(context->hit) {
        return true;
    }

    ZSnake* snake = (ZSnake*)Snake;
    ZSegment* head = &snake->body[snake->head];

    context->hit = z_collision_boxAndBox(z_fix_toInt(head->x),
                                         z_fix_toInt(head->y),
                                         4,
                                         4,
                                         context->x,
                                         context->y,
                                         context->dim,
                                         context->dim);

    if(context->hit) {
        snake->grow += 4;
    }

    return true;
}

bool z_snake_collides(ZFix X, ZFix Y, int Dim)
{
    ZCollisionContext context = {
        z_fix_toInt(X),
        z_fix_toInt(Y),
        Dim,
        false
    };

    z_pool_tick(Z_POOL_SNAKE, collisionCheck, &context);

    return context.hit;
}
