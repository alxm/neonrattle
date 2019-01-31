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

#include "obj_game.h"

#include "obj_map.h"
#include "util_save.h"

typedef struct {
    unsigned level;
    unsigned score;
    OSnake* snake;
} OGame;

static OGame g_game;

void o_game_setup(unsigned Level)
{
    g_game.level = Level;
    g_game.score = 0;
}

OSnake* o_game_snakeGet(void)
{
    return g_game.snake;
}

unsigned o_game_levelGet(void)
{
    return g_game.level;
}

void o_game_levelNext(void)
{
    ZFix startX, startY;
    o_map_init(g_game.level, &startX, &startY);

    g_game.level++;
    g_game.score = 0;
    g_game.snake = o_snake_new(startX, startY);
}

unsigned o_game_scoreGet(void)
{
    return g_game.score;
}

void o_game_scoreAdd(unsigned Points)
{
    g_game.score += Points;
}
