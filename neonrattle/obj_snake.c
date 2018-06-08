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
#include "util_pool.h"

#define Z_SNAKE_LEN 256
#define Z_SNAKE_LEN_MASK (Z_SNAKE_LEN - 1)

typedef struct {
    ZFix x, y;
    int r, g, b;
    ZColorId targetColor;
} ZSegment;

struct ZSnake {
    ZSegment body[Z_SNAKE_LEN];
    unsigned head, tail;
    ZFixu angle;
    int grow;

};

Z_POOL_DECLARE(ZSnake, 1, g_pool);

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

bool z_snake_tick(ZSnake* Snake)
{
    static bool move = false;

    if(z_button_pressed(Z_BUTTON_A)) {
        move = true;
    }

    if(!move) {
        return true;
    }

    ZSegment* head = &Snake->body[Snake->head];

    ZFix x = head->x + z_fix_cosf(Snake->angle);
    ZFix y = head->y - z_fix_sinf(Snake->angle);

    if(Snake->grow > 0) {
        if(((Snake->head - Snake->tail) & Z_SNAKE_LEN_MASK) == Z_SNAKE_LEN_MASK) {
            Snake->grow = 0;
        } else {
            Snake->grow--;
            Snake->head = (Snake->head + 1) & Z_SNAKE_LEN_MASK;

            ZSegment* s = &Snake->body[Snake->head];
            ZColorId c = z_color_getRandomApple();

            s->x = x;
            s->y = y;
            s->r = z_colors[c].r;
            s->g = z_colors[c].g;
            s->b = z_colors[c].b;
            s->targetColor = z_color_getRandomSnake();
        }
    } else {
        Snake->tail = (Snake->tail + 1) & Z_SNAKE_LEN_MASK;
        Snake->head = (Snake->head + 1) & Z_SNAKE_LEN_MASK;

        ZSegment* s = &Snake->body[Snake->head];
        ZColorId c = z_color_getRandomSnake();

        s->x = x;
        s->y = y;
        s->r = z_colors[c].r;
        s->g = z_colors[c].g;
        s->b = z_colors[c].b;
        s->targetColor = c;
    }

    if(z_button_pressed(Z_BUTTON_LEFT)) {
        Snake->angle += Z_FIX_DEG_001 * 8;
    }

    if(z_button_pressed(Z_BUTTON_RIGHT)) {
        Snake->angle -= Z_FIX_DEG_001 * 8;
    }

    unsigned len = ((Snake->head - Snake->tail) & Z_SNAKE_LEN_MASK) + 1;

    for(unsigned i = Snake->tail; len--; i = (i + 1) & Z_SNAKE_LEN_MASK) {
        ZSegment* s = &Snake->body[i];
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

void z_snake_draw(ZSnake* Snake)
{
    unsigned len = ((Snake->head - Snake->tail) & Z_SNAKE_LEN_MASK) + 1;

    for(unsigned i = Snake->tail; len--; i = (i + 1) & Z_SNAKE_LEN_MASK) {
        ZSegment* s = &Snake->body[i];

        int x, y;
        z_camera_coordsToScreen(s->x, s->y, &x, &y);

        z_sprite_blitAlphaMaskRGBA(Z_SPRITE_SNAKE_ALPHAMASK,
                                   x,
                                   y,
                                   0,
                                   s->r,
                                   s->g,
                                   s->b,
                                   128);
    }
}

bool z_snake_collides(ZSnake* Snake, ZFix X, ZFix Y, int Dim)
{
    ZSegment* head = &Snake->body[Snake->head];

    if(z_collision_boxAndBox(z_fix_toInt(head->x),
                             z_fix_toInt(head->y),
                             4,
                             4,
                             z_fix_toInt(X),
                             z_fix_toInt(Y),
                             Dim,
                             Dim)) {

        Snake->grow += 4;
        return true;
    }

    return false;
}
