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
#include "state.h"
#include "util_camera.h"
#include "util_collision.h"
#include "util_coords.h"
#include "util_effects.h"
#include "util_input.h"
#include "util_map.h"
#include "util_pool.h"

#define Z_SNAKE_LEN (Z_APPLE_NUM_MAX * Z_APPLE_GROW_PER)
#define Z_SNAKE_LEN_MASK (Z_SNAKE_LEN - 1)

#define Z_SNAKE_TURN_DEG (Z_FIX_DEG_001 * 10)

#define Z_SNAKE_ALPHA_MIN 8
#define Z_SNAKE_ALPHA_MAX 128
#define Z_SNAKE_TAIL_FADE_RATIO 4

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

static void z_snake_addHead(ZSnake* Snake, ZFix X, ZFix Y, ZColorId Color)
{
    ZSegment* s = &Snake->body[Snake->head];

    s->x = X;
    s->y = Y;
    s->r = z_colors[Color].r;
    s->g = z_colors[Color].g;
    s->b = z_colors[Color].b;
    s->targetColor = z_color_getRandomSnake();
}

ZSnake* z_snake_new(ZFix X, ZFix Y)
{
    ZSnake* s = z_pool_alloc(Z_POOL_SNAKE);

    if(s != NULL) {
        s->head = 0;
        s->tail = 0;
        s->angle = Z_FIX_DEG_022;
        s->grow = 3;

        z_snake_addHead(s, X, Y, z_color_getRandomSnake());
    }

    return s;
}

void z_snake_getCoords(const ZSnake* Snake, ZFix* X, ZFix* Y)
{
    const ZSegment* head = &Snake->body[Snake->head];

    *X = head->x;
    *Y = head->y;
}

static void moveSnake(ZSnake* Snake)
{
    const ZSegment* head = &Snake->body[Snake->head];

    ZFix x = head->x + z_fix_cosf(Snake->angle);
    ZFix y = head->y - z_fix_sinf(Snake->angle);

    if(Snake->grow > 0) {
        unsigned len = (Snake->head - Snake->tail) & Z_SNAKE_LEN_MASK;

        if(len == Z_SNAKE_LEN_MASK) {
            Snake->grow = 0;
        } else {
            Snake->grow--;
            Snake->head = (Snake->head + 1) & Z_SNAKE_LEN_MASK;

            z_snake_addHead(Snake, x, y, z_color_getRandomApple());
        }
    } else {
        Snake->tail = (Snake->tail + 1) & Z_SNAKE_LEN_MASK;
        Snake->head = (Snake->head + 1) & Z_SNAKE_LEN_MASK;

        z_snake_addHead(Snake, x, y, z_color_getRandomSnake());
    }

    if(z_button_pressed(Z_BUTTON_LEFT)) {
        Snake->angle += Z_SNAKE_TURN_DEG;
    }

    if(z_button_pressed(Z_BUTTON_RIGHT)) {
        Snake->angle -= Z_SNAKE_TURN_DEG;
    }
}

static void updateColors(ZSnake* Snake)
{
    unsigned len = ((Snake->head - Snake->tail) & Z_SNAKE_LEN_MASK) + 1;

    for(unsigned i = Snake->tail; len--; i = (i + 1) & Z_SNAKE_LEN_MASK) {
        ZSegment* s = &Snake->body[i];
        const ZColor* targetColor = &z_colors[s->targetColor];

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
}

static void checkWall(ZSnake* Snake)
{
    const ZSegment* head = &Snake->body[Snake->head];

    int tileX, tileY;
    z_coords_fixToTile(head->x, head->y, &tileX, &tileY);

    if(z_map_isWall(tileX, tileY)) {
        z_sfx_play(Z_SFX_HIT_WALL);
        z_state_set(Z_STATE_PLAY, false);
    }
}

static void checkApples(ZSnake* Snake)
{
    const ZSegment* head = &Snake->body[Snake->head];

    int snakeGridX, snakeGridY;
    z_coords_fixToGrid(head->x, head->y, &snakeGridX, &snakeGridY);

    int snakeGridXOffset, snakeGridYOffset;
    z_coords_fixToGridOffset(
        head->x, head->y, &snakeGridXOffset, &snakeGridYOffset);

    int gridStartX, gridStartY, gridEndX, gridEndY;

    if(snakeGridX > 0 && snakeGridXOffset < Z_TILE_DIM / 2) {
        gridStartX = snakeGridX - 1;
    } else {
        gridStartX = snakeGridX;
    }

    if(snakeGridY > 0 && snakeGridYOffset < Z_TILE_DIM / 2) {
        gridStartY = snakeGridY - 1;
    } else {
        gridStartY = snakeGridY;
    }

    if(snakeGridX < Z_GRID_W - 1
        && snakeGridXOffset > Z_CELL_DIM - Z_TILE_DIM / 2) {

        gridEndX = snakeGridX + 1;
    } else {
        gridEndX = snakeGridX;
    }

    if(snakeGridY < Z_GRID_H - 1
        && snakeGridYOffset > Z_CELL_DIM - Z_TILE_DIM / 2) {

        gridEndY = snakeGridY + 1;
    } else {
        gridEndY = snakeGridY;
    }

    for(int gridY = gridStartY; gridY <= gridEndY; gridY++) {
        for(int gridX = gridStartX; gridX <= gridEndX; gridX++) {
            Z_LIST_ITERATE(z_map_getApples(gridX, gridY), ZApple*, apple) {
                ZFix appleX, appleY;
                z_apple_getCoords(apple, &appleX, &appleY);

                if(z_collision_sqAndSq(z_fix_toInt(head->x),
                                       z_fix_toInt(head->y),
                                       z_sprite_getWidth(
                                            Z_SPRITE_SNAKE_ALPHAMASK),
                                       z_fix_toInt(appleX),
                                       z_fix_toInt(appleY),
                                       z_apple_getDim(apple))) {

                    Snake->grow += Z_APPLE_GROW_PER;

                    z_apple_free(apple);

                    z_effects_circles(head->x, head->y);
                    z_sfx_play(Z_SFX_APPLE_EAT);
                }
            }
        }
    }
}

void z_snake_tick(ZSnake* Snake)
{
    static bool move = false;

    if(z_button_pressed(Z_BUTTON_A)) {
        move = true;
    }

    if(!move) {
        return;
    }

    moveSnake(Snake);
    updateColors(Snake);
    checkWall(Snake);
    checkApples(Snake);
}

void z_snake_draw(ZSnake* Snake)
{
    unsigned len = ((Snake->head - Snake->tail) & Z_SNAKE_LEN_MASK) + 1;
    int alpha = Z_SNAKE_ALPHA_MIN;
    int alphaInc = (Z_SNAKE_ALPHA_MAX - Z_SNAKE_ALPHA_MIN)
                        / z_math_max(1, (int)len / Z_SNAKE_TAIL_FADE_RATIO);

    for(unsigned i = Snake->tail; len--; i = (i + 1) & Z_SNAKE_LEN_MASK) {
        const ZSegment* s = &Snake->body[i];

        int x, y;
        z_camera_coordsToScreen(s->x, s->y, &x, &y);

        if(alpha < Z_SNAKE_ALPHA_MAX) {
            alpha += alphaInc;
        }

        z_sprite_blitAlphaMaskRGBA(Z_SPRITE_SNAKE_ALPHAMASK,
                                   x,
                                   y,
                                   0,
                                   s->r,
                                   s->g,
                                   s->b,
                                   alpha);
    }
}
