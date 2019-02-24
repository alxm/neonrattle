/*
    Copyright 2019 Alex Margarit <alex@alxm.org>
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

#pragma once

#include "platform.h"

#include "obj_snake.h"

extern void n_game_setup(unsigned Level);

extern void n_game_tick(void);
extern void n_game_draw(void);

extern OSnake* n_game_snakeGet(void);
extern unsigned n_game_levelGet(void);

extern unsigned n_game_scoreGet(void);
extern void n_game_scoreAdd(unsigned Points);
