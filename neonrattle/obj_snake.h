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

#pragma once

#include "util_fix.h"

typedef struct ZSnake ZSnake;

extern void z_snake_setup(void);

extern ZSnake* z_snake_new(ZFix X, ZFix Y);

extern void z_snake_getCoords(const ZSnake* Snake, ZFix* X, ZFix* Y);
extern int z_snake_getDim(const ZSnake* Snake);
extern void z_snake_grow(ZSnake* Snake, int Amount);

extern bool z_snake_tick(ZSnake* Snake);
extern void z_snake_draw(ZSnake* Snake);
