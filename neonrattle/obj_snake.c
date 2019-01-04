/*
    Copyright 2018, 2019 Alex Margarit <alex@alxm.org>

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
#include "obj_map.h"
#include "util_camera.h"
#include "util_coords.h"
#include "util_effects.h"
#include "util_fps.h"
#include "util_input.h"
#include "util_light.h"
#include "util_pool.h"
#include "util_sound.h"

#define O_SNAKE_LEN (O_APPLE_NUM_MAX * O_APPLE_GROW_PER)
#define O_SNAKE_LEN_MASK (O_SNAKE_LEN - 1)

#define O_SNAKE_START_LEN 4
#define O_SNAKE_START_ANGLE Z_FIX_DEG_090

#define O_SNAKE_TURN_DEG (Z_FIX_DEG_001 * 12)

#define O_SNAKE_ALPHA_MIN 8
#define O_SNAKE_ALPHA_MAX 128
#define O_SNAKE_TAIL_FADE_RATIO 4

typedef struct {
    ZVectorFix coords;
} OSnakeSegment;

struct OSnake {
    OSnakeSegment body[O_SNAKE_LEN];
    unsigned head, tail;
    ZFixu angle;
    int grow;
    int eaten;
    int r, g, b;
    ZColorId targetColor;
};

Z_POOL_DECLARE(OSnake, 1, g_pool);

void o_snake_setup(void)
{
    z_pool_reset(g_pool);
}

static inline unsigned getLength(const OSnake* Snake)
{
    return ((Snake->head - Snake->tail) & O_SNAKE_LEN_MASK) + 1;
}

static inline void headSet(OSnake* Snake, ZFix X, ZFix Y)
{
    OSnakeSegment* s = &Snake->body[Snake->head];

    s->coords = (ZVectorFix){X, Y};
}

static inline ZFix dimGet(const OSnake* Snake, int Leeway)
{
    Z_UNUSED(Snake);

    return z_coords_pixelsToUnits(
            z_sprite_widthGet(Z_SPRITE_SNAKE_ALPHAMASK) - Leeway);
}

static void colorSet(OSnake* Snake, ZColorId CurrentColor, ZColorId TargetColor)
{
    if(CurrentColor != Z_COLOR_INVALID) {
        Snake->r = z_colors[CurrentColor].r;
        Snake->g = z_colors[CurrentColor].g;
        Snake->b = z_colors[CurrentColor].b;
    }

    Snake->targetColor = TargetColor;
}

static void growAndAdvance(OSnake* Snake)
{
    const OSnakeSegment* head = &Snake->body[Snake->head];

    ZFix x = head->coords.x + z_fix_cosf(Snake->angle) / Z_COORDS_UNIT_PIXELS;
    ZFix y = head->coords.y - z_fix_sinf(Snake->angle) / Z_COORDS_UNIT_PIXELS;

    if(Snake->grow > 0) {
        if(getLength(Snake) == O_SNAKE_LEN) {
            Snake->grow = 0;
        } else {
            Snake->grow--;
            Snake->head = (Snake->head + 1) & O_SNAKE_LEN_MASK;

            headSet(Snake, x, y);
        }
    } else {
        Snake->tail = (Snake->tail + 1) & O_SNAKE_LEN_MASK;
        Snake->head = (Snake->head + 1) & O_SNAKE_LEN_MASK;

        headSet(Snake, x, y);
    }
}

OSnake* o_snake_new(ZFix X, ZFix Y)
{
    OSnake* s = z_pool_alloc(g_pool);

    if(s != NULL) {
        s->head = 0;
        s->tail = 0;
        s->angle = O_SNAKE_START_ANGLE;
        s->grow = O_SNAKE_START_LEN - 1;
        s->eaten = 0;

        headSet(s, X, Y);
        colorSet(s, z_color_snakeGet(), z_color_snakeGet());

        while(s->grow > 0) {
            growAndAdvance(s);
        }
    }

    return s;
}

ZVectorFix o_snake_coordsGet(const OSnake* Snake)
{
    const OSnakeSegment* head = &Snake->body[Snake->head];

    return head->coords;
}

int o_snake_eatenNumGet(const OSnake* Snake)
{
    return Snake->eaten;
}

static void updateColors(OSnake* Snake, bool CycleColors)
{
    const ZColor* target = &z_colors[Snake->targetColor];

    if(Snake->r != target->r) {
        Snake->r += (target->r - Snake->r) >> 3;
    }

    if(Snake->g != target->g) {
        Snake->g += (target->g - Snake->g) >> 3;
    }

    if(Snake->b != target->b) {
        Snake->b += (target->b - Snake->b) >> 3;
    }

    if(CycleColors) {
        colorSet(Snake, Z_COLOR_INVALID, z_color_snakeGet());
    }
}

static bool checkWall(const OSnake* Snake)
{
    const OSnakeSegment* head = &Snake->body[Snake->head];
    ZVectorInt tile = z_vectorfix_toInt(head->coords);

    if(o_map_isWall(tile)) {
        return true;
    }

    return false;
}

static bool checkTail(const OSnake* Snake)
{
    unsigned len = getLength(Snake);
    unsigned skipFromTail = len / O_SNAKE_TAIL_FADE_RATIO;
    unsigned skipFromHead = 32;

    if(len <= skipFromTail + skipFromHead) {
        return false;
    }

    ZVectorFix headCoords = Snake->body[Snake->head].coords;
    ZFix snakeDim = dimGet(Snake, 6);

    len -= skipFromTail + skipFromHead;

    for(unsigned i = (Snake->tail + skipFromTail) & O_SNAKE_LEN_MASK;
        len--;
        i = (i + 1) & O_SNAKE_LEN_MASK) {

        if(z_coords_collideSquares(
            headCoords, snakeDim, Snake->body[i].coords, snakeDim)) {

            return true;
        }
    }

    return false;
}

static void checkApples(OSnake* Snake)
{
    const OSnakeSegment* head = &Snake->body[Snake->head];

    ZVectorInt snakeGrid = z_coords_unitsToGrid(head->coords);
    ZVectorFix snakeGridOffset = z_coords_unitsToGridOffset(head->coords);

    int gridStartX, gridStartY, gridEndX, gridEndY;

    if(snakeGrid.x > 0 && snakeGridOffset.x < Z_FIX_ONE / 2) {
        gridStartX = snakeGrid.x - 1;
    } else {
        gridStartX = snakeGrid.x;
    }

    if(snakeGrid.y > 0 && snakeGridOffset.y < Z_FIX_ONE / 2) {
        gridStartY = snakeGrid.y - 1;
    } else {
        gridStartY = snakeGrid.y;
    }

    if(snakeGrid.x < Z_COORDS_GRID_W - 1
        && snakeGridOffset.x
            > Z_FIX_ONE * Z_COORDS_TILES_PER_GRID - Z_FIX_ONE / 2) {

        gridEndX = snakeGrid.x + 1;
    } else {
        gridEndX = snakeGrid.x;
    }

    if(snakeGrid.y < Z_COORDS_GRID_H - 1
        && snakeGridOffset.y
            > Z_FIX_ONE * Z_COORDS_TILES_PER_GRID - Z_FIX_ONE / 2) {

        gridEndY = snakeGrid.y + 1;
    } else {
        gridEndY = snakeGrid.y;
    }

    const ZFix snakeDim = dimGet(Snake, 2);

    for(int gridY = gridStartY; gridY <= gridEndY; gridY++) {
        for(int gridX = gridStartX; gridX <= gridEndX; gridX++) {
            Z_LIST_ITERATE(o_map_applesListGet(gridX, gridY), OApple*, apple) {
                if(z_coords_collideSquares(head->coords,
                                           snakeDim,
                                           o_apple_coordsGet(apple),
                                           o_apple_dimGet(apple))) {

                    Snake->grow += O_APPLE_GROW_PER;
                    Snake->eaten++;

                    colorSet(
                        Snake, o_apple_colorGet(apple), z_color_snakeGet());

                    o_apple_free(apple);

                    z_effects_circles(head->coords.x, head->coords.y);
                    z_light_pulseSet(Z_LIGHT_APPLE_EAT);
                    z_sfx_play(Z_SFX_APPLE_EAT);
                }
            }
        }
    }
}

void o_snake_tickStart(OSnake* Snake)
{
    updateColors(Snake, true);
}

bool o_snake_tickPlay(OSnake* Snake)
{
    if(z_button_pressGet(Z_BUTTON_LEFT)) {
        Snake->angle += O_SNAKE_TURN_DEG;
    }

    if(z_button_pressGet(Z_BUTTON_RIGHT)) {
        Snake->angle -= O_SNAKE_TURN_DEG;
    }

    updateColors(Snake, true);
    growAndAdvance(Snake);

    if(checkWall(Snake) || checkTail(Snake)) {
        colorSet(Snake, Z_COLOR_INVALID, Z_COLOR_BG_GREEN_03);

        z_light_pulseSet(Z_LIGHT_SNAKE_HIT_WALL);
        z_sfx_play(Z_SFX_HIT_WALL);

        return true;
    }

    checkApples(Snake);

    return false;
}

void o_snake_tickDied(OSnake* Snake)
{
    updateColors(Snake, false);
}

void o_snake_draw(const OSnake* Snake)
{
    unsigned len = getLength(Snake);
    int alpha = O_SNAKE_ALPHA_MIN;
    int alphaInc = (O_SNAKE_ALPHA_MAX - O_SNAKE_ALPHA_MIN)
                        / z_math_max(1, (int)len / O_SNAKE_TAIL_FADE_RATIO);

    for(unsigned i = Snake->tail; len--; i = (i + 1) & O_SNAKE_LEN_MASK) {
        const OSnakeSegment* s = &Snake->body[i];
        ZVectorInt screen = z_camera_coordsToScreen(s->coords);

        if(alpha < O_SNAKE_ALPHA_MAX) {
            alpha += alphaInc;
        }

        z_sprite_blitAlphaMaskRGBA(Z_SPRITE_SNAKE_ALPHAMASK,
                                   screen.x,
                                   screen.y,
                                   ((z_fps_ticksGet() - len) >> 1) & 7,
                                   Snake->r,
                                   Snake->g,
                                   Snake->b,
                                   alpha);
    }
}
