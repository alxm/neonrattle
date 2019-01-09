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

#pragma once

#include "platform.h"

#include "util_fix.h"

#define O_SNAKE_LIFE_MAX 64

typedef enum {
    O_SNAKE_FLAG_HURT = Z_FLAG_BIT(1),
    O_SNAKE_FLAG_ATE = Z_FLAG_BIT(2),
    O_SNAKE_FLAG_DEAD = Z_FLAG_BIT(3),
} OSnakeFlags;

typedef struct OSnake OSnake;

extern void o_snake_setup(void);

extern OSnake* o_snake_new(ZFix X, ZFix Y);

extern ZVectorFix o_snake_coordsGet(const OSnake* Snake);
extern int o_snake_eatenNumGet(const OSnake* Snake);
extern int o_snake_lifeGet(const OSnake* Snake);
extern bool o_snake_flagsTest(const OSnake* Snake, OSnakeFlags Flags);

extern void o_snake_tickStart(OSnake* Snake);
extern void o_snake_tickPlay(OSnake* Snake);
extern void o_snake_tickDied(OSnake* Snake);
extern void o_snake_draw(const OSnake* Snake);
