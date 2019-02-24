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

#include "obj_camera.h"
#include "obj_map.h"
#include "obj_hud.h"
#include "util_save.h"

typedef struct {
    unsigned level;
    unsigned score;
    OSnake* snake;
} NGame;

static NGame g_game;

void n_game_setup(unsigned Level)
{
    o_apple_setup();
    o_snake_setup();

    ZFix startX, startY;
    n_map_init(Level, &startX, &startY);

    g_game.level = Level;
    g_game.score = 0;
    g_game.snake = o_snake_new(startX, startY);
}

void n_game_tick(void)
{
    n_map_tick();
    o_snake_tick(g_game.snake);
    n_camera_tick(o_snake_coordsGet(g_game.snake));
    n_hud_tick(g_game.snake);
}

void n_game_draw(void)
{
    n_map_draw();
    o_snake_draw(g_game.snake);
    n_map_drawMinimap(o_snake_coordsGet(g_game.snake));
    n_hud_draw(g_game.snake);
}

OSnake* n_game_snakeGet(void)
{
    return g_game.snake;
}

unsigned n_game_levelGet(void)
{
    return g_game.level;
}

unsigned n_game_scoreGet(void)
{
    return g_game.score;
}

void n_game_scoreAdd(unsigned Points)
{
    g_game.score += Points;
}
