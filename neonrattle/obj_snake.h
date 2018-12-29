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

#pragma once

#include "platform.h"

#include "util_fix.h"

typedef struct OSnake OSnake;

extern void o_snake_setup(void);

extern OSnake* o_snake_new(ZFix X, ZFix Y);

extern ZVectorFix o_snake_coordsGet(const OSnake* Snake);
extern int o_snake_eatenNumGet(const OSnake* Snake);

extern bool o_snake_tick(OSnake* Snake);
extern void o_snake_tickDied(OSnake* Snake);
extern void o_snake_draw(const OSnake* Snake);
