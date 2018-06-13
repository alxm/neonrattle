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
#include "state_died.h"

#include "obj_snake.h"
#include "state_play.h"
#include "util_camera.h"
#include "util_effects.h"
#include "util_map.h"
#include "util_pool.h"
#include "util_timer.h"

static ZSnake* g_snake;

void z_state_died_init(void)
{
    g_snake = z_state_play_getSnake();

    z_timer_start(Z_TIMER_G1, 10);
}

void z_state_died_tick(void)
{
    z_map_tick();
    z_snake_tickDied(g_snake);
    z_effects_tick();

    if(z_timer_expired(Z_TIMER_G1)) {
        z_state_set(Z_STATE_PLAY, false);
    }
}

void z_state_died_draw(void)
{
    z_map_draw();
    z_effects_draw1();
    z_snake_draw(g_snake);
    z_effects_draw2();
}

void z_state_died_free(void)
{
    z_timer_stop(Z_TIMER_G1);

    z_pool_reset(Z_POOL_APPLE);
    z_pool_reset(Z_POOL_SNAKE);
}
