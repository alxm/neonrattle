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

#include "obj_snake.h"

#include "obj_apple.h"
#include "util_camera.h"
#include "util_collision.h"
#include "util_coords.h"
#include "util_effects.h"
#include "util_input.h"
#include "util_light.h"
#include "util_map.h"
#include "util_pool.h"
#include "util_screen.h"
#include "util_sound.h"

#define Z_SNAKE_LEN (Z_APPLE_NUM_MAX * Z_APPLE_GROW_PER)
#define Z_SNAKE_LEN_MASK (Z_SNAKE_LEN - 1)

#define Z_SNAKE_START_LEN 4
#define Z_SNAKE_START_ANGLE Z_FIX_DEG_090

#define Z_SNAKE_TURN_DEG (Z_FIX_DEG_001 * 12)

#define Z_SNAKE_ALPHA_MIN 8
#define Z_SNAKE_ALPHA_MAX 128
#define Z_SNAKE_TAIL_FADE_RATIO 4

typedef struct {
    ZVectorFix coords;
    int r, g, b;
    ZColorId targetColor;
} ZSegment;

struct ZSnake {
    ZSegment body[Z_SNAKE_LEN];
    unsigned head, tail;
    ZFixu angle;
    int grow;
    int eaten;
};

Z_POOL_DECLARE(ZSnake, 1, g_pool);

void z_snake_setup(void)
{
    z_pool_register(Z_POOL_SNAKE, g_pool);
}

static inline unsigned getLength(const ZSnake* Snake)
{
    return ((Snake->head - Snake->tail) & Z_SNAKE_LEN_MASK) + 1;
}

static void setHead(ZSnake* Snake, ZFix X, ZFix Y, ZColorId Color)
{
    ZSegment* s = &Snake->body[Snake->head];

    s->coords = (ZVectorFix){X, Y};
    s->r = z_colors[Color].r;
    s->g = z_colors[Color].g;
    s->b = z_colors[Color].b;
    s->targetColor = z_color_snakeGet();
}

ZSnake* z_snake_new(ZFix X, ZFix Y)
{
    ZSnake* s = z_pool_alloc(Z_POOL_SNAKE);

    if(s != NULL) {
        s->head = (unsigned)-1;
        s->tail = 0;
        s->angle = Z_SNAKE_START_ANGLE;
        s->grow = 0;
        s->eaten = 0;

        for(int i = Z_SNAKE_START_LEN; i--; ) {
            s->head = (s->head + 1) & Z_SNAKE_LEN_MASK;
            setHead(s, X, Y, z_color_snakeGet());

            X += z_fix_cosf(s->angle);
            Y -=z_fix_sinf(s->angle);
        }
    }

    return s;
}

ZVectorFix z_snake_coordsGet(const ZSnake* Snake)
{
    const ZSegment* head = &Snake->body[Snake->head];

    return head->coords;
}

int z_snake_eatenNumGet(const ZSnake* Snake)
{
    return Snake->eaten;
}

static void moveSnake(ZSnake* Snake)
{
    const ZSegment* head = &Snake->body[Snake->head];

    ZFix x = head->coords.x + z_fix_cosf(Snake->angle);
    ZFix y = head->coords.y - z_fix_sinf(Snake->angle);

    if(Snake->grow > 0) {
        if(getLength(Snake) == Z_SNAKE_LEN) {
            Snake->grow = 0;
        } else {
            Snake->grow--;
            Snake->head = (Snake->head + 1) & Z_SNAKE_LEN_MASK;

            setHead(Snake, x, y, z_color_appleGet());
        }
    } else {
        Snake->tail = (Snake->tail + 1) & Z_SNAKE_LEN_MASK;
        Snake->head = (Snake->head + 1) & Z_SNAKE_LEN_MASK;

        setHead(Snake, x, y, z_color_snakeGet());
    }

    if(z_button_pressGet(Z_BUTTON_LEFT)) {
        Snake->angle += Z_SNAKE_TURN_DEG;
    }

    if(z_button_pressGet(Z_BUTTON_RIGHT)) {
        Snake->angle -= Z_SNAKE_TURN_DEG;
    }
}

static void updateColors(ZSnake* Snake)
{
    for(unsigned len = getLength(Snake), i = Snake->tail;
        len--;
        i = (i + 1) & Z_SNAKE_LEN_MASK) {

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

static bool checkWall(ZSnake* Snake)
{
    const ZSegment* head = &Snake->body[Snake->head];
    ZVectorInt tile = z_coords_fixToTile(head->coords);

    if(z_map_isWall(tile.x, tile.y)) {
        for(unsigned len = getLength(Snake), i = Snake->tail;
            len--;
            i = (i + 1) & Z_SNAKE_LEN_MASK) {

            Snake->body[i].targetColor = Z_COLOR_BG_GREEN_03;
        }

        z_light_pulseSet(Z_LIGHT_SNAKE_HIT_WALL);
        z_sfx_play(Z_SFX_HIT_WALL);

        return true;
    }

    return false;
}

static void checkApples(ZSnake* Snake)
{
    const ZSegment* head = &Snake->body[Snake->head];

    ZVectorInt snakeGrid = z_coords_fixToGrid(head->coords);
    ZVectorInt snakeGridOffset = z_coords_fixToGridOffset(head->coords);

    int gridStartX, gridStartY, gridEndX, gridEndY;

    if(snakeGrid.x > 0 && snakeGridOffset.x < Z_TILE_DIM / 2) {
        gridStartX = snakeGrid.x - 1;
    } else {
        gridStartX = snakeGrid.x;
    }

    if(snakeGrid.y > 0 && snakeGridOffset.y < Z_TILE_DIM / 2) {
        gridStartY = snakeGrid.y - 1;
    } else {
        gridStartY = snakeGrid.y;
    }

    if(snakeGrid.x < Z_GRID_W - 1
        && snakeGridOffset.x > Z_CELL_DIM - Z_TILE_DIM / 2) {

        gridEndX = snakeGrid.x + 1;
    } else {
        gridEndX = snakeGrid.x;
    }

    if(snakeGrid.y < Z_GRID_H - 1
        && snakeGridOffset.y > Z_CELL_DIM - Z_TILE_DIM / 2) {

        gridEndY = snakeGrid.y + 1;
    } else {
        gridEndY = snakeGrid.y;
    }

    for(int gridY = gridStartY; gridY <= gridEndY; gridY++) {
        for(int gridX = gridStartX; gridX <= gridEndX; gridX++) {
            Z_LIST_ITERATE(z_map_applesListGet(gridX, gridY), ZApple*, apple) {
                ZVectorFix coords = z_apple_coordsGet(apple);

                if(z_collision_sqAndSq(z_fix_toInt(head->coords.x),
                                       z_fix_toInt(head->coords.y),
                                       z_sprite_widthGet(
                                            Z_SPRITE_SNAKE_ALPHAMASK),
                                       z_fix_toInt(coords.x),
                                       z_fix_toInt(coords.y),
                                       z_apple_dimGet(apple))) {

                    Snake->grow += Z_APPLE_GROW_PER;
                    Snake->eaten++;

                    z_apple_free(apple);

                    z_effects_circles(head->coords.x, head->coords.y);
                    z_light_pulseSet(Z_LIGHT_APPLE_EAT);
                    z_sfx_play(Z_SFX_APPLE_EAT);
                }
            }
        }
    }
}

bool z_snake_tick(ZSnake* Snake)
{
    moveSnake(Snake);
    updateColors(Snake);
    bool hitWall = checkWall(Snake);
    checkApples(Snake);

    return hitWall;
}

void z_snake_tickDied(ZSnake* Snake)
{
    updateColors(Snake);
}

void z_snake_draw(const ZSnake* Snake)
{
    unsigned len = getLength(Snake);
    int alpha = Z_SNAKE_ALPHA_MIN;
    int alphaInc = (Z_SNAKE_ALPHA_MAX - Z_SNAKE_ALPHA_MIN)
                        / z_math_max(1, (int)len / Z_SNAKE_TAIL_FADE_RATIO);

    for(unsigned i = Snake->tail; len--; i = (i + 1) & Z_SNAKE_LEN_MASK) {
        const ZSegment* s = &Snake->body[i];

        ZVectorInt screen = z_camera_coordsToScreen(s->coords);
        ZVectorInt shake = z_screen_shakeGet();

        screen.x += shake.x;
        screen.y += shake.y;

        if(alpha < Z_SNAKE_ALPHA_MAX) {
            alpha += alphaInc;
        }

        z_sprite_blitAlphaMaskRGBA(Z_SPRITE_SNAKE_ALPHAMASK,
                                   screen.x,
                                   screen.y,
                                   0,
                                   s->r,
                                   s->g,
                                   s->b,
                                   alpha);
    }
}
