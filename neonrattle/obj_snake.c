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

#include "obj_snake.h"

#include "obj_camera.h"
#include "obj_game.h"
#include "obj_map.h"
#include "util_coords.h"
#include "util_fps.h"
#include "util_light.h"
#include "util_pool.h"
#include "util_sound.h"
#include "util_state.h"

#define O_SNAKE_LEN (O_APPLE_NUM_MAX)
#define O_SNAKE_LEN_MASK (O_SNAKE_LEN - 1)

#define O_SNAKE_START_LEN 4
#define O_SNAKE_START_ANGLE Z_DEG_090_FIX

#define O_SNAKE_TURN_DEG (Z_DEG_001_FIX * 12)

#define O_SNAKE_ALPHA_MIN 8
#define O_SNAKE_ALPHA_MAX 128
#define O_SNAKE_TAIL_FADE_RATIO 4

// May enter at most 1/2 inside a wall tile
#define O_SNAKE_DAMAGE_WALL (O_SNAKE_LIFE_MAX / (Z_COORDS_PIXELS_PER_UNIT / 2))
#define O_SNAKE_DAMAGE_TAIL (O_SNAKE_LIFE_MAX / 32)

typedef struct {
    ZVectorFix coords;
} OSnakeSegment;

struct OSnake {
    OSnakeSegment body[O_SNAKE_LEN];
    unsigned head, tail;
    ZFixu angle;
    int grow;
    int eaten;
    int life;
    OSnakeFlags flags;
    ZRgb rgb;
    const ZRgb* rgbTarget;
};

Z_POOL_DECLARE(OSnake, 1, g_pool);

void o_snake_reset(void)
{
    z_pool_clear(g_pool);
}

static inline unsigned getLength(const OSnake* Snake)
{
    return ((Snake->head - Snake->tail) & O_SNAKE_LEN_MASK) + 1;
}

static inline void headSet(OSnake* Snake, ZVectorFix Coords)
{
    Snake->body[Snake->head].coords = Coords;
}

static inline ZFix dimGet(const OSnake* Snake, int Leeway)
{
    Z_UNUSED(Snake);

    return z_coords_pixelsToUnits(
            z_sprite_sizeGetWidth(Z_SPRITE_SNAKE_MASK) - Leeway);
}

static void colorSet(OSnake* Snake, ZColorId CurrentColor, ZColorId TargetColor)
{
    if(CurrentColor != Z_COLOR_INVALID) {
        Snake->rgb = z_colors[CurrentColor].rgb;
    }

    Snake->rgbTarget = &z_colors[TargetColor].rgb;
}

static void growAndAdvance(OSnake* Snake)
{
    const OSnakeSegment* head = &Snake->body[Snake->head];

    ZVectorFix newHeadCoords = {
        head->coords.x + z_fix_cosf(Snake->angle) / Z_COORDS_PIXELS_PER_UNIT,
        head->coords.y - z_fix_sinf(Snake->angle) / Z_COORDS_PIXELS_PER_UNIT
    };

    if(Snake->grow > 0) {
        if(getLength(Snake) == O_SNAKE_LEN) {
            Snake->grow = 0;
        } else {
            Snake->grow--;
            Snake->head = (Snake->head + 1) & O_SNAKE_LEN_MASK;

            headSet(Snake, newHeadCoords);
        }
    } else {
        Snake->tail = (Snake->tail + 1) & O_SNAKE_LEN_MASK;
        Snake->head = (Snake->head + 1) & O_SNAKE_LEN_MASK;

        headSet(Snake, newHeadCoords);
    }
}

OSnake* o_snake_new(ZVectorFix Coords)
{
    OSnake* s = z_pool_alloc(g_pool);

    if(s != NULL) {
        s->head = 0;
        s->tail = 0;
        s->angle = O_SNAKE_START_ANGLE;
        s->grow = O_SNAKE_START_LEN - 1;
        s->eaten = 0;
        s->life = O_SNAKE_LIFE_MAX;
        s->flags = 0;

        headSet(s, Coords);
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

void o_snake_turnLeft(OSnake* Snake)
{
    Snake->angle += O_SNAKE_TURN_DEG;
}

void o_snake_turnRight(OSnake* Snake)
{
    Snake->angle -= O_SNAKE_TURN_DEG;
}

int o_snake_eatenNumGet(const OSnake* Snake)
{
    return Snake->eaten;
}

int o_snake_lifeGet(const OSnake* Snake)
{
    return Snake->life;
}

bool o_snake_flagsTest(const OSnake* Snake, OSnakeFlags Flags)
{
    return Z_FLAG_TEST_ALL(Snake->flags, Flags);
}

static void updateColors(OSnake* Snake, bool CycleColors)
{
    const ZRgb* current = &Snake->rgb;
    const ZRgb* target = Snake->rgbTarget;

    Snake->rgb.r += (target->r - current->r) >> 3;
    Snake->rgb.g += (target->g - current->g) >> 3;
    Snake->rgb.b += (target->b - current->b) >> 3;

    if(CycleColors) {
        colorSet(Snake, Z_COLOR_INVALID, z_color_snakeGet());
    }
}

static bool checkWall(const OSnake* Snake)
{
    const OSnakeSegment* head = &Snake->body[Snake->head];
    ZVectorInt tile = z_vectorfix_toInt(head->coords);

    if(n_map_isWall(tile)) {
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

static bool checkApples(OSnake* Snake)
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
    int eaten = Snake->eaten;

    for(int gridY = gridStartY; gridY <= gridEndY; gridY++) {
        for(int gridX = gridStartX; gridX <= gridEndX; gridX++) {
            Z_LIST_ITERATE(n_map_applesListGet(gridX, gridY), OApple*, apple) {
                if(o_apple_eatGet(apple)) {
                    continue;
                }

                if(z_coords_collideSquares(head->coords,
                                           snakeDim,
                                           o_apple_coordsGet(apple),
                                           o_apple_dimGet(apple))) {

                    Snake->grow++;
                    Snake->eaten++;

                    n_map_appleEat(apple);
                    n_game_scoreAdd(1);

                    colorSet(
                        Snake, o_apple_colorGet(apple), z_color_snakeGet());

                    z_light_pulseSet(Z_LIGHT_APPLE_EAT);
                    z_sfx_play(Z_SFX_APPLE_EAT);
                }
            }
        }
    }

    return Snake->eaten > eaten;
}

void o_snake_tick(OSnake* Snake)
{
    ZStateId state = z_state_getCurrent();

    if(state == Z_STATE_END) {
        colorSet(Snake, z_color_appleGet(), z_color_snakeGet());
    }

    updateColors(Snake, state != Z_STATE_DIED);

    Z_FLAG_CLEAR(Snake->flags, O_SNAKE_FLAG_HURT | O_SNAKE_FLAG_ATE);

    if(state == Z_STATE_PLAY) {
        growAndAdvance(Snake);

        int damage = 0;

        if(checkWall(Snake)) {
            damage += O_SNAKE_DAMAGE_WALL;
            z_light_pulseSet(Z_LIGHT_SNAKE_HIT_WALL);
        }

        if(checkTail(Snake)) {
            damage += O_SNAKE_DAMAGE_TAIL;
            z_light_pulseSet(Z_LIGHT_SNAKE_HIT_SELF);
        }

        if(damage > 0) {
            Z_FLAG_SET(Snake->flags, O_SNAKE_FLAG_HURT);
            Snake->life = z_math_max(Snake->life - damage, 0);

            n_camera_shakeSet(100);
            z_sfx_play(Z_SFX_HIT_WALL);

            if(Snake->life == 0) {
                Z_FLAG_SET(Snake->flags, O_SNAKE_FLAG_DEAD);
                colorSet(Snake, Z_COLOR_INVALID, Z_COLOR_BG_GREEN_03);
            }
        }

        if(checkApples(Snake)) {
            Z_FLAG_SET(Snake->flags, O_SNAKE_FLAG_ATE);
        }
    }
}

void o_snake_draw(const OSnake* Snake)
{
    ZStateId state = z_state_getCurrent();

    unsigned len = getLength(Snake);
    unsigned frameBase = state == Z_STATE_DIED ? 0 : (z_fps_ticksGet() >> 2);
    unsigned frameMask = z_sprite_framesNumGet(Z_SPRITE_SNAKE_MASK) - 1u;
    int alpha = O_SNAKE_ALPHA_MIN;
    int alphaInc = (O_SNAKE_ALPHA_MAX - O_SNAKE_ALPHA_MIN)
                        / z_math_max(1, (int)len / O_SNAKE_TAIL_FADE_RATIO);

    z_sprite_align(Z_ALIGN_X_CENTER | Z_ALIGN_Y_CENTER);

    for(unsigned i = Snake->tail; len--; i = (i + 1) & O_SNAKE_LEN_MASK) {
        const OSnakeSegment* s = &Snake->body[i];
        ZVectorInt screen = n_camera_coordsToScreen(s->coords);

        if(alpha < O_SNAKE_ALPHA_MAX) {
            alpha += alphaInc;
        }

        z_graphics_colorSetRgb(&Snake->rgb);
        z_graphics_alphaSet(alpha);

        // frameTicks == animation speed, (len >> 3) == sprite frames cycle
        z_sprite_blitAlphaMask(Z_SPRITE_SNAKE_MASK,
                               (frameBase - (len >> 3)) & frameMask,
                               screen.x,
                               screen.y);
    }
}
