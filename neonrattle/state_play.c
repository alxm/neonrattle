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
#include "state_play.h"

#include "obj_apple.h"
#include "obj_snake.h"
#include "util_camera.h"
#include "util_fix.h"
#include "util_map.h"
#include "util_pool.h"
#include "util_screen.h"

static struct {
    ZSnake* snake;
} g_context;

void z_state_play_init(void)
{
    // place apples and player on map
    z_map_init();

    g_context.snake = z_pool_getFirst(Z_POOL_SNAKE);
}

void z_state_play_tick(void)
{
    z_pool_tick(Z_POOL_SNAKE, z_snake_tick, NULL);
    z_pool_tick(Z_POOL_APPLE, z_apple_tick, NULL);
    z_camera_tick(g_context.snake);
}

void z_state_play_draw(void)
{
    z_map_draw();
    z_pool_draw(Z_POOL_SNAKE, z_snake_draw);
    z_pool_draw(Z_POOL_APPLE, z_apple_draw);
}
