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

#include "state_play.h"

#include "obj_map.h"
#include "obj_snake.h"
#include "util_camera.h"
#include "util_effects.h"
#include "util_hud.h"
#include "util_input.h"
#include "util_light.h"

static OSnake* g_snake;

void s_play_init(void)
{
    g_snake = z_state_contextGet();
}

void s_play_tick(void)
{
    o_map_tick();
    bool died = o_snake_tick(g_snake);
    z_camera_tick(g_snake);
    z_effects_tick();

    if(died) {
        z_state_set(Z_STATE_DIED);
        z_state_contextSet(g_snake);
    }
}

void s_play_draw(void)
{
    o_map_draw();
    z_effects_draw1();
    o_snake_draw(g_snake);
    z_effects_draw2();
    z_hud_draw(g_snake);
}

void s_play_free(void)
{
    //
}
